#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform bool Refract = false;

void main()
{
    float ratio = 1.00 / 1.52;            
    vec3 I = normalize(Position - cameraPos);
    vec3 R;
    if(Refract)
    {
        R = refract(I, normalize(Normal), ratio);
    }
    else R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}