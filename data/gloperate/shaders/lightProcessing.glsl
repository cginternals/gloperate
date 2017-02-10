#ifndef LIGHT_PROCESSING
#define LIGHT_PROCESSING

#define LIGHT_TYPE_AMBIENT 1
#define LIGHT_TYPE_DIRECTIONAL 2
#define LIGHT_TYPE_POINT 3
#define LIGHT_TYPE_POINT_ATT 4

#ifdef LIGHT_PROCESSING_PHONG
    #include </gloperate/shaders/lightProcessingPhong.glsl>
#else
    // Default: Diffuse only
    #include </gloperate/shaders/lightProcessingDiffuse.glsl>
#endif

vec3 lightIntensity(
    vec3 worldPosition,
    vec3 diffuseColor,
    vec3 specularColor,
    vec3 normal,
    float glossiness,
    vec3 eyePosition,
    samplerBuffer colorTypeData,
    samplerBuffer positionData,
    samplerBuffer attenuationData)
{
    vec3 intensity = vec3(0.0);

    for (int i = 0; i < textureSize(colorTypeData); i++)
    {
        vec4 colorType = texelFetch(colorTypeData, i);
        vec3 lightColor = colorType.rgb;
        int lightType = int(colorType.a);
        vec3 lightPosition = texelFetch(positionData, i).xyz;
        vec3 attenuationCoefficients = texelFetch(attenuationData, i).xyz;

        intensity += singleLightIntensity(
            worldPosition,
            diffuseColor,
            specularColor,
            normal,
            glossiness,
            eyePosition,
            lightType,
            lightColor,
            lightPosition,
            attenuationCoefficients);
    }

    return intensity;
}

#endif //LIGHT_PROCESSING
