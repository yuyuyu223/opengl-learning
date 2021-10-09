IF (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	MESSAGE(STATUS "current platform: Linux ")
    set(PLATFORM Linux)
ELSEIF (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	MESSAGE(STATUS "current platform: Windows")
    set(PLATFORM "Windows")
ELSEIF (CMAKE_HOST_SYSTEM_NAME MATCHES "FreeBSD")
	MESSAGE(STATUS "current platform: FreeBSD")
    set(PLATFORM "FreeBSD")
ELSE ()
	MESSAGE(STATUS "other platform: ${CMAKE_HOST_SYSTEM_NAME}")
    set(PLATFORM "${CMAKE_HOST_SYSTEM_NAME}")
ENDIF (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")

MESSAGE("###################################")

IF(PLATFORM MATCHES "Linux")
    cmake_minimum_required(VERSION 2.8)
    MESSAGE(STATUS "进行linux配置")
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 11)
    project(OpenGl_homework)
    MESSAGE(STATUS "进行依赖库检查......")
    if (POLICY CMP0072)
        set(OpenGL_GL_PREFERENCE LEGACY)
    endif()
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(GLUT REQUIRED)
    find_package(X11 REQUIRED)
    file(GLOB project_main_file ./review/light/basic_light/main.cpp glad.c)
    # set common source
    file(GLOB GUI_SRC
        ./include/ImGUI/*.h
        ./include/ImGUI/*.cpp
        ./include/ImGUI/examples/libs/gl3w/GL/gl3w.*
        ./include/ImGUI/backends/imgui_impl_glfw.*
        ./include/ImGUI/backends/imgui_impl_opengl3.*
    )
    add_executable(${PROJECT_NAME} ${project_main_file} ${GUI_SRC})
    # 添加头文件路径
    include_directories(
        ./include
        ./include/ImGUI/examples/libs/gl3w # for GL/gl3w.h
        ./include/ImGUI/backends
        ./include/ImGUI
    )
    target_link_libraries(${PROJECT_NAME}
        -lglfw3
        -lGL
        -lGLU
        -lm
        -lXrandr
        -lXi
        -lX11
        # -lXxf86vm
        -lpthread
        -ldl
        -lXinerama
        -lXcursor
        -lassimp
        )
ELSEIF(PLATFORM MATCHES "Windows")
    MESSAGE(STATUS "进行windows配置")
    # cmake最低版本号要求
    cmake_minimum_required (VERSION 3.1)
    # 设置PROJECT_NAME变量
    set(PROJECT_NAME OpenGL_Homework)
    # 设置工程名
    project (${PROJECT_NAME})

    # 递归列出所有静态库
    file (GLOB_RECURSE STATIC_LIB ./lib/*.a)
    # ImGui所需的源代码
    file(GLOB GUI_SRC
        ./include/ImGUI/*.h
        ./include/ImGUI/*.cpp
        ./include/ImGUI/examples/libs/gl3w/GL/gl3w.*
        ./include/ImGUI/backends/imgui_impl_glfw.*
        ./include/ImGUI/backends/imgui_impl_opengl3.*
        ./GUI/include/*
        ./GUI/src/*
    )
    # 主文件源代码
    file(GLOB MAIN_SRC
        ./review/Advanced_lighting/Deferred_Shading/main.cpp
    )
    # 添加头文件路径
    include_directories(
        ./include
        ./include/ImGUI/examples/libs/gl3w # for GL/gl3w.h
        ./include/ImGUI/backends
        ./include/ImGUI
        ./GUI/include
    )
    # 添加静态链接库
    link_libraries(${STATIC_LIB})
    # 添加一个可编译的目标到工程
    add_executable (${PROJECT_NAME} ${MAIN_SRC} ${GUI_SRC})

    file(COPY ./lib/libassimpd.dll DESTINATION ./ FOLLOW_SYMLINK_CHAIN)
    file(COPY ./lib/glfw3.dll DESTINATION ./ FOLLOW_SYMLINK_CHAIN)
    file(COPY ./texture DESTINATION ./ FOLLOW_SYMLINK_CHAIN)
    file(COPY ./shaders DESTINATION ./ FOLLOW_SYMLINK_CHAIN)
ENDIF(PLATFORM MATCHES "Linux")