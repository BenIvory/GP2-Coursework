#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <vector>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();

    //Display and Gamesate
	Display _gameDisplay;
	GameState _gameState;

    //Meshs
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Mesh mesh4;

    //camera
	MyCamera myCamera;

    //Textures
	Texture texture1;
	Texture texture2;
	Texture texture3;
	Shader shader;

    //Audio
	Audio audio;

	//Positions
	glm::vec3 cameraPos;
	glm::vec3 lightPos;
	
	//For Smooth Movement
	std::vector<SDL_Keycode> pressedKeys;
	void AddKey(SDL_Keycode key);
	void RemoveKey(SDL_Keycode key);
	bool CheckKey(SDL_Keycode key);

	//Collison detection
	bool CheckCollison(glm::vec3 pos1, float radius1, glm::vec3 pos2, float radius2);
};

