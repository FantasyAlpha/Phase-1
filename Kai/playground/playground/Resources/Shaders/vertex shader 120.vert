#version 120

//NOTE(kai): attribute takes values into the vertex shader (same as in)
attribute vec3 position;	
attribute vec4 inputColor;	
attribute vec2 inputTexCoords;	

//NOTE(kai): varying takes values out of the vertex shader (same as out) and into the fragment shader (same as out)
varying vec4 outputColor;
varying vec2 outputTexCoords;

void main()
{
	gl_Position = vec4(position, 1.0f);
	outputColor = inputColor;
	outputTexCoords = inputTexCoords;
}