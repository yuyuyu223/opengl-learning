/*
 * @Author: HowsenFisher
 * @Date: 2021-07-28 18:22:43
 * @LastEditors: HowsenFisher
 * @LastEditTime: 2021-08-14 17:44:20
 * @Description: the camera head file
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera
{
private:
    glm::mat4 view;
    glm::vec3 camera_pos;
    glm::vec3 view_pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 world_up;
    glm::vec3 right;
    float fov;
public:
    Camera(glm::vec3 camera_pos,glm::vec3 view_pos,glm::vec3 up);
    glm::mat4 get_view_matrix();
    glm::vec3 get_front_vec();
    glm::vec3 get_view_pos();
    glm::vec3 get_up();
    glm::vec3 get_worldUp();
    glm::vec3 get_right_vec();
    glm::vec3 get_camera_pos();
    float get_fov();
    void set_camera_pos(glm::vec3 camera_pos);
    void set_view_pos(glm::vec3 view_pos);
    void set_up(glm::vec3 up);
    void set_front(glm::vec3 front);
    void set_right(glm::vec3 right);
    void set_fov(float fov);
    void reset();

};

Camera::Camera(glm::vec3 camera_pos,glm::vec3 view_pos,glm::vec3 up)
{
    this->camera_pos = camera_pos;
    this->view_pos = view_pos;
    this->up = up;
    this->world_up = up;
    this->front = glm::normalize(view_pos-camera_pos);
    this->fov = 45.0f;
}

void Camera::reset()
{
    this->camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);  //相机位置
    this->view_pos = glm::vec3(0.0f, 0.0f, 0.0f);  //观察位置
    this->up = glm::vec3(0.0f, 1.0f, 0.0f); //上向量
    this->front = glm::vec3(0.0f, 0.0f, -1.0f); 
    this->fov = 45.0f;
    this->right = glm::normalize(glm::cross(this->front, this->up)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.

}

glm::vec3 Camera::get_up()
{
    return this->up;
}

glm::vec3 Camera::get_worldUp()
{
    return this->world_up;
}

glm::vec3 Camera::get_camera_pos()
{
    return this->camera_pos;
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(this->camera_pos,  //相机位置
                       this->camera_pos+this->front,  //观察位置
                       this->up); //上向量
}

void Camera::set_camera_pos(glm::vec3 camera_pos)
{
    this->camera_pos = camera_pos;
    // std::cout<<"相机位置:"<<"("<<camera_pos.x<<","<<camera_pos.y<<","<<camera_pos.z<<")"<<std::endl;
}

void Camera::set_view_pos(glm::vec3 view_pos)
{
    this->view_pos = view_pos;
}

void Camera::set_up(glm::vec3 up)
{
    this->up = up;
}

glm::vec3 Camera::get_front_vec()
{
    return this->front;
}

glm::vec3 Camera::get_view_pos()
{
    return this->view_pos;
}

void Camera::set_front(glm::vec3 front)
{
    this->front = front;
}

void Camera::set_right(glm::vec3 right)
{
    this->right = right;
}

glm::vec3 Camera::get_right_vec()
{
    return this->right;
}

float Camera::get_fov()
{
    return this->fov;
}

void Camera::set_fov(float fov)
{
    this->fov = fov;
}