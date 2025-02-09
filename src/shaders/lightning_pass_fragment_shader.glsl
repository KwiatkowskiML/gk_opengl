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

struct PhongProperties{
    float ambientStrength;
    float specularStrength;
    float shininess;
};

uniform Light light;
uniform PhongProperties phongProperties;
uniform vec3 view;

void main()
{
    vec3 lightPosCamSpace = light.position;

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
    vec3 viewDir = normalize(view - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), phongProperties.shininess);
    vec3 specular = spec * light.color *  Specular;


    vec3 lightning = ambient + diffuse + specular;
    FragColor = vec4(specular, 1.0);
}

