#version 120

//NOTE(kai): attribute takes values into the vertex shader (same as in)
attribute vec3 position;	
attribute vec2 inputTexCoords;	
attribute vec3 normals;	

//NOTE(kai): varying takes values out of the vertex shader (same as out) and into the fragment shader (same as out)
varying vec2 outputTexCoords;
varying vec3 outputNormals;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);
	outputTexCoords = inputTexCoords;
	outputNormals = normals;
}