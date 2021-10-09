#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
// 灰度
uniform bool grey = false;
// 反相
uniform bool Inversion = false;
// 锐化
uniform bool sharpening = false;
// 模糊
uniform bool Blur = false;
// 边缘检测
uniform bool Edge_detection = false;

const float offset = 1.0 / 300.0; 

// 添加3x3核支持
vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // 左上
    vec2( 0.0f,    offset), // 正上
    vec2( offset,  offset), // 右上
    vec2(-offset,  0.0f),   // 左
    vec2( 0.0f,    0.0f),   // 中
    vec2( offset,  0.0f),   // 右
    vec2(-offset, -offset), // 左下
    vec2( 0.0f,   -offset), // 正下
    vec2( offset, -offset)  // 右下
);

void main()
{
    vec3 col;
    if(Inversion) col = 1-texture(screenTexture, TexCoords).rgb;
    
    else if(grey){
        col = texture(screenTexture, TexCoords).rgb;
        vec4 pFragColor = vec4(col, 1.0);
        float average = 0.2126 * pFragColor.r + 0.7152 * pFragColor.g + 0.0722 * pFragColor.b;
        col = vec3(average, average, average);
    }
    

    else if(sharpening){
        // 锐化核
        float kernel[9] = float[](
            -1, -1, -1,
            -1,  9, -1,
            -1, -1, -1
        );
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        }
        // vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
        {   
            col += sampleTex[i] * kernel[i];
        }
    }
    else if(Blur){
        // 模糊核
        float kernel[9] = float[](
            1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16  
        );
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        }
        // vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
        {   
            col += sampleTex[i] * kernel[i];
        }
    }
    else if(Edge_detection){
        // 模糊核
        float kernel[9] = float[](
            1.0, 1.0, 1.0,
            1.0,-8.0, 1.0,
            1.0, 1.0, 1.0 
        );
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        }
        // vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
        {   
            col += sampleTex[i] * kernel[i];
        }
    }
    else{
        col = texture(screenTexture, TexCoords).rgb;
    }

    FragColor = vec4(col, 1.0);

} 