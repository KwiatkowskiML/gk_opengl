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

struct PhongProperties {
    float ambientStrength;
    float specularStrength;
    float shininess;
};

struct FogProperties {
    vec3 color;     // Fog color
    float density;  // Controls fog intensity
    float start;    // Distance where fog begins
    float end;      // Distance where fog is fully opaque
};

uniform Light light;
uniform PhongProperties phongProperties;
uniform FogProperties fogProperties;
uniform mat4 view;

// Linear fog calculation
float calculateLinearFog(float distance) {
    // Fog factor interpolates between 0 (no fog) and 1 (full fog)
    float fogFactor = (fogProperties.end - distance) / (fogProperties.end - fogProperties.start);
    return clamp(1.0 - fogFactor, 0.0, 1.0);
}

// Exponential fog calculation
float calculateExponentialFog(float distance) {
    // Density controls how quickly fog increases
    float fogFactor = exp(-distance * fogProperties.density);
    return 1.0 - fogFactor;
}

// Exponential squared fog calculation (more dramatic)
float calculateExponentialSquaredFog(float distance) {
    float fogFactor = exp(-pow(distance * fogProperties.density, 2.0));
    return 1.0 - fogFactor;
}

void main()
{
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
    float distance = length(lightPosCamSpace - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 lightning = ambient + diffuse + specular;

    // Fog calculation (choose one method)
    // 1. Linear Fog
    // float fogFactor = calculateLinearFog(distance);

    // 2. Exponential Fog
    float fogFactor = calculateExponentialFog(distance);

    // 3. Exponential Squared Fog
    // float fogFactor = calculateExponentialSquaredFog(distance);

    // Mix the original color with fog color based on fog factor
    vec3 finalColor = mix(lightning, fogProperties.color, fogFactor);

    FragColor = vec4(finalColor, 1.0);
}