#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;//ÿһ�������uv����

out vec3 color;
out vec2 outUv; //����ȥʱ��openGL���Զ������Բ�ֵ
uniform float time;

void main()
{
   float scale = 1.0/time;
   gl_Position = vec4(aPos * scale ,  1.0);
   // color = aColor * (sin(time) + 1.0f)/2.0f;
   color = aColor;
   outUv = aUV;
}