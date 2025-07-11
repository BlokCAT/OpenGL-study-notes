#version 460 core

struct Material {

    sampler2D diffuse;//漫反射颜色 ,就是纹理图片的颜色
    vec3 specular; //高光颜色
    float shininess; //镜面高光的散射半径
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



//定义平行光的光照着色
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, UV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, UV));
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

//计算点光源的照着色
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));
    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, UV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, UV));
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}



// 每一个片段的着色方式！所有属性都已差值
void main()
{

  
    vec3 norm = normalize(N);
    vec3 viewDir = normalize(viewPos - ShaderPos);

    //太阳光
    vec3 res = CalcDirLight(dirLight, norm, viewDir);

    //点光源
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        res += CalcPointLight(pointLights[i], norm, ShaderPos, viewDir);
    }
  
    FragColor = vec4(res, 1);
}