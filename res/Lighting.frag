#version 330 core

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform sampler2D diffuse;

in vec3 Normal;
in vec2 texCoord0;
in vec3 Position;




void main()
{

	//ambient
	float ambientStrength = 0.1; //Strength of ambient
    	vec3 ambientLighting = ambientStrength * lightColor;
	
	//diffuse
	vec3 normal2 = normalize(Normal);
	vec3 targetDirection = normalize(lightPos - Position);
	float diff = max(dot(normal2, targetDirection), 0.0);
	vec3 diffuseLighting = diff * lightColor;
	
	//Specular - Blinn-Phong
	float specularStr = 1;
	vec3 viewDir = normalize(cameraPos - Position);
	vec3 halfwayDir = normalize(targetDirection + viewDir);
	float spec = pow(max(dot(normal2, halfwayDir), 0.0), 32);
	vec3 specularLighting = specularStr * spec * lightColor;  

	//End Result with Textures
    vec3 result = (ambientLighting + diffuseLighting + specularLighting);
    FragColor =  texture2D(diffuse, texCoord0) * vec4(result, 1.0);
}