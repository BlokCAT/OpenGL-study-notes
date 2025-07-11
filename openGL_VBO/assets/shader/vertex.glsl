#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;//每一个顶点的uv坐标

out vec3 color;
out vec2 outUv; //传过去时，openGL会自动的线性差值
//uniform float time;

void main()
{
   gl_Position = vec4(aPos ,  1.0);
   // color = aColor * (sin(time) + 1.0f)/2.0f;
   color = aColor;
   outUv = aUV;
}