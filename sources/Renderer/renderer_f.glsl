#version 330 core
layout (location = 0) out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D brightTexture;
uniform bool blackScreen;

const float stride = 1 / 400.0;
float weights[9] = float [9](0.000229,0.005977,0.060598,0.241732,0.382928,0.241732,0.060598,0.005977,0.000229);

void main()
{
	vec3 color = vec3(0,0,0);
	for(int i = 0;i < 9;i++)
	{
		for(int j = 0; j < 9;j++)
		{
			vec2 samplePoint = TexCoords;

			samplePoint.x = samplePoint.x - stride*(i-4);
			samplePoint.y = samplePoint.y - stride*(j-4);

			color += weights[j]*weights[i]*texture(brightTexture,samplePoint).xyz;
		}
	}

	vec3 colorNormal = texture(screenTexture,TexCoords).xyz;


	vec3 col = colorNormal + color;
	float value = max(max(col.x, col.y),col.z);

	if(value > 1.0) {
		col *= 1/value * 0.95;
	}

	FragColor = vec4(col,1.0);

	if(blackScreen) {
		float average = (FragColor.x + FragColor.y + FragColor.y) / 3.0;
		FragColor.xyz = vec3(average,average,average);
	}
} 