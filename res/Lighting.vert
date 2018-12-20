#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normals;

out vec3 Normal;
out vec2 texCoord0;
out vec3 Position;

uniform mat4 m;
uniform mat4 vp;

void main()
{
	texCoord0 = texCoord; //Texture Coords.
	Normal = mat3(transpose(inverse(m))) * normals; 


	Position = vec3(m * vec4(position, 1.0));
	gl_Position = vp * vec4(Position, 1.0);
	
}



