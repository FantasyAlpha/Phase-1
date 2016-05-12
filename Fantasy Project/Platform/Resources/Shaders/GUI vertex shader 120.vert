#version 120

//NOTE(kai): attribute takes values into the vertex shader (same as in)
attribute vec2 position;	
attribute vec2 inputTexCoords;
attribute vec4 Color;

//NOTE(kai): varying takes values out of the vertex shader (same as out) and into the fragment shader (same as out)
varying vec2 outputTexCoords;
varying vec4 vertOutColor;

uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * vec4(position.xy, 0.0f, 1.0f);
	outputTexCoords = inputTexCoords;
	vertOutColor = Color;
}