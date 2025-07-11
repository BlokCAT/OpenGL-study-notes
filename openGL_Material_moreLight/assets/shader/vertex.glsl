#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aN;
layout(location = 1) in vec2 aUv;

uniform mat4 transform;
uniform mat4 model;


out vec3 ShaderPos;
out vec3 N;
out vec2 UV;
void main()
{
  // float scale = 1.0/(time * 3);

   vec4 positionnew = vec4(aPos,  1.0);
   positionnew = transform * positionnew;
   gl_Position = vec4(positionnew);

   N = normalize(mat3(transpose(inverse(model))) * aN);

   ShaderPos = vec3(model * vec4(aPos , 1.0));
   UV = aUv;
}