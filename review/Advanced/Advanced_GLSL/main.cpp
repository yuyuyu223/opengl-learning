#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
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

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    ///////////////////////////////////////////////////////////////////////////////////

    /* 着色器配置 */
    ///////////////////////////////////////////////////////////////////////////////////
    Shader *shaderRed = new Shader("../review/Advanced/Advanced_GLSL/8.advanced_glsl.vs", "../review/Advanced/Advanced_GLSL/8.red.fs");
    Shader *shaderGreen = new Shader("../review/Advanced/Advanced_GLSL/8.advanced_glsl.vs", "../review/Advanced/Advanced_GLSL/8.green.fs");
    Shader *shaderBlue = new Shader("../review/Advanced/Advanced_GLSL/8.advanced_glsl.vs", "../review/Advanced/Advanced_GLSL/8.blue.fs");
    Shader *shaderYellow = new Shader("../review/Advanced/Advanced_GLSL/8.advanced_glsl.vs", "../review/Advanced/Advanced_GLSL/8.yellow.fs");
    ///////////////////////////////////////////////////////////////////////////////////

    /* 顶点和缓冲配置 */
    //////////////////////////////////////////////////////////////////////////////////
    //顶点信息
    float cubeVertices[] = {
        // positions         
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

    /* cube数据绑定 */
    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // 设置Uniform缓冲
    // ---------------------------------
    // 将着色器中特定的uniform块绑定到指定绑定点上
    shaderRed->BindUniformPoint("Matrices",0);
    shaderGreen->BindUniformPoint("Matrices",0);
    shaderBlue->BindUniformPoint("Matrices",0);
    shaderYellow->BindUniformPoint("Matrices",0);

    // 创建uniform缓冲
    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);
    // uboMatrices作为UNIFORM_BUFFER
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    // 开辟2 * sizeof(glm::mat4)的空间
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    // 恢复默认
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // 定义uboMatrices与UNIFORM_BUFFER位置映射关系 uboMatrices的0——2 * sizeof(glm::mat4)写入UNIFORM_BUFFER的0位置
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    

    //////////////////////////////////////////////////////////////////////////////////
    /* 渲染部分 */
    ///////////////////////////////////////////////////////////////////////////////////
    //初始化三大矩阵(模型矩阵，视野矩阵，裁剪矩阵)
    glm::mat4 model, view, projection;
    // 计算裁剪矩阵
    projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    // 要对uboMatrices进行操作
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    // projection矩阵所在地址开始的sizeof(glm::mat4)长度（即矩阵本身数据），写入UNIFORM_BUFFER的0处
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    // 恢复默认
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //检测键盘输入
        processInput(window, camera);

        //清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //清除颜色缓冲和深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         // 获取观察矩阵
        view = camera->get_view_matrix();
        // 对uboMatrices进行操作
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        // view所在地址开始sizeof(glm::mat4)长度（view本身），写入UNIFORM_BUFFER的sizeof(glm::mat4)处
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        // 恢复默认
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // draw 4 cubes 
        // RED
        glBindVertexArray(cubeVAO);
        shaderRed->use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
        shaderRed->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // GREEN
        shaderGreen->use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
        shaderGreen->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // YELLOW
        shaderYellow->use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
        shaderYellow->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // BLUE
        shaderBlue->use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
        shaderBlue->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
    glfwTerminate();
    return 0;
}
