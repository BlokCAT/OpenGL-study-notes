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

//每一个片段的着色方式！所有属性都已差值
void main()
{

   
    vec3 Normal = normalize(N); //取出法线
    vec3 lightDir = normalize(lightPos - ShaderPos);//取出光线来的方向


    //着色
    float ambientStrength = 0.1;//环境色
    vec3 ambient = ambientStrength * lightColor;
    float diff = max(dot(Normal, lightDir), 0.0f); //漫反射光照
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = 0.7;
    int glossy = 256;
    vec3 viewDirection = normalize(viewPos - ShaderPos);
    vec3 halfDirection = normalize(lightDir + viewDirection);
    vec3 specular = specularStrength * lightColor * pow(max( dot(Normal, halfDirection), 0.0 ), glossy);

    vec3 result = (ambient  + specular + diffuse ) * texture(sampler, UV).rgb;
    FragColor = vec4(result, 1.0);
}