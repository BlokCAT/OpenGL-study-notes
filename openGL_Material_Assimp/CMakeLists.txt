cmake_minimum_required(VERSION 3.12)
project(openGLstudy)

set(CMAKE_CXX_STANDARD 20)

# 向项目加入全局的预编译宏
add_definitions(-DDEBUG)

#拷贝所有的资产assets去可执行文件的目录
file(GLOB ASSETS "./assets")
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)  # 意思是需要找头文件时，直接去这里面找头文件 ， CMAKE_CURRENT_SOURCE_DIR是项目根目录的意思

 
 
 
link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
) # 之后链接的时候到这里面找静态链接库

add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(glFrameWork)

add_executable( openGLstudy "main.cpp" "glad.c" )# 加载exe
target_link_libraries(openGLstudy  glfw3.lib  wrapper  application  Fw  assimp-vc143-mtd.lib  poly2tri.lib ) # 加载exe的 时候需要静态链接库，所以这里就是链接 glfw3.lib


# 添加自定义命令和目标，用于复制DLL
add_custom_command(
    TARGET openGLstudy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/bin/assimp-vc143-mtd.dll"
        $<TARGET_FILE_DIR:openGLstudy>
    COMMENT "复制Assimp DLL到可执行文件目录"
)