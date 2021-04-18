#version 440
layout(location = 0)in vec3 Position ;

out vec3 TexCoords;

uniform mat4 ViewProjection;

void main()
{
    TexCoords = Position;
    gl_Position = (ViewProjection * vec4(Position, 1.0)).xyww;
}  