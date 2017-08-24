
#ifndef SSAO
#define SSAO

const float ssaoCompareEpsilon = 0.0001;
const float ssaoFadeoutArea = 0.01;

mat3 getRandomizedTBN(
    vec3 normal,
    vec2 uv,
    sampler2D depthTexture,
    sampler2DArray noiseTexture,
    int currentFrame)
{
    vec2 noiseTexsize = vec2(textureSize(noiseTexture, 0));
    vec2 colorTexsize = vec2(textureSize(depthTexture, 0));

    vec3 r = texture(noiseTexture, vec3(uv * (colorTexsize / noiseTexsize), currentFrame)).rgb * 2.0 - 1.0;

    vec3 t = normalize(cross(normal, r));
    vec3 b = cross(normal, t);

    return mat3(t, b, normal);
}

float ssao(
    vec2 uv,
    sampler2D depthTexture,
    sampler2D normalTexture,
    sampler2DArray noiseTexture,
    sampler1D kernelTexture,
    mat4 viewProjectionMatrix,
    mat4 viewProjectionMatrixInverse,
    float radius,
    int currentFrame
)
{
    vec3 normal = texture(normalTexture, uv).xyz;
    float depth = texture(depthTexture, uv).x;

    vec4 ndc = vec4(uv * 2.0 - 1.0, depth, 1.0);
    vec4 worldPosition = viewProjectionMatrixInverse * ndc;
    worldPosition /= worldPosition.w;

    float occlusion = 0.0;

    mat3 tbn = getRandomizedTBN(normal, uv, depthTexture, noiseTexture, currentFrame);

    int numSamples = textureSize(kernelTexture, 0);

    for (int i = 0; i < numSamples; ++i) {
        vec3 sampleOffset = tbn * texelFetch(kernelTexture, i, 0).xyz;
        sampleOffset *= radius;

        vec3 samplePosition = worldPosition.xyz + sampleOffset;

        // project the sample position to NDC
        vec4 projectedPosition = viewProjectionMatrix * vec4(samplePosition, 1.0);
        projectedPosition /= projectedPosition.w;

        vec2 sampleUV = projectedPosition.xy * 0.5 + 0.5;
        float sampleDepth = projectedPosition.z;

        float compareDepth = texture(depthTexture, sampleUV).x;

        // compare distance in world position in order to not generate halos
        vec4 compareNdc = vec4(sampleUV * 2.0 - 1.0, compareDepth, 1.0);
        vec4 compareWorldPos = viewProjectionMatrixInverse * compareNdc;
        compareWorldPos /= compareWorldPos.w;
        float rangeCheck = 1.0 - smoothstep(radius - ssaoFadeoutArea, radius + ssaoFadeoutArea, distance(worldPosition, compareWorldPos));

        occlusion += step(compareDepth, sampleDepth - ssaoCompareEpsilon) * rangeCheck;
    }

    return 1.0 - occlusion / numSamples;
}

#endif // SSAO
