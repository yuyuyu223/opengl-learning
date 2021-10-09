/*
 * @Author: HowsenFisher
 * @Date: 2021-08-14 15:27:32
 * @LastEditors: HowsenFisher
 * @LastEditTime: 2021-08-14 18:29:35
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

    //开启opengl的深度测试开关(这样才有遮挡)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    ///////////////////////////////////////////////////////////////////////////////////

    /* 着色器配置 */
    ///////////////////////////////////////////////////////////////////////////////////
    Shader *shader = new Shader("../review/stencil_testing/2.stencil_testing.vs", "../review/stencil_testing/2.stencil_testing.fs");
    Shader *shaderSingleColor = new Shader("../review/stencil_testing/2.stencil_testing.vs", "../review/stencil_testing/2.stencil_single_color.fs");

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

    unsigned int cubeVBO, cubeVAO, planeVBO, planeVAO;

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    // 位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    // 纹理属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);


    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    // 位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    // 纹理属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);
    //////////////////////////////////////////////////////////////////////////////////

    /* 纹理部分 */
    ///////////////////////////////////////////////////////////////////////////////////
    Texture *cubeTexture = new Texture("../texture/marble.jpg", GL_TEXTURE0);
    Texture *floorTexture = new Texture("../texture/metal.png", GL_TEXTURE0);

    ///////////////////////////////////////////////////////////////////////////////////

    /* 渲染部分 */
    ///////////////////////////////////////////////////////////////////////////////////

    //激活链接后的着色器程序
    shader->use();
    //设置不同纹理
    shader->setInt("texture1", 0);

    
    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //检测键盘输入
        processInput(window, camera);
        //清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清除颜色缓冲、深度缓冲和模板缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        shaderSingleColor->use();
        //初始化三大矩阵(模型矩阵，视野矩阵，裁剪矩阵)
        glm::mat4 model, view, projection;
        //裁剪矩阵：视角45°，宽高比和窗口相同，远近平面0.1-100
        projection = glm::perspective(glm::radians(camera->get_fov()), screenWidth / screenHeight, 0.1f, 100.0f);
        //裁剪矩阵传入着色器
        shaderSingleColor->setMat4("projection",projection);
        //视野矩阵
        view = camera->get_view_matrix();
        //视野矩阵传入着色器
        shaderSingleColor->setMat4("view",view);

        shader->use();
        shader->setMat4("projection",projection);
        shader->setMat4("view",view);

        // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
        glStencilMask(0x00);

        // floor
        glBindVertexArray(planeVAO);
        floorTexture->bind();
        shader->setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        // cubes
        glBindVertexArray(cubeVAO);
        // glActiveTexture(GL_TEXTURE0);
        cubeTexture->bind();
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        shaderSingleColor->use();
        float scale = 1.1;
        // cubes
        glBindVertexArray(cubeVAO);
        cubeTexture->bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        shaderSingleColor->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        shaderSingleColor->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);

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

    glfwTerminate();
    return 0;
}
