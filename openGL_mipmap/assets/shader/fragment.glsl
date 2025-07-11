#version 460 core
out vec4 FragColor;
in vec3 color;
in vec2 outUv; //������ÿһ��������Ҫ��ͼƬ�ϲ�����uv���꣬�Ѿ���ֵ����˵�
uniform float cnt;
uniform sampler2D sampler;

uniform float w;
uniform float h;


void main()
{
   //FragColor = vec4(  color , 1.0f);   ��ɫ��������

   //ʹ�������������
	//FragColor = texture(sampler, outUv);


	//�Լ�����ʹ����һ�����mipmap
	vec2 location = outUv * vec2(w , h);
	vec2 dx = dFdx(location), dy = dFdy( location);
	float maxDelta = sqrt( max(dot(dx , dx) , dot(dy , dy)));
	float L = log2(maxDelta);

	int level = max( int(L + 0.5) , 0 );
	FragColor = textureLod( sampler , outUv , level);
}