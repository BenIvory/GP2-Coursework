#include "MainGame.h"
#include "MyCamera.h"
#include <iostream>
#include <string>



Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0.0, 0.0)),
Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5, 1.0)),
Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0, 0.0)) };

unsigned int indices[] = { 0, 1, 2 };

Transform transform;


MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();
	gameLoop();
}

void MainGame::initSystems()
{
    //Initlise display
	_gameDisplay.initDisplay();

	//Positions for use later
	cameraPos = glm::vec3(0, 0, 0);
	lightPos = glm::vec3(0, 0, 5.f);

	//Load Meshes
	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\monkey3.obj");
	mesh3.loadModel("..\\res\\t2.obj");
	mesh4.loadModel("..\\res\\Lightbulb.obj");

	//Textures
	texture1.init("..\\res\\bricks.jpg");
	texture2.init("..\\res\\wrinkles.jpg");
	texture3.init("..\\res\\Fabric.jpg");

	//Shaders
	shader.init();

	//Camera
	myCamera.initCamera(cameraPos, 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
	
	//Audio
	unsigned int backgroundMusic;
	backgroundMusic = audio.loadSound("..//res//background.wav");
	audio.playSound(backgroundMusic);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();

		//Checks for collisons between light and other objects.
		if (CheckCollison(mesh1.GetSpherePos(), mesh1.GetSphereRadius(), mesh4.GetSpherePos(), mesh4.GetSphereRadius()))
			std::cout << "Light colliding with: Brick monkey" << std::endl;
		if (CheckCollison(mesh2.GetSpherePos(), mesh2.GetSphereRadius(), mesh4.GetSpherePos(), mesh4.GetSphereRadius()))
			std::cout << "Light colliding with: Fabric monkey" << std::endl;
		if (CheckCollison(mesh3.GetSpherePos(), mesh3.GetSphereRadius(), mesh4.GetSpherePos(), mesh4.GetSphereRadius()))
			std::cout << "Light colliding with: Teapot" << std::endl;
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;

		case SDL_KEYDOWN: //On key down
			AddKey(evnt.key.keysym.sym); //add key to the vector
			break;

		case SDL_KEYUP: //on key up
			RemoveKey(evnt.key.keysym.sym); // remove key from the vector
			break;
		}
	}

	//Movement
	float speed = 0.1f;
	if (CheckKey(SDLK_w))//UP
		lightPos.y += speed;
	if (CheckKey(SDLK_s))//Down
		lightPos.y -= speed;
	if (CheckKey(SDLK_a))//Left
		lightPos.x += speed;
	if (CheckKey(SDLK_d))//Right
		lightPos.x -= speed;
	if (CheckKey(SDLK_UP))//Forward
		lightPos.z += speed;
	if (CheckKey(SDLK_DOWN))//Back
		lightPos.z -= speed;

}


void MainGame::drawGame()
{
	//Clearing the display
	_gameDisplay.clearDisplay(0.1f, 0.1f, 0.0f, 1.0f);

	//Setting positions to be used later
	glm::vec3 monkeyPosition(5, 0.0, 15.0);
	glm::vec3 monkey2Pos(-5, 5, 15);
	glm::vec3 teapotPos(-2, 0, 15);

	
	transform.SetRot(glm::vec3(0.0, 3.14f, 0.0)); //sets the rotation for the first model.

	//Monkey - Model
	shader.Bind(); //Bind the shader
	transform.SetPos(monkeyPosition); //Set the position
	transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f)); //sets the scale
	shader.Update(transform, myCamera, glm::vec3(1.0, 1.0, 1.0), lightPos, monkeyPosition, cameraPos); //Updates the uniforms for the shader
	texture3.Bind(0); //Bind the relvant texture to the model.
	mesh1.draw(); //Draw the mesh
	mesh1.UpdateSphereData(*transform.GetPos(), 1.2f); //update the bounding sphere

	//Monkey2 - Model
	transform.SetPos(monkey2Pos); //Set the position
	transform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f)); //sets the scale
	shader.Update(transform, myCamera, glm::vec3(1.0, 1.0, 1.0), lightPos, monkey2Pos, cameraPos); //Updates the uniforms for the shader
	texture2.Bind(0); //Bind the relvant texture to the model.
	mesh2.draw(); //Draw the mesh
	mesh2.UpdateSphereData(*transform.GetPos(), 1.2f); //update the bounding sphere

	//TeaPot - Model
	transform.SetPos(teapotPos); //Set the position
	transform.SetRot(glm::vec3(-1.f, 0.0, 0.0)); //sets the rotation
	transform.SetScale(glm::vec3(0.03f, 0.03f, 0.03f)); //sets the scale
	shader.Update(transform, myCamera, glm::vec3(1.0, 1.0, 1.0), lightPos, teapotPos, cameraPos); //Updates the uniforms for the shader
	texture1.Bind(0); //Bind the relvant texture to the model.
	mesh3.draw(); //Draw the mesh
	mesh3.UpdateSphereData(*transform.GetPos(), 1.2f); //update the bounding sphere


	//Lightbulb - Light
	transform.SetPos(lightPos); //Set the position
	transform.SetRot(glm::vec3(0.f, 0.0, 0.0)); //sets the rotation
	transform.SetScale(glm::vec3(0.5f, 0.5f, 0.5f)); //sets the scale
	shader.Update(transform, myCamera, glm::vec3(1.0, 1.0, 1.0), lightPos, lightPos, cameraPos); //Updates the uniforms for the shader
	mesh4.draw(); //Draw the mesh
	mesh4.UpdateSphereData(*transform.GetPos(), 0.4f);//update the bounding sphere

	//Double buffer
	_gameDisplay.swapBuffer(); //update the display
}


//Adds keystroke
void MainGame::AddKey(SDL_Keycode key)
{
	
	std::vector<SDL_Keycode>::iterator it; //iterater of the same type as vector
	bool found = false;

	for (it = pressedKeys.begin(); it != pressedKeys.end(); it++) //Loop thorugh the pressed keys
	{
		if (*it == key) //Found Key
		{
			found = true; //True
		}
	}

	if (!found) 
	{
		pressedKeys.push_back(key); //if not found, add the key to the vector
	}
}


//Removes keypress
void MainGame::RemoveKey(SDL_Keycode key)
{
	std::vector<SDL_Keycode>::iterator it; //iterater of the same type as vector

	for (it = pressedKeys.begin(); it != pressedKeys.end();) //Loop thorugh the pressed keys, but only iterate if not found
	{
		if (*it == key) //Found key
		{
			it = pressedKeys.erase(it); //Erase the key that the iterater is pointing at.
		}
		else
		{
			it++; //Iterate
		}
	}
}


//Checks key press
bool MainGame::CheckKey(SDL_Keycode key)
{
	std::vector<SDL_Keycode>::iterator it; //iterater of the same type as vector

	
	for (it = pressedKeys.begin(); it != pressedKeys.end(); it++) //Loop thorugh the pressed keys
	{
		if (*it == key) //Found key
		{
			return true; //True
		}
	}

	return false; //Found key - false
}


//Checks collison 
bool MainGame::CheckCollison(glm::vec3 pos1, float radius1, glm::vec3 pos2, float radius2)
{
    //Distance calculations 
	float distance = glm::sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) + (pos2.z - pos1.z) * (pos2.z - pos1.z));

    //Collided or not
	return distance < radius1 + radius2;
}