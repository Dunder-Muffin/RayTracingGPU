#version 440
layout(location = 0)in vec2 Position;

out vec2 fragUV;
void main()
{
    fragUV = Position;
    gl_Position = vec4(Position, 0, 1);
}