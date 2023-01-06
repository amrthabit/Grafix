#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec4 v_FragPos;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_Normal = normal;
	v_TexCoord = texCoord;
	v_FragPos = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec4 v_FragPos;

uniform float u_Theta;
uniform vec3 u_Attenuation;
uniform sampler2D u_Texture;

void main()
{
	vec3 lightPos = vec3(0, 200, 0);

	vec3 red = vec3(1.0, 0.0, 0.0);
	vec3 green = vec3(0.0, 1.0, 0.0);
	vec3 blue = vec3(0.0, 0.0, 1.0);

	vec3 surfaceToLight = normalize(lightPos - v_FragPos.xyz);

	float diff = max(dot(v_Normal, surfaceToLight), 0.0);
	
	vec3 redLight = diff * red;
	vec3 greenLight = diff * green;
	vec3 blueLight = diff * blue;

	float distance = length(lightPos - v_FragPos.xyz);

	float attenuation = 1.0 / (u_Attenuation.x + u_Attenuation.y * distance + u_Attenuation.z * distance * distance);
	float attenuationRed = attenuation;
	float attenuationGreen = attenuation;
	float attenuationBlue = attenuation;	

	// rotation around y axis counter clockwise
	mat4 rotateAroundY = mat4(
		cos(-u_Theta), 0.0, sin(-u_Theta), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(-u_Theta), 0.0, cos(-u_Theta), 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	vec3 redDirection = vec3(rotateAroundY * vec4(50.0, -200.0, -50.0, 1.0));
	vec3 greenDirection = vec3(rotateAroundY * vec4(-50.0, -200.0, -50.0, 1.0));
	vec3 blueDirection = vec3(rotateAroundY * vec4(0.0, -200.0, 50.0, 1.0));

	// account for spotlight radius
	float thetaRed = acos(dot(surfaceToLight, normalize(-redDirection)));
	float thetaGreen = acos(dot(surfaceToLight, normalize(-greenDirection)));
	float thetaBlue = acos(dot(surfaceToLight, normalize(-blueDirection)));

	float cutOffAngle = 3.14159265359 / 6.0;

	if (thetaRed > cutOffAngle)
	{
		attenuationRed = 0.0;
	}
	if (thetaGreen > cutOffAngle)
	{
		attenuationGreen = 0.0;
	}
	if (thetaBlue > cutOffAngle)
	{
		attenuationBlue = 0.0;
	}
	
	vec3 ambient = vec3(0.2, 0.2, 0.2);
	
	vec3 resultRed = ambient + redLight * attenuationRed;
	vec3 resultGreen = ambient + greenLight * attenuationGreen;
	vec3 resultBlue = ambient + blueLight * attenuationBlue;

	vec3 finalColor = resultRed + resultGreen + resultBlue;
	vec4 textureColor = texture(u_Texture, v_TexCoord);

	color = vec4(finalColor, 1.0f) * textureColor;
}