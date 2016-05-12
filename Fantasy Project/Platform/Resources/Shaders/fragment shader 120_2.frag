#version 120

varying vec4 outpos;
varying vec4 myColor;
uniform vec2 mousePos;

void main()
{
	float intensity = (1.0f / length(outpos.xy - vec2(mousePos.x, mousePos.y))) * 50.0f;

	gl_FragColor =  myColor;
}