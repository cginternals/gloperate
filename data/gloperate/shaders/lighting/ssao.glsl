
#ifndef SSAO
#define SSAO
/*
vec3 kernel(
    const in float i,
    const in vec4 samplerSizes,
    const in sampler1D kernelSampler)
{
    return texture(kernelSampler, i * samplerSizes[1]).xyz;
}

vec3 normal(
    const in vec2 uv,
    const in sampler2D normalTexture)
{
    return texture(normalTexture, uv).xyz;
}

// returns values in [nearZ:farZ]
float linearDepth(
    const in vec2 uv,
    const in mat4 projectionMatrix,
    const in sampler2D depthTexture)
{
    float d = texture(depthTexture, uv).x;
    return projectionMatrix[3][2] / (d + projectionMatrix[2][2]);
}

mat3 noised(
    const in vec3 normal,
    in vec2 uv,
    const in vec2 screenSize,
    const in vec4 samplerSizes,
    const in sampler2DArray noiseSampler,
    const in int noiseIndex)
{
    uv *= screenSize * samplerSizes[3];

    vec3 random = texture(noiseSampler, vec3(uv, noiseIndex)).xyz * vec3(2,2,0) - vec3(1,1,0);

    // orientation matrix
    vec3 t = normalize(random - normal * dot(random, normal));
    vec3 b = cross(normal, t);

    return mat3(t, b, normal);
}

float ssaoKernel(
    const in vec2 uv,
    const in vec3 origin,
    const in float radius,
    const in float intensity,
    const in sampler1D kernelSampler,
    const in sampler2DArray noiseSampler,
    const in sampler2D depthTexture,
    const in sampler2D normalTexture,
    const in mat4 projectionMatrix,
    const in mat3 normalMatrix,
    const in int noiseIndex)
{
    int kernelSize = textureSize(kernelSampler, 0);
    ivec3 noiseSize = textureSize(noiseSampler, 0);

    vec4 samplerSizes = vec4(kernelSize, 1.0 / float(kernelSize), noiseSize.x, 1.0 / float(noiseSize.x)); // expected: [kernelSize, 1 / kernelSize, noiseSize, 1 / noiseSize]
    vec3 screenspaceNormal = normalMatrix * normal(uv, normalTexture);

    // randomized orientation matrix for hemisphere based on face normal
    mat3 m = noised(screenspaceNormal, uv, vec2(textureSize(depthTexture, 0)), samplerSizes, noiseSampler, noiseIndex);

    float ao = 0.0;

    for (float i = 0.0; i < samplerSizes[0]; ++i)
    {
        vec3 s = m * kernel(i, samplerSizes, kernelSampler);
        s *= 2.0 * radius;
        s += origin;

        vec4 s_offset = projectionMatrix * vec4(s, 1.0);
        s_offset.xyz /= s_offset.w;

        s_offset.xy = s_offset.xy * 0.5 + 0.5;

        float sd = -linearDepth(s_offset.xy, projectionMatrix, depthTexture);

        float ndcRangeCheck = 1.0 - float(any(greaterThan(s_offset.xyz, vec3(1.0))) || any(lessThan(s_offset.xyz, vec3(0.0))));
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(-origin.z + sd));
        ao += rangeCheck * ndcRangeCheck * float(sd > s.z);
    }

    return pow(1.0 - (ao * samplerSizes[1]), intensity);
}


vec3 ssao(
    const in vec2 uv,
    const in vec3 ssaoColor,
    const in float farZ,
    const in float radius,
    const in float intensity,
    const in sampler1D kernelSampler,
    const in sampler2DArray noiseSampler,
    const in sampler2D depthTexture,
    const in sampler2D normalTexture,
    const in mat4 projectionMatrix,
    const in mat4 projectionInverseMatrix,
    const in mat3 normalMatrix,
    const in int noiseIndex)
{
    float d = linearDepth(uv, projectionMatrix, depthTexture);

    if (d > farZ)
        return vec3(1.0f);

    vec4 eye = (projectionInverseMatrix * vec4(2.0*(uv - vec2(0.5)), 1.0, 1.0));
    eye.xyz /= eye.w;
    eye.xyz /= farZ;
    // eye has a z of -1 here

    vec3 origin = eye.xyz * d;

    float v = ssaoKernel(
        uv,
        origin,
        radius,
        intensity,
        kernelSampler,
        noiseSampler,
        depthTexture,
        normalTexture,
        projectionMatrix,
        normalMatrix,
        noiseIndex);

    return mix(ssaoColor, vec3(1.0f), v);
}

// ======================================================================================================
*/
const float compareEpsilon = 0.0001;
const float fadeoutArea = 0.01;

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
        float rangeCheck = 1.0 - smoothstep(radius - fadeoutArea, radius + fadeoutArea, distance(worldPosition, compareWorldPos));

        occlusion += step(compareDepth, sampleDepth - compareEpsilon) * rangeCheck;
    }

    return 1.0 - occlusion / numSamples;
}

#endif // SSAO
