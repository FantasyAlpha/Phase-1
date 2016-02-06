#version 330

//Takes the value from the vertex shader
in vec4 outputColor;
in vec2 outputTexCoords;

//Output the fragments' colors
out vec4 FragColor;

uniform sampler2D myTexture;

void main()
{
	vec4 tex = texture(myTexture, outputTexCoords);
	FragColor = tex * outputColor;
}
