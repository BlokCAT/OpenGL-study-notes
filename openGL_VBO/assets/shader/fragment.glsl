#version 460 core
out vec4 FragColor;
in vec3 color;
in vec2 outUv; //������ÿһ��������Ҫ��ͼƬ�ϲ�����uv���꣬�Ѿ���ֵ����˵�
uniform float cnt;
uniform sampler2D sampler;
uniform sampler2D sampler2;



void main()
{
   //FragColor = vec4(  color , 1.0f);   ��ɫ��������

   //ʹ�������������
	FragColor = mix(texture(sampler, outUv), texture(sampler2, outUv), cnt);
}