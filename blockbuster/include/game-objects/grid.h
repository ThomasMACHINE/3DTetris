#pragma once
#include <engine/engine.h>
#include <blockbuster/include/game-objects/box.h>
#include <blockbuster/include/characters/player.h>
/*
* The Grid class has the responsibilty to load all the other game objects
* and dictate the state of the game (e.g. reseting, and setting the state)
* 
* The description of Grid in which all the shapes in BlockBuster falls down,
* can be found in the Wiki.
*/
const int Height = 11;
const int Width = 7;
const int Length = 7;
class Grid
{
public:
	Grid();

	void load();
	void onUpdate(engine::Time ts);
	void onRender();
	void gameOver();
	void reset();	// Run when game is over and players/ ghost get back to positions

	bool getOccupied(int z, int y, int x);

	bool isGameOver() const { return m_GameOver; }
	void* getMapMatrix() { return m_GridMatrix; }	// Accessor method for the mapMatrix
	glm::vec3 getDimensions() { return { m_Length, m_Width, m_Height }; }
private:
	//Grid values
	int m_Height = Height;
	int	m_Width = Width;
	int m_Length = Length;
	int m_GridMatrix[Length][Width][Height];	//3D list that holds all values for the level

	//Game-Objects
	std::vector<engine::s_Ptr<Box>> m_Boxes; //All boxes
	engine::s_Ptr<Player> m_Player;					// Player

	//Game values
	bool m_GameOver = false; // State - game is either over or ongoing
	int m_Score = 0;         // Score - The score for current game
	//Shader
	engine::ObjectLibrary* s_ObjectLibrary;
	engine::ShaderLibrary* s_ShaderLibrary;

};
//Constructor
//Sets up window, shader and other game-objects used
Grid::Grid() {
	load();

	// Get the shader
	s_ShaderLibrary = engine::Renderer::getShaderLibrary();

	// Setup view with camera
	auto& window = engine::AppFrame::get().getWindow();	

	//Camera position
	glm::vec3 cam = { 0, 0, 0 };
}

void Grid::onUpdate(engine::Time ts)
{
	return;
}

void Grid::load() 
{
	engine::Renderer::loadShape("./assets/models/wall", "wall");
	//Load Map
	for (int x = 0; x < m_Length; x++) {
		for (int y = 0; y < m_Width; y++) {
			for (int z = 0; z < m_Height; z++) {
				//If the slot is at the edge of the grid, then place a wall in it
				if (x == 0 || x == m_Length-1|| y == 0 || y == m_Width-1 || z == 0)
				{
					m_GridMatrix[x][y][z] = 1;
					m_Boxes.push_back(engine::m_SPtr<Box>(glm::vec3(x- m_Length/2, y- m_Width/2, z-m_Height),
						glm::vec3(0.485f, 0.485f, 0.485f),
						glm::vec4(0.1f, 0.69f, 0.19f, 1.f)));
				}
				else
				{
					m_GridMatrix[x][y][z] = 0;
				}
			}
		}
	}
	return;
}
//
//void Grid::onUpdate(engine::Time ts) 
//{
//
//	return;
//}

void Grid::onRender() 
{
	for (auto &it : m_Boxes)
	{
		it->onRender();
	}
	return;
}
/*
* Checks if the space is occupied
*/
bool Grid::getOccupied(int z, int y, int x) 
{
	//A block with height 2 might be placed when there is 1 slot left
	//Simple solution until I can think of a better way to avoid out of index
	if (z > m_Height || y > m_Width || x > m_Length) 
	{
		APP_INFO("Do not go out of index!");
		return true;
	}
	//Precaution incase something weird happens
	else if (z < 0 || y < 0 || x < 0)
	{
		APP_INFO("Traveling below the grid..");
		return true;
	}
	int value = m_GridMatrix[z][y][x];

	switch (value) {
	case 0:
		return false;
		break;

	case 1:
		return true;
		break;

	default:
		APP_INFO("GridMatrix contains values which are not 0 or 1");
		return false;

	}
}