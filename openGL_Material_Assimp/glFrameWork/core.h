#pragma once 
#include<glad/glad.h>
#include<GLFW/glfw3.h> 
#include <map>
#include <iostream>
#include <string>
#include <vector>


#include <assimp/Importer.hpp>       // 模型导入器
#include <assimp/scene.h>            // 模型场景数据（网格、材质、动画等）
#include <assimp/postprocess.h>      // 后处理选项（如三角化、优化等）

//GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"




