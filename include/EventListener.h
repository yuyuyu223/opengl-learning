/*
 * @Author: HowsenFisher
 * @Date: 2021-07-29 14:45:26
 * @LastEditors: HowsenFisher
 * @LastEditTime: 2021-07-31 17:30:57
 * @Description: the head file of keyboard listener
 */

float lastX = 400, lastY = 300;
float yaw = -90.0f, pitch = 0;
bool firstMouse = true;

/**
 * 检测键盘输入
 * @param {GLFWwindow} *window：窗体指针
 * @return {*}
 */
void processInput(GLFWwindow *window, Camera *camera, float cameraSpeed = 0.005f)
{
    glm::vec3 cameraFront = camera->get_front_vec();
    glm::vec3 cameraUp = camera->get_up();
    glm::vec3 cameraPos = camera->get_camera_pos();
    // float cameraSpeed = 0.005f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraFront;
        camera->set_camera_pos(cameraPos);
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraFront;
        camera->set_camera_pos(cameraPos);
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        camera->set_camera_pos(cameraPos);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        camera->set_camera_pos(cameraPos);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera->reset();
        yaw = -90.0f;
        pitch = 0;
    }
}

/**
 * 窗口尺寸变化回调函数
 * @param {GLFWwindow} *window：窗体指针
 * @param {int} width：变化后的宽
 * @param {int} height：变化后的长
 * @return {*}
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/**
 * 鼠标事件回调函数
 * @param {GLFWwindow*} window
 * @param {double} xpos
 * @param {double} ypos
 * @return {*}
 */
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    // std::cout << yaw << " " << pitch << std::endl;
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    glm::vec3 cameraFront = glm::normalize(front);
    camera->set_front(cameraFront);
    camera->set_right(glm::normalize(glm::cross(cameraFront, camera->get_worldUp()))); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    camera->set_up(glm::normalize(glm::cross(camera->get_right_vec(), camera->get_front_vec())));
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    float fov = camera->get_fov();
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
    camera->set_fov(fov);
}