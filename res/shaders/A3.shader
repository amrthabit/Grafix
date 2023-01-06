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
uniform vec3 u_LightPos;
uniform vec3 u_Attenuation;
uniform sampler2D u_Texture;

void main()
{
	vec3 diffuseColor = vec3(1.0, 1.0, 1.0);
	vec3 ambient = vec3(0.2, 0.2, 0.2);
	vec3 surfaceToLight = normalize(u_LightPos - v_FragPos.xyz);

	float diff = max(dot(v_Normal, surfaceToLight), 0.0);

	vec3 diffuse = diff * diffuseColor;

	float distance = length(u_LightPos - v_FragPos.xyz);

	float attenuation = 1.0 / (u_Attenuation.x + u_Attenuation.y * distance + u_Attenuation.z * distance * distance);

	vec3 result = ambient + diffuse * attenuation;

	vec4 textureColor = texture(u_Texture, v_TexCoord);

	color = vec4(result, 1.0f) * textureColor;
}