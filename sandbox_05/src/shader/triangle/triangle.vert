#version 460 core
in vec4 a_position;
in vec4 a_color;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec4 a_color_out;

void main(void)
{
	a_color_out = a_color;
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;


};


