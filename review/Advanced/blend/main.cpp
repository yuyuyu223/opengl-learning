/*
 * @Author: HowsenFisher
 * @Date: 2021-08-14 15:27:32
 * @LastEditors: HowsenFisher
 * @LastEditTime: 2021-08-14 19:29:37
 * @Description: 
 */
/*
 * @Author: HowsenFisher
 * @Date: 2021-07-24 19:10:52
 * @LastEditors: HowsenFisher
 * @LastEditTime: 2021-07-29 19:10:03
 * @Description: the main func to create an window
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
// #include <FragmentShader.h>
// #include <VertexShader.h>
#include <Shader.h>
#include <cmath>
#include <Texture.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera.h>
#include <vector>
#include <algorithm>
#include <map>

bool distance_sort_cmp(glm::vec3 &a, glm::vec3 &b);

float screenWidth = 800;
float screenHeight = 600;

//初始化相机
Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f),  //相机位置
                            glm::vec3(0.0f, 0.0f, 0.0f),  //观察位置
                            glm::vec3(0.0f, 1.0f, 0.0f)); //上向量

#include <EventListener.h>

int main()
{
    /* GLFW配置 */
    //////////////////////////////////////////////////////////////////////////////
    //初始化GLFW
    glfwInit();
    //设置OpenGL版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //使用OpenGL-core模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //创建窗体指针，窗体大小800*600，标题LearnOpenGL
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    //若创建窗体失败
    if (window == NULL)
    {
        //打印错误信息
        std::cout << "Failed to create GLFW window" << std::endl;
        //结束GLFW
        glfwTerminate();
        return -1;
    }
    //通知GLFW将我们窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    //隐藏光标，并捕捉(Capture)它
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //设置回调函数，触发器为窗体
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //绑定鼠标回调
    glfwSetCursorPosCallback(window, mouse_callback);
    //绑定滚动事件
    glfwSetScrollCallback(window, scroll_callback);

    //传入GLFW的进程地址来初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //初始化失败报错
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ///////////////////////////////////////////////////////////////////////////////////

    /* 着色器配置 */
    ///////////////////////////////////////////////////////////////////////////////////
    Shader *shader = new Shader("../review/Advanced/blend/3.1.blending.vs", "../review/Advanced/blend/3.1.blending.fs");

    ///////////////////////////////////////////////////////////////////////////////////

    /* 顶点和缓冲配置 */
    //////////////////////////////////////////////////////////////////////////////////
    //顶点信息
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
        5.0f, -0.5f, -5.0f, 2.0f, 2.0f};

    float grassVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.5f, 0.0f, 1.0f, 0.0f};

    unsigned int cubeVBO, cubeVAO, planeVBO, planeVAO, grassVAO, grassVBO;
    /* 绑定cube数据 */
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 纹理属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* 绑定floor数据 */
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 纹理属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    /* 绑定grass数据 */
    glGenVertexArrays(1,&grassVAO);
    glGenBuffers(1,&grassVBO);
    glBindVertexArray(grassVAO);
    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 纹理属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 窗户位置
    std::vector<glm::vec3> win_pos;
    win_pos.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    win_pos.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    win_pos.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    win_pos.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    win_pos.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    //对窗户位置进行排序(算法排序或者map排序)
    //std::sort(win_pos.begin(), win_pos.end(),distance_sort_cmp);
    std::map<float,glm::vec3> win_map;
    for(int i=0;i<win_pos.size();i++)
    {
        glm::vec3 cameraPos = camera->get_camera_pos();
        float distance = glm::length(win_pos[i]-cameraPos);
        win_map[distance] = win_pos[i];
    }


    //////////////////////////////////////////////////////////////////////////////////

    /* 纹理部分 */
    ///////////////////////////////////////////////////////////////////////////////////
    Texture *cubeTexture = new Texture("../texture/marble.jpg", GL_TEXTURE0);
    Texture *floorTexture = new Texture("../texture/metal.png", GL_TEXTURE0);
    Texture *grassTexture = new Texture("../texture/blending_transparent_window.png", GL_TEXTURE0);

    ///////////////////////////////////////////////////////////////////////////////////

    /* 渲染部分 */
    ///////////////////////////////////////////////////////////////////////////////////

    // 激活链接后的着色器程序
    shader->use();
    // 设置不同纹理
    shader->setInt("texture1", 0);
    // shader->setInt("texture2", 1);
    // 开启opengl的深度测试开关(这样才有遮挡)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // 开启混合模式
    glEnable(GL_BLEND);
    // 设置混合函数
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 初始化三大矩阵(模型矩阵，视野矩阵，裁剪矩阵)
    glm::mat4 model, view, projection;
    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //检测键盘输入
        processInput(window, camera);
        //清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清除颜色缓冲和深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //裁剪矩阵：视角45°，宽高比和窗口相同，远近平面0.1-100
        projection = glm::perspective(glm::radians(camera->get_fov()), screenWidth / screenHeight, 0.1f, 100.0f);
        //裁剪矩阵传入着色器
        shader->setMat4("projection", projection);
        //视野矩阵
        view = camera->get_view_matrix();
        //视野矩阵传入着色器
        shader->setMat4("view", view);
        
        // 绘制cubes
        glBindVertexArray(cubeVAO);
        cubeTexture->bind();
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // 绘制floor
        glBindVertexArray(planeVAO);
        floorTexture->bind();
        shader->setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //绘制草
        glBindVertexArray(grassVAO);
        grassTexture->bind();
        for (std::map<float,glm::vec3>::reverse_iterator it = win_map.rbegin(); it != win_map.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            shader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        //交换缓冲
        glfwSwapBuffers(window);
        //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，
        //并调用对应的回调函数（可以通过回调方法手动设置）
        glfwPollEvents();
    }
    //////////////////////////////////////////////////////////////////////////////////////
    //清理所有的资源并正确地退出应用程序
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteProgram(shader->ID);
    glfwTerminate();
    return 0;
}


bool distance_sort_cmp(glm::vec3 &a, glm::vec3 &b)
{
    glm::vec3 cameraPos = camera->get_camera_pos();
    float len_a = glm::length(a-cameraPos);
    float len_b = glm::length(b-cameraPos);
    if(len_a > len_b)return true;
    else return false;
}