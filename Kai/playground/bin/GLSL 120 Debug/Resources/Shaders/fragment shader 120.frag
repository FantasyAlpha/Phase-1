#version 120

varying vec2 outputTexCoords;

uniform sampler2D myTexture;
uniform vec4 myColor;

uniform vec4 ambientColor;
uniform float ambientStrength;

void main()
{
	vec4 totalLight = (ambientStrength * ambientColor);
	vec4 tex = texture2D(myTexture, outputTexCoords);
	gl_FragColor = tex * myColor * totalLight;
}