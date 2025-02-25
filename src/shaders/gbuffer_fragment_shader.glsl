#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
flat in int hasTexture;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 objectColor;
uniform float specularIntensity;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);

    if (hasTexture == 1)
    {
        // and the diffuse per-fragment color
        gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
        // store specular intensity in gAlbedoSpec's alpha component
        gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
    }
    else
    {
        // if the object doesn't have a texture, use its color instead
        gAlbedoSpec.rgb = objectColor;
        gAlbedoSpec.a = specularIntensity;
    }
}

