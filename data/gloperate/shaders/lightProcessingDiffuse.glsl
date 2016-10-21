vec3 singleLightIntensity(
    vec3 worldPosition,
    vec3 diffuseColor,
    vec3 specularColor,
    vec3 normal,
    float glossiness,
    vec3 eyePosition,
    int lightType,
    vec3 lightColor,
    vec3 lightPosition,
    vec3 attenuationCoefficients)
{
    switch(lightType)
    {
    case LIGHT_TYPE_AMBIENT:
        {
            return diffuseColor * lightColor;
        }
    case LIGHT_TYPE_DIRECTIONAL:
        {
            return diffuseColor * lightColor * max(dot(normalize(lightPosition), normal), 0.0);
        }
    case LIGHT_TYPE_POINT:
        {
            vec3 L = lightPosition - worldPosition;
            return diffuseColor * lightColor * max(dot(normalize(L), normal), 0.0);
        }
    case LIGHT_TYPE_POINT_ATT:
        {
            vec3 L = lightPosition - worldPosition;
            float d = length(L);
            float attenuation = 1 / (attenuationCoefficients.x + attenuationCoefficients.y * d + attenuationCoefficients.z * d*d);
            return diffuseColor * lightColor * attenuation * max(dot(normalize(L), normal), 0.0);
        }
    default:
        return vec3(0.0);
    }
}
