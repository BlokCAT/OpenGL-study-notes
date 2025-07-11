#version 460 core

struct Material {

    sampler2D diffuse;//��������ɫ ,��������ͼƬ����ɫ
    vec3 specular; //�߹���ɫ
    float shininess; //����߹��ɢ��뾶
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};

#define NR_POINT_LIGHTS 3
uniform PointLight pointLights[NR_POINT_LIGHTS];


struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

out vec4 FragColor;
in vec3 N;
in vec3 ShaderPos;
in vec2 UV;
//uniform float cnt;
uniform sampler2D sampler;
//uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;



//����ƽ�й�Ĺ�����ɫ
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // �ϲ����
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, UV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, UV));
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

//������Դ������ɫ
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // ��������ɫ
    float diff = max(dot(normal, lightDir), 0.0);
    // �������ɫ
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // ˥��
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));
    // �ϲ����
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, UV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, UV));
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}



// ÿһ��Ƭ�ε���ɫ��ʽ���������Զ��Ѳ�ֵ
void main()
{

  
    vec3 norm = normalize(N);
    vec3 viewDir = normalize(viewPos - ShaderPos);

    //̫����
    vec3 res = CalcDirLight(dirLight, norm, viewDir);

    //���Դ
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        res += CalcPointLight(pointLights[i], norm, ShaderPos, viewDir);
    }
  
    FragColor = vec4(res, 1);
}