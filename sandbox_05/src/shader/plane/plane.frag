#version 460 core
in vec4 a_color_out;
out vec4 FragColor;

uniform vec3 uDepthColor;
uniform vec3 uSurfaceColor;
uniform float uColorOffset;
uniform float uColorMultiplier;

varying float vElevation;

void main(void)
{
	float mixStrength = (vElevation + uColorOffset) * uColorMultiplier;
	vec3 color = mix(uDepthColor, uSurfaceColor, mixStrength );
	FragColor = vec4(color,1.0);

}
