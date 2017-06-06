
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
            vec3 V = normalize(eyePosition - worldPosition);
            vec3 R = normalize(2 * normal - normalize(L));
            float diffusePart = max(dot(normalize(L), normal), 0.0);
            float n = 1 / pow(1 - glossiness, 3);
            // [TODO]: search for source or explanation of this specular part computation
            float specularPart = (2+n) / 6.283185307179586 * pow(max(dot(R,V), 0.0), n);
            return mix(diffuseColor * diffusePart, specularColor * specularPart, glossiness) * lightColor;
        }
    case LIGHT_TYPE_POINT_ATT:
        {
            vec3 L = lightPosition - worldPosition;
            vec3 V = normalize(eyePosition - worldPosition);
            vec3 R = normalize(2 * normal - normalize(L));
            float d = length(L);
            float attenuation = 1 / (attenuationCoefficients.x + attenuationCoefficients.y * d + attenuationCoefficients.z * d*d);
            float diffusePart = max(dot(normalize(L), normal), 0.0);
            float n = 1 / pow(1 - glossiness, 3);
            // [TODO]: search for source or explanation of this specular part computation
            float specularPart = (2+n) / 6.283185307179586 * pow(max(dot(R,V), 0.0), n);
            return mix(diffuseColor * diffusePart, specularColor * specularPart, glossiness) * lightColor * attenuation;
        }
    default:
        return vec3(0.0);
    }
}
