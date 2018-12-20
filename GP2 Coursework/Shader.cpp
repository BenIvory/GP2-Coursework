#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader()
{
}

void Shader::init()
{

	program = glCreateProgram(); // create shader program (openGL saves as ref number)
	shaders[0] = CreateShader(LoadShader("..\\res\\\Lighting.vert"), GL_VERTEX_SHADER); // create vertex shader
	shaders[1] = CreateShader(LoadShader("..\\res\\Lighting.frag"), GL_FRAGMENT_SHADER); // create fragment shader


	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(program, shaders[i]); //add all our shaders to the shader program "shaders" 
	}

	glBindAttribLocation(program, 0, "position"); // associate attribute variable with our shader program attribute (in this case attribute vec3 position;)
	glBindAttribLocation(program, 1, "texCoord");
	glBindAttribLocation(program, 2, "normals");

	glLinkProgram(program); //create executables that will run on the GPU shaders
	CheckShaderError(program, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(program); //check the entire program is valid
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[0] = glGetUniformLocation(program, "m"); //Uniform for model matrix 
	uniforms[2] = glGetUniformLocation(program, "lightColor"); //Uniform for the color of the light
	uniforms[3] = glGetUniformLocation(program, "lightPos"); //Uniform for position of the light
	uniforms[4] = glGetUniformLocation(program, "target"); //Uniform for the target of the light
	uniforms[5] = glGetUniformLocation(program, "cameraPos"); //Uniform for the position of the camera
	uniforms[6] = glGetUniformLocation(program, "vp"); //Uniform for the view projection matrix
	
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++) //Loop through the shaders.
	{
		glDetachShader(program, shaders[i]); //detach shader from program
		glDeleteShader(shaders[i]); //delete the sahders
	}
	glDeleteProgram(program); // delete the program
}

void Shader::Bind()
{
	glUseProgram(program); //installs the program object specified by program as part of rendering state
}

void Shader::Update(const Transform& transform, const MyCamera& camera, glm::vec3 lightColor, glm::vec3 lightPos, glm::vec3 targetPos, glm::vec3 cameraPos)
{
	glm::mat4 m = transform.GetModel(); //Defining m as the model matrix
	glm::mat4 vp = camera.GetViewProjection(); // Defining vp as the view projection matrix
	glUniformMatrix4fv(uniforms[0], 1, GLU_FALSE, &m[0][0]); //Model matrix
	glUniformMatrix4fv(uniforms[6], 1, GLU_FALSE, &vp[0][0]); //View projection matrix
	glUniform3f(uniforms[2], lightColor.x, lightColor.y, lightColor.z); //Light colour
	glUniform3f(uniforms[3], lightPos.x, lightPos.y, lightPos.z); //lightPos
	glUniform3f(uniforms[4], targetPos.x, targetPos.y, targetPos.z); //Target Pos
	glUniform3f(uniforms[5], cameraPos.x, cameraPos.y, cameraPos.z); //Camera Pos
	
}


GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl; //Print error

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error


	return shader;
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file; //Input stream class to operate on files
	file.open((fileName).c_str()); //Opens file of filename

	std::string output;  //declares new varaibles
	std::string line;

	if (file.is_open()) //if the file is open
	{
		while (file.good()) // good refers to the fact that none of the stream's error state are triggered
		{
			getline(file, line); //Turns line into string
			output.append(line + "\n"); //apennd allows the use of extra/addiontal characters.
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl; //Print error
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0; //Variables
	GLchar error[1024] = { 0 };

	if (isProgram) //Check shader or program, if shader created succefully, check the program, else, check the shader.
		glGetProgramiv(shader, flag, &success); //return a parameter from a program object (program object to be queried, the object parameter, third parameter returns the requested object parameters)
	else
		glGetShaderiv(shader, flag, &success); //Return a parameter from a shader object 

	if (success == GL_FALSE) //failed
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error); //used to get the information log from a program object
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error); //used top get the informaion log from a shader object

		std::cerr << errorMessage << ": '" << error << "'" << std::endl; // Prints Error
	}
}

