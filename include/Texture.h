/*
 * @Author: HowsenFisher
 * @Date: 2021-07-27 13:37:41
 * @LastEditors: HowsenFisher
 * @LastEditTime: 2021-07-30 16:37:12
 * @Description: the head file of texture
 */

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

class Texture
{
private:
    unsigned int texture;
    bool isCubeTexture;
    int texture_unit_id;

public:
    Texture(const char *path, int texture_unit_id, bool IsCube, bool gammaCorrection);
    unsigned int getId();
    void change_texture_unit(int texture_unit_id);
    void bind();
};

Texture::Texture(const char *path, int texture_unit_id, bool IsCube = false, bool gammaCorrection = false)
{
    this->isCubeTexture = IsCube;
    this->texture_unit_id = texture_unit_id;
    if (!IsCube)
    {
        glGenTextures(1, &texture);
        glActiveTexture(texture_unit_id);
        // stbi_set_flip_vertically_on_load(true);
        // 加载并生成纹理
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format,pformat;
            if (nrChannels == 1)
            {
                format = GL_RED;
                pformat = GL_RGB;
            }
            else if (nrChannels == 3)
            {
                format = gammaCorrection ? GL_SRGB : GL_RGB;
                pformat = GL_RGB;
            }
            else if (nrChannels == 4)
            {
                format = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
                pformat = GL_RGBA;
            }

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, pformat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            // 为当前绑定的纹理对象设置环绕、过滤方式
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            //如果带有alpha则改变纹理环绕方式
            // if (format = GL_RGBA)
            // {
            //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            // }
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
    else
    {
        std::vector<std::string> faces{
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"};

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            std::string imgpath = std::string(path) +"/"+faces[i];
            unsigned char *data = stbi_load(imgpath.c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
}

unsigned int Texture::getId()
{
    return this->texture;
}

void Texture::change_texture_unit(int texture_unit_id)
{
    glActiveTexture(texture_unit_id);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::bind()
{
    glActiveTexture(this->texture_unit_id);
    if(!this->isCubeTexture)
        glBindTexture(GL_TEXTURE_2D, texture);
    else
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}
