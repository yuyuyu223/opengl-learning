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
    Shader *shader = new Shader("./shaders/vertex.dat", "./shaders/fragment.dat");

    ///////////////////////////////////////////////////////////////////////////////////

    /* 顶点和缓冲配置 */
    //////////////////////////////////////////////////////////////////////////////////
    //顶点信息
    float vertices[] = {
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

    unsigned int indices[] = {
        // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // 纹理属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //////////////////////////////////////////////////////////////////////////////////

    /* 纹理部分 */
    ///////////////////////////////////////////////////////////////////////////////////
    Texture *texture1 = new Texture("./texture/1.jpg", GL_TEXTURE0);
    Texture *texture2 = new Texture("./texture/2.jpg", GL_TEXTURE1);

    ///////////////////////////////////////////////////////////////////////////////////

    /* 渲染部分 */
    ///////////////////////////////////////////////////////////////////////////////////

    //激活链接后的着色器程序
    shader->use();
    //设置不同纹理
    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1);
    //设置光照
    shader->setVec3("lightColor",0.90f, 0.05f, 0.05f);
    //开启opengl的深度测试开关(这样才有遮挡)
    glEnable(GL_DEPTH_TEST);

    //初始化三大矩阵(模型矩阵，视野矩阵，裁剪矩阵)
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
        int projectionLoc = glGetUniformLocation(shader->ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //视野矩阵
        // float radius = 10.0f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;
        // camera->set_camera_pos(glm::vec3(camX, 0.0, camZ));
        view = camera->get_view_matrix();
        //视野矩阵传入着色器
        int viewLoc = glGetUniformLocation(shader->ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        for (unsigned int i = 0; i < 10; i++)
        {
            //模型配置————自转
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i + 1);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            int modelLoc = glGetUniformLocation(shader->ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            //视野配置————相机公转

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        // shader->use();
        // glBindVertexArray(VAO);
        //绘制元素（EBO用法），绘制6个顶点，第三个参数是索引的类型，这里是GL_UNSIGNED_INT
        //最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候），但是我们会在这里填写0。;
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //交换缓冲
        glfwSwapBuffers(window);
        //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，
        //并调用对应的回调函数（可以通过回调方法手动设置）
        glfwPollEvents();
    }
    //////////////////////////////////////////////////////////////////////////////////////
    //清理所有的资源并正确地退出应用程序
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader->ID);
    glfwTerminate();
    return 0;
}