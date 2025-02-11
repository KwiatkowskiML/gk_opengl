#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;

    vec3 direction;
    float cutOff;
    float outerCutOff;
};

struct PhongProperties{
    float ambientStrength;
    float specularStrength;
    float shininess;
};

struct FogProperties{
    float fogStart;
    float fogEnd;
    vec3 fogColor;
};

#define MAX_POINT_LIGHTS 10
uniform Light pointLights[MAX_POINT_LIGHTS];

uniform Light light;
uniform SpotLight spotLight;
uniform PhongProperties phongProperties;
uniform FogProperties fogProperties;
uniform mat4 view;

vec3 CalcPointLight(Light light);
vec3 CalcSpotLight(SpotLight light);

void main()
{
    vec3 result = vec3(0.0);
    vec3 pointLightning = CalcPointLight(light);
    vec3 spotLightning = CalcSpotLight(spotLight);

    for(int i = 0; i < MAX_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i]);

    // fog
    vec3 lighting = result + pointLightning + spotLightning;

    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    float distance = length(FragPos);

    float fogFactor = clamp((fogProperties.fogEnd - distance) / (fogProperties.fogEnd - fogProperties.fogStart), 0.0, 1.0);
    vec3 finalColor = mix(fogProperties.fogColor, lighting, fogFactor);

    FragColor = vec4(finalColor, 1.0);
}

vec3 CalcPointLight(Light light)
{
    // Calculate light position in camera space
    vec3 lightPosCamSpace = vec3(view * vec4(light.position, 1.0));

    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    // Ambient component
    vec3 ambient = phongProperties.ambientStrength * light.color * Diffuse;

    // Diffuse component
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosCamSpace - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color * Diffuse;

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), phongProperties.shininess);
    vec3 specular = spec * light.color *  Specular;

    // attenuation
    float distance    = length(lightPosCamSpace - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 pointLightning = ambient + diffuse + specular;
    return pointLightning;
}

vec3 CalcSpotLight(SpotLight light)
{
    // Calculate light position in camera space
    vec3 lightPosCamSpace = vec3(view * vec4(light.position, 1.0));
    vec3 directionCameraSpace = mat3(transpose(inverse(view))) * light.direction;

    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    // Diffuse component
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosCamSpace - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), phongProperties.shininess);

    // attenuation
    float distance    = length(lightPosCamSpace - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(lightDir, normalize(-directionCameraSpace));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = phongProperties.ambientStrength * light.color * Diffuse;
    vec3 diffuse = diff * light.color * Diffuse;
    vec3 specular = spec * light.color *  Specular;

    ambient  *= attenuation;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}
