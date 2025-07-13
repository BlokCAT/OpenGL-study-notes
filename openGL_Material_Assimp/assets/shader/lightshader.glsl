#version 460 core
out vec4 FragColor;

uniform vec3 lookslikeColor;

void main()
{
    FragColor = vec4( lookslikeColor , 0);
}