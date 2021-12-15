#pragma once
#include <engine/engine.h>
#include <blockbuster/include/game-objects/box.h>
#include <blockbuster/include/characters/player.h>
#include <blockbuster/include/game-objects/shape.h>
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

	//Box generation
	virtual void newActiveBox();
private:
	//Grid values
	int m_Height = Height;
	int	m_Width = Width;
	int m_Length = Length;
	int m_GridMatrix[Length][Width][Height];	//3D list that holds all values for the level

	//Game-Objects
	std::vector<engine::s_Ptr<Box>> m_walls; //All walls
	std::vector<engine::s_Ptr<Box>> m_Boxes; //All playing boxes
	engine::s_Ptr<Player> m_Player;			 // Player
	engine::s_Ptr<Shape> m_Shape;
	//Object-Constants
	glm::vec4 standardColour = { 0.1f, 0.69f, 0.19f, 1.f };
	glm::vec3 standardSize = { 0.485f, 0.485f, 0.485f };
	glm::vec3 startPos = { Length / 2, Width / 2 , Height - 1 };
	//Game values
	bool m_GameOver = false; // State - game is either over or ongoing
	int m_Score = 0;         // Score - The score for current game
	int m_Counter = 0;
	//Game Texture 
	engine::s_Ptr<engine::Texture> m_boxTexture = engine::m_SPtr<engine::Texture>("assets/textures/box2.jpg");
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


void Grid::load() 
{
	//Instantiate player
	m_Player = engine::m_SPtr<Player>();
	//Instantiate Shape and load first block
	m_Shape = engine::m_SPtr<Shape>();
	newActiveBox();
	//LoadBoxes
	engine::Renderer::loadShape("./assets/models/box", "box");
	//Load Map
	for (int x = 0; x < m_Length; x++) {
		for (int y = 0; y < m_Width; y++) {
			for (int z = 0; z < m_Height; z++) {
				//If the slot is at the edge of the grid, then place a wall in it
				if (x == 0 || x == m_Length-1|| y == 0 || y == m_Width-1 || z == 0)
				{
					m_GridMatrix[x][y][z] = 1;
					m_walls.push_back(engine::m_SPtr<Box>(glm::vec3(x, y, z),
						glm::vec3(0.485f, 0.485f, 0.485f),
						glm::vec4(0.8f, 1.0f, 0.69f, 1.0f)
						));
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
/*
* Get movementCode from Player
* 
* do movementCalculation on activeBlock
* 
* if it collides, do not update
* 
* else update position
*/
void Grid::onUpdate(engine::Time ts) 
{
	int signal = 0;
	signal = m_Player->onUpdate(ts);
	m_Shape->onUpdate(ts, signal);

	std::vector<glm::vec3> positions = m_Shape->realPositions;
	bool collision = false;
	for (auto &it :positions)
	{
		if (getOccupied(it.x, it.y, it.z) == true) 
		{ 
			collision = true; 
		}
	}
	if(collision == true){ 
		m_Shape->revertPositions(); 
	}
	else { 
		m_Shape->submitPositions(); 
	}
	//Check for collision under the box
	bool collisionUnder = false;
	for (auto &it : positions)
	{
 		if (getOccupied(it.z, it.y, it.z - 1) == true) 
		{ 
			collisionUnder = true; 
			APP_INFO("DEBUGLINE");
		}
	}
	if(collisionUnder == true)
	{
		auto boxList = m_Shape->getBoxes();
		for (auto &it : boxList)
		{
			it->solidify();
			it->setColourOnHeight(it->getVirtualPosition().z);
		}
		m_Shape->clearBoxes();
		newActiveBox();
	}
	for (auto it : m_Boxes)
	{
		it->onUpdate(ts);
	}
	return;
}

void Grid::onRender() 
{
	for (auto& it : m_walls)
	{
		it->onRender();
	}
	for (auto &it : m_Boxes)
	{
		it->onRender();
	}
	return;
}
/*
* Checks if the space is occupied
*/
bool Grid::getOccupied(int x, int y, int z) 
{
	//A block with height 2 might be placed when there is 1 slot left
	//Simple solution until I can think of a better way to avoid out of index
	if (x > m_Length || y > m_Width || z > m_Height) 
	{
		APP_INFO("Do not go out of index!");
		return false;
	}
	//Precaution incase something weird happens
	else if (z < 0 || y < 0 || x < 0)
	{
		APP_INFO("Traveling below the grid..");
		return true;
	}
	int value = m_GridMatrix[x][y][z];

	switch (value) {
	case 0:
		return false;
		break;

	case 1:
		return true;
		break;

	default:
		APP_INFO("GridMatrix contains values which are not 0 or 1");
		APP_INFO(value);
		return false;

	}
}

//Sets new active boxes and adds them to m_Boxes in Grid 
void Grid::newActiveBox() 
{
	//Cycle through different Letters
	switch (m_Counter)
	{
	case 0:
		m_Shape->constructL();
		m_Counter = 1;
		break;
	case 1:
		m_Shape->constructT();
		m_Counter = 2;
		break;
	case 2:
		m_Shape->constructZ();
		m_Counter = 0;
		break;
	}
	m_Shape->findPositions();
	auto newBoxes = m_Shape->getBoxes();
	//Add to vector
	for (auto it : newBoxes) {
		m_Boxes.push_back(it);
		it->transperice();
	}
	return;
}