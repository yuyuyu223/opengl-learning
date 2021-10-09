// 引入IMGUI
#include "ImGUI/imgui.h"
#include "ImGUI/backends/imgui_impl_glfw.h"
#include "ImGUI/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <string>
// 引入glad
#include <glad/glad.h>
// 引入glfw
#include <GLFW/glfw3.h>
#include "imgui_internal.h"

class RenderGUI
{
private:
    GLFWwindow *window;
    // 是否显示demo窗体
    bool show_demo_window = true;
    // 是否显示another窗体
    bool show_another_window = false;
    // 是否显示hello_world窗体
    bool show_hello_world_window = true;
    // 调色盘的默认颜色
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
public:
    RenderGUI(int width, int height, std::string title);
    int initGLFW(int width, int height, std::string title);
    void initImgUI(std::string style="dark");
    int RenderLoop();
    void frame_fresh();
};

/* 刷新帧 */
void RenderGUI::frame_fresh()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


int RenderGUI::initGLFW(int width, int height, std::string title)
{
    /* GLFW相关配置 */
    //初始化GLFW
    glfwInit();
    //设置OpenGL版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //使用OpenGL-core模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //创建窗体指针，窗体大小1280*720，标题Dear ImGui GLFW+OpenGL3 example
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    //若创建窗体失败
    if (window == NULL)
    {
        //打印错误信息
        std::cout << "Failed to create GLFW window" << std::endl;
        //结束GLFW
        glfwTerminate();
        return -1;
    }
    // glfw窗体注册进程上下文，设置为当前进程
    glfwMakeContextCurrent(window);
    /* GLAD相关配置 */
    //传入GLFW的进程地址来初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //初始化失败报错
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    return 1;
}

void RenderGUI::initImgUI(std::string style)
{
    /* ImGui相关配置 */
    // 检查imgui版本和其他信息（变量自己长度等等）
    IMGUI_CHECKVERSION();
    // imgui注册到当前进程
    ImGui::CreateContext();
    // 设置风格
    if(style=="dark")
    {
        ImGui::StyleColorsDark();
    }
    else ImGui::StyleColorsClassic();
    // 设置渲染环境openGL+GLFW+ImGui
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    // 设置glsl版本号，具体多少好像不太影响
    const char *glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

int RenderGUI::RenderLoop()
{
    // 渲染主循环
    while (!glfwWindowShouldClose(window))
    {
        //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，
        //并调用对应的回调函数（可以通过回调方法手动设置）
        glfwPollEvents();

        // 播放新的一帧
        frame_fresh();

        // 显示demo窗体
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 显示hello_world窗体
        if (show_hello_world_window)
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!", (bool *)NULL, ImGuiWindowFlags_MenuBar);

            ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always); // Create a window called "Hello, world!" and append into it.
            // ImGui::TextColored
            ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 显示another窗体
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // 渲染
        ImGui::Render();
        // 获取帧的宽高
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        // 设置渲染范围0-w，0-h
        glViewport(0, 0, display_w, display_h);
        // 设置清除屏幕后的默认填充颜色（ui底层的颜色）
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        // 清除屏幕（颜色位恢复默认）
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // 双缓冲区交换（前缓冲区展示内容，后缓冲区绘制）
        glfwSwapBuffers(window);
    }

    // 清除内存
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

RenderGUI::RenderGUI(int width, int height, std::string title)
{
    // 初始化GLFW窗体
    initGLFW(width, height, title);
    // 初始化ImgUI,设置暗色主题
    initImgUI("dark");
    // 渲染循环
    RenderLoop();
}

int main()
{
    RenderGUI *renderGUI = new RenderGUI(1920,1080,"于大哥openGL光栅渲染器");
}

