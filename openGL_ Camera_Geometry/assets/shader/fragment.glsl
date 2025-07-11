#version 460 core
out vec4 FragColor;

in vec3 N;
in vec3 ShaderPos;
in vec2 UV;


//uniform float cnt;
uniform sampler2D sampler;

//uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

//ÿһ��Ƭ�ε���ɫ��ʽ���������Զ��Ѳ�ֵ
void main()
{

   
    vec3 Normal = normalize(N); //ȡ������
    vec3 lightDir = normalize(lightPos - ShaderPos);//ȡ���������ķ���


    //��ɫ
    float ambientStrength = 0.1;//����ɫ
    vec3 ambient = ambientStrength * lightColor;
    float diff = max(dot(Normal, lightDir), 0.0f); //���������
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = 0.7;
    int glossy = 256;
    vec3 viewDirection = normalize(viewPos - ShaderPos);
    vec3 halfDirection = normalize(lightDir + viewDirection);
    vec3 specular = specularStrength * lightColor * pow(max( dot(Normal, halfDirection), 0.0 ), glossy);

    vec3 result = (ambient  + specular + diffuse ) * texture(sampler, UV).rgb;
    FragColor = vec4(result, 1.0);
}