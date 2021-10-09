#version 330 core
out vec4 FragColor; //输出：片段颜色

//材质
struct Material {
    sampler2D diffuse; //漫反射
    sampler2D specular;//镜面反射
    float shininess;   //反光度
}; 

//平行光
struct DirLight {
    vec3 direction; //平行光方向

    vec3 ambient; //平行光的环境光权重，一般来说向量三个维度的数字相同
    vec3 diffuse; //平行光的漫反射光权重
    vec3 specular; //平行光的镜面反射光权重
};

//点光源
struct PointLight {
    vec3 position; //点光源位置
    // 衰减的三个常数
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 聚光（手电筒）
struct SpotLight {
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 添加衰减公式种的三个常数
    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 4 //定义4个点光源

in vec2 TexCoords; //输入：纹理
in vec3 Normal; //输入：法向量
in vec3 FragPos; //输入：片段位置

uniform vec3 viewPos;  //变量：相机的位置
uniform DirLight dirLight; //变量：平行光属性
uniform PointLight pointLights[NR_POINT_LIGHTS]; //变量：点光源属性数组
uniform SpotLight spotLight; //变量：聚光属性
uniform Material material; //变量：材质属性

// 计算光分量的函数
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // 法向量标准化
    vec3 norm = normalize(Normal);
    // 片段-->相机的向量
    vec3 viewDir = normalize(viewPos - FragPos);
    // 计算平行光分量
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // 计算点光源分量
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // 计算聚光分量
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    // 输出片段颜色
    FragColor = vec4(result, 1.0);
}

// 计算平行光分量（太阳光，几乎无衰减)
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // 光方向向量标准化
    vec3 lightDir = normalize(-light.direction);
    // 计算法线与光方向的夹角余弦值
    float diff = max(dot(normal, lightDir), 0.0);
    // 计算反射方向
    vec3 reflectDir = reflect(-lightDir, normal);
    // 计算反射方向和观察方向的夹角余弦，与反射光强做指数，计算出一个光强的相对值
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    //环境光是纹理颜色×光的环境分量权重
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //漫反射光是纹理颜色×光的漫反射权重
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //镜面反射光是纹理颜色×光的镜面反射权重
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

// 计算点光源分量
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 计算片段-->光源的方向
    vec3 lightDir = normalize(light.position - fragPos);
    // 计算片段-->光源的方向与法线夹角的余弦值
    float diff = max(dot(normal, lightDir), 0.0);
    // 计算反射光方向
    vec3 reflectDir = reflect(-lightDir, normal);
    // 计算反射光强
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 计算光源到片段的距离
    float distance = length(light.position - fragPos);
    // 计算衰减
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    //环境光是纹理颜色×光的环境分量权重
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //漫反射光是纹理颜色×光的漫反射权重
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //镜面反射光是纹理颜色×光的镜面反射权重
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    // 衰减
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// 计算聚光分量
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 计算片段-->光源的方向
    vec3 lightDir = normalize(light.position - fragPos);
    // 计算片段-->光源的方向与法线夹角的余弦值
    float diff = max(dot(normal, lightDir), 0.0);
    // 计算反射光方向
    vec3 reflectDir = reflect(-lightDir, normal);
    // 计算反射光强
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 计算光源到片段的距离
    float distance = length(light.position - fragPos);
    // 计算衰减
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // 边缘平滑相关参数
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //环境光是纹理颜色×光的环境分量权重
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //漫反射光是纹理颜色×光的漫反射权重
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //镜面反射光是纹理颜色×光的镜面反射权重
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    // 衰减、平滑处理
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
