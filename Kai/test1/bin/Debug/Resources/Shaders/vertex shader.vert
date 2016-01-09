#version 330

//NOTE(kai): location of the attributes
layout (location = 0) in vec3 position;	
layout (location = 1) in vec4 inputColor;		

//NOTE(kai): "out" takes values out of the vertex shader and into the fragment shader 
out vec4 outputColor;

void main()
{
	//Set vertex position
	gl_Position = vec4(position, 1.0f);
	//Vertex color
	outputColor = inputColor;
}