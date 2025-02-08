#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Light {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

struct PhongProperties{
    float ambientStrength;
    float specularStrength;
    float shininess;
};

uniform Light light;
uniform PhongProperties phongProperties;
uniform mat4 view;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    // Transform light position to camera space
    vec3 lightPosCamSpace = vec3(view * vec4(light.position, 1.0));

    // ambient
    vec3 ambient = phongProperties.ambientStrength * light.color * texture(texture_diffuse1, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosCamSpace - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color * texture(texture_diffuse1, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), phongProperties.shininess);
    vec3 specular = phongProperties.specularStrength * spec * light.color *  texture(texture_specular1, TexCoords).rgb;

    // attenuation
    float distance    = length(lightPosCamSpace - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}

