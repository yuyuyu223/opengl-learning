#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

struct Material {
    // vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    // vec3 position; // 使用定向光就不再需要了
    vec3 direction; //光的方向向量

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
in vec2 TexCoords;
uniform Light light;
uniform Material material;

void main()
{
    // 环境光
    // vec3 ambient  = light.ambient * material.ambient;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse  = light.diffuse * (diff * material.diffuse);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));



    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular * (spec * material.specular);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));


    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}