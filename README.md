# OpenGL学习

## 介绍
仿真入门作业，learnopengl官网教程按章节写的项目

## 软件架构
vscode项目+cmake配置实现openGL工程
支持win-x64和linux-x64


## 安装教程
### win环境：
只需要安装MinGW-64bit的g++并配置好环境变量即可
### linux环境：
安装好openGL，GLFW，glu,glut,assimp依赖库,glfw需要make编译，其依赖于Xinerama，Xcursor，X11，Xrandr，Xi

#### 使用说明
将需要编译的cpp文件路径添加到Cmake的“主文件源代码”处，根据Cmake文件configure然后build即可，可以通过vscode插件直接运行

## Review
----basic：入门部分

----light：光照

    basic_light:普通光照(含材质和光照贴图)
    parallel_light:平行光
    point_light：点光源
    flash_light：手电筒
    multi_light：多光源

----model：模型加载

----Advanced：高级opengl

    Advanced_GLSL：高级GLSL
    blend：混合
    Cubemaps：立方贴图
    Depth_Testing：深度测试
    Face_culling：面剔除
    frameBuffer：帧缓冲
    Geometry_Shader：几何着色器
    Instancing：实例化
    stencil_testing：模板测试

----Advanced_lighting：高级光照

    Blinn-Phong：Blinn-Phong光照
    Bloom：泛光
    Deferred_Shading：延迟着色
    HDR：高动态光照渲染
    Normal_Mapping：法线贴图
    Parallax_Mapping：深度贴图
    Point_Shadows：点阴影
    Shadow_Mapping：阴影贴图



