/*
 * @Author: HowsenFisher
 * @Date: 2021-08-01 16:15:32
 * @LastEditors: HowsenFisher
 * @LastEditTime: 2021-08-02 13:52:09
 * @Description: the head file of mesh
 */

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <glad/glad.h>
#include <Shader.h>
using namespace std;

struct Vertex
{
    // 位置
    glm::vec3 Position;
    // 法线
    glm::vec3 Normal;
    // 纹理
    glm::vec2 TexCoords;
    // 切线
    glm::vec3 Tangent;
    // 双切线
    glm::vec3 Bitangent;
};

struct Texture
{
    // 纹理id
    unsigned int id;
    // 纹理类型
    string type;
    // 纹理路径
    string path;
};

class Mesh
{
public:
    /*  网格数据  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    /*  函数  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader shader);
    unsigned int getVAO();

private:
    /*  渲染数据  */
    unsigned int VAO, VBO, EBO;
    /*  函数  */
    void setupMesh();
};

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::setupMesh()
{
    // 生成顶点数组
    glGenVertexArrays(1, &VAO);
    // 生成VBO缓冲
    glGenBuffers(1, &VBO);
    // 生成EBO缓冲
    glGenBuffers(1, &EBO);
    // 绑定VAO数组
    glBindVertexArray(VAO);
    // VAO数组绑定VBO缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // VBO缓冲存放数据为所有顶点信息
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  
    // VAO绑定EBO缓冲
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // EBO缓冲存放索引信息
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 顶点法线
    glEnableVertexAttribArray(1);   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);   
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // 切线
    glEnableVertexAttribArray(3);   
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // 双切线
    glEnableVertexAttribArray(4);   
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}  

void Mesh::Draw(Shader shader) 
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
        // 获取纹理序号（diffuse_textureN 中的 N）
        string number;
        string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        else if(name == "texture_normal")
            number = std::to_string(normalNr++);
        else if(name == "texture_height")
            number = std::to_string(heightNr++);

        shader.setFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

unsigned int Mesh::getVAO()
{
    return this->VAO;
}