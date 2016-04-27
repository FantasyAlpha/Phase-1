#version 120

varying vec2 outputTexCoords;
varying vec4 outpos;
varying float slot;
varying vec4 myColor;

uniform sampler2D myTexture0;
uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform sampler2D myTexture3;
uniform sampler2D myTexture4;
uniform sampler2D myTexture5;
uniform sampler2D myTexture6;
uniform sampler2D myTexture7;

uniform vec4 ambientColor;
uniform float ambientStrength;
uniform vec2 mousePos;

void main()
{
	float intensity = (1.0f / length(outpos.xy - vec2(mousePos.x, mousePos.y))) * 50.0f;
	//vec4 totalLight = (ambientStrength * ambientColor);
	int tid = int(slot + 0.5f);

	vec4 tex = vec4(1, 1, 1, 1);

	if(tid == 0){	tex = texture2D(myTexture0, outputTexCoords);	}
	if(tid == 1){	tex = texture2D(myTexture1, outputTexCoords);	}
	if(tid == 2){	tex = texture2D(myTexture2, outputTexCoords);	}
	if(tid == 3){	tex = texture2D(myTexture3, outputTexCoords);	}
	if(tid == 4){	tex = texture2D(myTexture4, outputTexCoords);	}
	if(tid == 5){	tex = texture2D(myTexture5, outputTexCoords);	}
	if(tid == 6){	tex = texture2D(myTexture6, outputTexCoords);	}
	if(tid == 7){	tex = texture2D(myTexture7, outputTexCoords);	}

	gl_FragColor = tex * myColor * intensity;// * totalLight;
}