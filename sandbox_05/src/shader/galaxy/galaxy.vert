#version 460 core
in vec3 a_position;
in vec3 a_color;
in vec3 a_randomness;
in float a_scales;

uniform float uTime;
uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
out vec3 a_color_out;

void main(void)
{
	vec4 modelPosition = u_modelMatrix * vec4(a_position,1.0);
	float angle = atan(modelPosition.x, modelPosition.z);
	float distanceToCenter = length(modelPosition.xz);
	float angleOffset = (1.0/distanceToCenter)*uTime*0.2;
	angle += angleOffset;
	modelPosition.x = cos(angle) * distanceToCenter;
	modelPosition.z = sin(angle) * distanceToCenter;

	modelPosition += vec4(a_randomness,1.0);

	vec4 viewPosition = u_viewMatrix * modelPosition;
	vec4 projectionPosition = u_projectionMatrix * viewPosition;

	gl_Position = projectionPosition;

	gl_PointSize  = 1.0f*a_scales;
	gl_PointSize *= (1.0f/-viewPosition.z);
	a_color_out = a_color;

};
