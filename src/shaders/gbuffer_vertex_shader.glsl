#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
flat out int hasTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool useTexture;

void main()
{
    // Pass the texture if needed
    TexCoords = useTexture ? aTexCoords : vec2(0.0, 0.0);
    hasTexture = useTexture ? 1 : 0;

    // Frag pos in camera space
    FragPos = vec3(view * model * vec4(aPos, 1.0));

    // Normal vector in camera space
    Normal = mat3(transpose(inverse(view * model))) * aNormal;

    gl_Position = projection * vec4(FragPos, 1.0);
}

