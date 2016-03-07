#version 120

//NOTE(kai): attribute takes values into the vertex shader (same as in)
attribute vec3 position;	
attribute vec2 inputTexCoords;	

//NOTE(kai): varying takes values out of the vertex shader (same as out) and into the fragment shader (same as out)
varying vec2 outputTexCoords;

uniform mat4 modelMatrix;

void main()
{
	gl_Position = modelMatrix * vec4(position, 1.0f);
	outputTexCoords = inputTexCoords;
}