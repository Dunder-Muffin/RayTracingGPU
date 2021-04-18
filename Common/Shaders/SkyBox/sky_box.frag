#version 440
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube cubebmap;

void main()
{    
    FragColor = textureLod(cubebmap, TexCoords, 0);
}