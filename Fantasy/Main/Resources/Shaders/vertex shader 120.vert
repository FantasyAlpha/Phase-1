#version 120

//NOTE(kai): attribute takes values into the vertex shader (same as in)
attribute vec3 position;	
attribute vec2 inputTexCoords;	
attribute vec4 color;
attribute float slot;
attribute vec3 normal;
//NOTE(kai): varying takes values out of the vertex shader (same as out) and into the fragment shader (same as out)
varying vec2 outputTexCoords;
varying vec4 myColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * modelMatrix * vec4(position, 1.0f);
	outputTexCoords = inputTexCoords;
	myColor = color;
}