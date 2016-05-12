#version 120

varying vec2 outputTexCoords;

varying vec4 vertOutColor;
uniform sampler2D myTexture;

void main()
{
	vec4 tex = texture2D(myTexture, outputTexCoords.st);
	gl_FragColor = vertOutColor * tex;
}