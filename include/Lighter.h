/*
 * @Author: HowsenFisher
 * @Date: 2021-07-29 18:13:44
 * @LastEditors: HowsenFisher
 * @LastEditTime: 2021-07-29 18:15:35
 * @Description: the head file of lighter
 */

class Lighter
{
private:
    glm::vec3 light_color;
public:
    Lighter(glm::vec3 light_color);
    void set_light_color(glm::vec light_color);

};

Lighter::Lighter(glm::vec3 light_color)
{
    this->light_color = light_color;
}

void Lighter::set_light_color(glm::vec3 light_color)
{
    this->light_color = light_color;
}

