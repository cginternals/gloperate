#ifndef SSAO
#define SSAO

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
    const in sampler2D noiseSampler)
{
    uv *= screenSize * samplerSizes[3];

    vec3 random = texture(noiseSampler, uv).xyz;

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
    const in sampler2D noiseSampler,
    const in sampler2D depthTexture,
    const in sampler2D normalTexture,
    const in mat4 projectionMatrix,
    const in mat3 normalMatrix)
{
    int kernelSize = textureSize(kernelSampler, 0);
    ivec2 noiseSize = textureSize(noiseSampler, 0);

    vec4 samplerSizes = vec4(kernelSize, 1.0 / float(kernelSize), noiseSize.x, 1.0 / float(noiseSize.x)); // expected: [kernelSize, 1 / kernelSize, noiseSize, 1 / noiseSize]
    vec3 screenspaceNormal = normalMatrix * normal(uv, normalTexture);

    // randomized orientation matrix for hemisphere based on face normal
    mat3 m = noised(screenspaceNormal, uv, vec2(textureSize(depthTexture, 0)), samplerSizes, noiseSampler);

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
    const in sampler2D noiseSampler,
    const in sampler2D depthTexture,
    const in sampler2D normalTexture,
    const in mat4 projectionMatrix,
    const in mat4 projectionInverseMatrix,
    const in mat3 normalMatrix)
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
        normalMatrix);

    return mix(ssaoColor, vec3(1.0f), v);
}

#endif // SSAO
