#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "MyCamera.h"

class Shader
{
public:
	Shader();

	void Bind(); //Set gpu to use our shaders
	void Update(const Transform & transform, const MyCamera & camera, glm::vec3 lightColor, glm::vec3 lightPos, glm::vec3 monkeyPos, glm::vec3 cameraPos);
	


	void init();

	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);

    ~Shader();


protected:
private:
	static const unsigned int NUM_SHADERS = 2; // number of shaders

	
	GLuint program; // Track the shader program
	GLuint shaders[NUM_SHADERS]; //array of shaders
	GLuint uniforms[8]; //no of uniform variables
};
