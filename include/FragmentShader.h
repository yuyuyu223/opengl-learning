/*
 * @Author: HowsenFisher
 * @Date: 2021-07-25 15:28:38
 * @LastEditors: HowsenFisher
 * @LastEditTime: 2021-07-25 21:04:18
 * @Description: the header file of FragmentShader
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class FragmentShader{
private:
    //片段着色器动态编译源码
    char * source;
    unsigned int id;

public:
    //构造函数
    FragmentShader(char *source);
    //编译结果检查函数
    void FragmentShader_complie_check(unsigned int fragmentShader);
    //返回id
    unsigned int getId();

};


/**
 * @param {char} *source：片段着色器自定义代码
 * @return {*}
 */
FragmentShader::FragmentShader(char *source)
{
    this->source = source;
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &(this->source), NULL);
    glCompileShader(fragmentShader);
    this->id = fragmentShader;
}

/**
 * @param {unsigned int} fragmentShader：片段着色器号
 * @return {*}
 */
void FragmentShader::FragmentShader_complie_check(unsigned int fragmentShader)
{
    //存储成功返回值
    int success;
    //存储日志信息
    char infoLog[512];
    //判定是否编译成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    //若不成功打印报错信息
    if (!success)
    {
        //获取报错信息
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        //打印报错信息
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
}

unsigned int FragmentShader::getId()
{
    return this->id;
}