#version 120

//NOTE(kai): attribute takes values into the vertex shader (same as in)
attribute vec3 position;	
attribute vec2 inputTexCoords;	
attribute vec4 color;
attribute float slotIndex;	

//NOTE(kai): varying takes values out of the vertex shader (same as out) and into the fragment shader (same as out)
varying vec2 outputTexCoords;
varying vec4 outpos;
varying float slot;
varying vec4 myColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0f);
	outputTexCoords = inputTexCoords;
	outpos = modelMatrix * vec4(position, 1.0f);
	slot = slotIndex;
	myColor = color;
}