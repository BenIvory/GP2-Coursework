#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture(); //Default construtor

	void Bind(unsigned int unit); // bind upto 32 textures
	void init(const std::string& fileName); //Initlaise texture

	~Texture(); //Destructor

protected:
private:

	GLuint textureHandler; //USed to hold the address of the texture
};

