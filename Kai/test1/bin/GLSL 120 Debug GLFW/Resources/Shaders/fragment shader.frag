#version 330

//Takes the value from the vertex shader
in vec4 outputColor;
//Output the fragments' colors
out vec4 FragColor;

void main()
{
	FragColor = outputColor;
}