#version 460 core
in vec3 a_color_out;
out vec4 FragColor;
void main(void)
{
FragColor = vec4(a_color_out, 0.9);  // Slightly dimmer white with 50% transparency
}
