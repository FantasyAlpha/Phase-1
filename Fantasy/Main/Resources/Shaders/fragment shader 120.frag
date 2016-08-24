#version 120

varying vec2 outputTexCoords;
varying vec4 myColor;

uniform sampler2D myTexture0;

void main()
{
	vec4 tex = vec4(1, 1, 1, 1);
	tex = texture2D(myTexture0, outputTexCoords);	
	
	gl_FragColor = tex * myColor ; 	
}


