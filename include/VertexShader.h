/*
 * @Author: HowsenFisher
 * @Date: 2021-07-25 15:08:33
 * @LastEditors: HowsenFisher
 * @LastEditTime: 2021-07-25 20:22:36
 * @Description: the head file of vertexShader
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VertexShader{

private:
    //顶点着色器动态编译源码
    char *source;
    unsigned int id;
public:
    //构造函数
    VertexShader(char *source);
    //编译结果检查函数
    void VertexShader_complie_check(unsigned int vertexShader);
    //获取id
    unsigned int getId();

};

/**
 * @param {char} *source：顶点着色器自定义代码
 * @return {*}
 */
VertexShader::VertexShader(char *source)
{
    std::cout << "创建着色器" << std::endl;
    this->source = source;
    //创建一个顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //把这个着色器源码附加到着色器对象
    glShaderSource(vertexShader, 1, &source, NULL);
    //编译着色器
    glCompileShader(vertexShader);
    //检查编译结果
    VertexShader_complie_check(vertexShader);
}

/**
 * @param {unsigned int} vertexShader：顶点着色器号
 * @return {*}
 */
void VertexShader::VertexShader_complie_check(unsigned int vertexShader)
{
    //存储成功返回值
    int success;
    //存储日志信息
    char infoLog[512];
    //判定是否编译成功
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    //若不成功打印报错信息
    if (!success)
    {
        //获取报错信息
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        //打印报错信息
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
}

unsigned int VertexShader::getId()
{
    return this->id;
}
