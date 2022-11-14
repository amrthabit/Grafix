#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_FragPos;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_Normal = normal;
	v_TexCoord = texCoord;
	v_FragPos = vec3(position);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_FragPos;

uniform bool u_Disco;
uniform vec3 u_LightPos;
uniform vec3 u_DiffuseColor;
uniform vec3 u_AmbientColor;
uniform vec3 u_Attenuation;
uniform sampler2D u_Texture;

void main()
{
	vec3 diffuseRed = vec3(1.0, 0.0, 0.0);
	vec3 diffuseGreen = vec3(0.0, 1.0, 0.0);
	vec3 diffuseBlue = vec3(0.0, 0.0, 1.0);

	vec3 redDirection = vec3(50.0, -200.0, -50.0);
	vec3 greenDirection = vec3(-50.0, -200.0, -50.0);
	vec3 blueDirection = vec3(0.0, -200.0, 50.0);

	float cutOffAngle = 3.14159265359 * 1.6 / 6.0;

	vec3 lightDir = normalize(u_LightPos - v_FragPos);

	float diff = max(dot(v_Normal, lightDir), 0.0);

	vec3 diffuse = diff * u_DiffuseColor;
	vec3 diffuseRedLight = diff * diffuseRed;
	vec3 diffuseGreenLight = diff * diffuseGreen;
	vec3 diffuseBlueLight = diff * diffuseBlue;

	vec3 ambient = u_AmbientColor;

	if (u_Disco)
	{
		ambient = vec3(0.2, 0.2, 0.2);
	}
	
	float distance = length(u_LightPos - v_FragPos);
	//float distance = 1.0;

	float attenuation = 1.0 / (u_Attenuation.x + u_Attenuation.y * distance + u_Attenuation.z * distance * distance);
	float attenuationRed = attenuation;
	float attenuationGreen = attenuation;
	float attenuationBlue = attenuation;
	
	// account for spotlight radius
	float theta = dot(lightDir, normalize(-lightDir));
	float thetaRed = dot(lightDir, normalize(-redDirection));
	float thetaGreen = dot(lightDir, normalize(-greenDirection));
	float thetaBlue = dot(lightDir, normalize(-blueDirection));

	if (theta < cutOffAngle)
	{
		attenuation = 0.0;
	}
	if (thetaRed < cutOffAngle)
	{
		attenuationRed = 0.0;
	}
	if (thetaGreen < cutOffAngle)
	{
		attenuationGreen = 0.0;
	}
	if (thetaBlue < cutOffAngle)
	{
		attenuationBlue = 0.0;
	}

	vec3 result = ambient + diffuse * attenuation;
	vec3 resultRed = ambient + diffuseRedLight * attenuation;
	vec3 resultGreen = ambient + diffuseGreenLight * attenuation;
	vec3 resultBlue = ambient + diffuseBlueLight * attenuation;

	vec3 finalColor = result;
	if (u_Disco) {
		finalColor = resultRed + resultGreen + resultBlue;
	}
	
	vec4 textureColor = texture(u_Texture, v_TexCoord);

	color = vec4(finalColor, 1.0f) * textureColor;
}