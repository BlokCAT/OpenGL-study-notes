#version 460 core
out vec4 FragColor;
in vec3 color;
in vec2 outUv; //这里是每一个像素需要在图片上采样的uv坐标，已经差值完毕了的
uniform float cnt;
uniform sampler2D sampler;

uniform float w;
uniform float h;


void main()
{
   //FragColor = vec4(  color , 1.0f);   橙色的三角形

   //使用纹理的三角形
	//FragColor = texture(sampler, outUv);


	//自己决定使用哪一级别的mipmap
	vec2 location = outUv * vec2(w , h);
	vec2 dx = dFdx(location), dy = dFdy( location);
	float maxDelta = sqrt( max(dot(dx , dx) , dot(dy , dy)));
	float L = log2(maxDelta);

	int level = max( int(L + 0.5) , 0 );
	FragColor = textureLod( sampler , outUv , level);
}