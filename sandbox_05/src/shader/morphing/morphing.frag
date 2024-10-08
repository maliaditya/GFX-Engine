#version 460 core
in vec4 a_color_out;
out vec4 FragColor;

varying vec3 vColor;

void main(void)
{
vec2 uv = gl_PointCoord;
FragColor = vec4(vColor,1.0f);
}
