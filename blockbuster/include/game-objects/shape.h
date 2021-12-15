#pragma once
#include <engine/engine.h>
#include <blockbuster/include/game-objects/box.h>
#include <math.h>
class Shape {
public:
	Shape();

	void onUpdate(engine::Time ts, int signal);
	//construction for shapes
	void constructZ();
	void constructT();
	void constructL();

	//xy - plane rotations
	void rotateRight();
	void rotateLeft();
	
	//zx - plane rotations
	void posPitch();
	void negPitch();
	//zy - plane rotations
	void posRoll();
	void negRoll();
	//Translation functions 
	void nextToCurrent();
	void submitPositions();
	void revertPositions();
	void findPositions();
	std::vector<glm::vec3> getPositions() { return realPositions; }
	//Mutator and accessor methods
	std::vector<engine::s_Ptr<Box>> getBoxes() { return m_Boxes; }
	void clearBoxes() { m_Boxes.clear(); }

	//Variables
	std::vector<glm::vec3> realPositions;
private:
	int m_orientation[3][3][3];
	int m_nextOrientation[3][3][3];
	//Center as as as as !!
	int x = 1,
		y = 1,
		z = 1;
	glm::vec3 m_center = { x,y,z };
	std::vector<engine::s_Ptr<Box>> m_Boxes;
	bool m_approved;
	//Translation variables
	glm::vec3 realWorldCoordinate = { 3,3,10 };
	glm::vec3 newCoordinate;
	//Timeholder
	float updateTime = 2.f,
		lastUpdate = 0.f;
};

Shape::Shape() {}

//A really cool idea for this signal system is to make inverted signals
//As most of these are negative / positive action, we can make the
//linked ones -X for negative action and +X for positive action
void Shape::onUpdate(engine::Time ts, int signal)
{
	lastUpdate += ts;								
													
	switch (signal)
	{
	case 0:
		newCoordinate = realWorldCoordinate;
		nextToCurrent();
		break;
	case 1:
		newCoordinate = { realWorldCoordinate.x, realWorldCoordinate.y, realWorldCoordinate.z - 1};			//	===   Return Codes and their meaning ====
		nextToCurrent();																					//
		break;																								// 0 - do nothing
	case 2:																									// 1 - go downwards
		newCoordinate = { realWorldCoordinate.x, realWorldCoordinate.y + 1, realWorldCoordinate.z };		// 2 - go forwards
		nextToCurrent();																					// 3 - go backwards
		break;																								// 4 - go right	
	case 3:																									// 5 - go left
		newCoordinate = { realWorldCoordinate.x, realWorldCoordinate.y - 1, realWorldCoordinate.z };		// 6 - posYaw
		nextToCurrent();																					// 7 - negYaw	
		break;																								// 8 - posRoll	
	case 4:																									// 9 - negRoll	
		newCoordinate = { realWorldCoordinate.x + 1, realWorldCoordinate.y, realWorldCoordinate.z };		//10 - posPitch	
		nextToCurrent();																					//11 - negPitch
		break;										
	case 5:											
		newCoordinate = { realWorldCoordinate.x - 1, realWorldCoordinate.y, realWorldCoordinate.z };
		nextToCurrent();
		break;
	case 6:
		newCoordinate = realWorldCoordinate;
		rotateRight();
		break;
	case 7:
		newCoordinate = realWorldCoordinate;
		rotateLeft();
		break;
	case 8:
		newCoordinate = realWorldCoordinate;
		posRoll();
		break;
	case 9:
		newCoordinate = realWorldCoordinate;
		negRoll();
		break;
	case 10:
		newCoordinate = realWorldCoordinate;
		posPitch();
		break;
	case 11:
		newCoordinate = realWorldCoordinate;
		negPitch();
		break;
	}
	findPositions();
}													
													
void Shape::findPositions() 
{
	realPositions.clear();
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {

				int value = m_nextOrientation[x][i][j];

				switch (value) 
				{
				case 0:
					break;
				case 1:
					//Vector math, m_center is the real center (translated to 0,0,0), but has position 2,2,2
					// ==> m_center - m_center = 0,0,0, for something in [1,1,1] - [2,2,2] = [-1,-1,-1]
					//So any other positions needs to be displaced by m_center
					//glm::vec3 matrixPos = { x, i, j};
					//glm::vec3 position;
					//position = { matrixPos.x - m_center.x,
					//			 matrixPos.y - m_center.y, 
					//			matrixPos.z - m_center.z };

					//Avoiding errors in floating point operations
					//if (x == 1 && i == 1 && j == 1) {
					//	position = { 0,0,0 };
					//}
					
					//To translate it into real world, add vector to realWorld
					//glm::vec3 toRealWorld = realWorldCoordinate + position;
					
					int xPos = i - m_center.x + newCoordinate.x,
						yPos = j - m_center.y + newCoordinate.y,
						zPos = x - m_center.x + newCoordinate.z;

					//
					realPositions.push_back({ xPos,yPos,zPos });
				}
			}
		}
	}
}
void Shape::constructZ()
{
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				m_orientation[x][i][j] = 0;
			}
		}
	}
															//Setting second matrix to look like this
	m_orientation[1][0][1] = 1;								//[0, 1, 0]
	m_orientation[1][1][1] = 1; m_orientation[1][1][2] = 1;	//[0, 1, 1]
	m_orientation[1][2][2] = 1;								//[0, 0, 1]
	//Set nextPosition to current
	nextToCurrent();
	newCoordinate = { 3,3,10 };
	realWorldCoordinate = { 3,3,10 };
	//Now need to push back the objects
	for (int i = 0; i < 4; i++) {
		engine::s_Ptr<Box> box = engine::m_SPtr<Box>(glm::vec3(0.f, 0.f, 0.f),
												glm::vec3(0.485f, 0.485f, 0.485f),
												glm::vec4(0.19f, 0.69f, 0.95f, 1.f) );
		box->transperice();
		m_Boxes.push_back(box);
	};
}
void Shape::constructT()
{
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				m_orientation[x][i][j] = 0;
			}
		}
	}
	//Setting second matrix to look like this
	m_orientation[1][0][1] = 1;								//[0, 1, 0]
	m_orientation[1][1][1] = 1; m_orientation[1][1][2] = 1;	//[1, 1, 1]
	m_orientation[1][1][0] = 1;								//[0, 0, 0]
	
	nextToCurrent();
	newCoordinate = { 3,3,10 };
	realWorldCoordinate = { 3,3,10 };

	//Now need to push back the objects
	for (int i = 0; i < 4; i++) {
		engine::s_Ptr<Box> box = engine::m_SPtr<Box>(glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.485f, 0.485f, 0.485f),
			glm::vec4(0.19f, 0.69f, 0.95f, 1.f));
		box->transperice();
		m_Boxes.push_back(box);
	};
}
void Shape::constructL()
{
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				m_orientation[x][i][j] = 0;
			}
		}
	}
	//Setting second matrix to look like this
	m_orientation[1][0][1] = 1;									//[0, 1, 0]
	m_orientation[1][1][1] = 1; 								//[0, 1, 0]
	m_orientation[1][2][1] = 1;	m_orientation[1][2][2] = 1;		//[0, 1, 1]
	
	nextToCurrent();
	newCoordinate = { 3,3,10 };
	realWorldCoordinate = { 3,3,10 };
	
	//Now need to push back the objects
	for (int i = 0; i < 4; i++) {
		engine::s_Ptr<Box> box = engine::m_SPtr<Box>(glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.485f, 0.485f, 0.485f),
			glm::vec4(0.19f, 0.69f, 0.95f, 1.f));
		box->transperice();
		m_Boxes.push_back(box);
	};
}
/*
* Angle shifting each element
* [1, 0, 0]    [0, 1, 1]    [0, 1, 0]
* [1, 1, 0] -> [1, 1, 0] -> [0, 1, 1]
* [0, 1, 0]	   [0, 0, 0]	[0, 0, 1]
* 
* Translation List:
* a11 = a13, a12 = a23, a13 = a33
* a21 = a12, a22 = a22, a23 = a32
* a31 = a11, a32 = a21, a33 = a31
*/

//Credit for finding this logic goes 100% to me - Thomas Vincent Lien
//This was hard
void Shape::rotateRight() 
{
	int  j_index;
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				switch (i) 
				{
				case 0:
					j_index = 2;
					break;
				case 1:
					j_index = 1;
					break;
				case 2:
					j_index = 0;
					break;
				}
				m_nextOrientation[x][i][j] = m_orientation[x][j][j_index];
			}
		}
	}
}
//Using the logic from rotateRight, we can do the opposite by reversing the indexing!
void Shape::rotateLeft()
{
	int  j_index;
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				switch (i)
				{
				case 0:
					j_index = 2;
					break;
				case 1:
					j_index = 1;
					break;
				case 2:
					j_index = 0;
					break;
				}
				m_nextOrientation[x][j][j_index] = m_orientation[x][i][j];
			}
		}
	}
}
//For Pitching we can do the same. Instead of operating on each matrix
//We slice each matrix at their first column to form a new matrix
void Shape::posPitch()
{
	int  j_index;
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				switch (i)
				{
				case 0:
					j_index = 2;
					break;
				case 1:
					j_index = 1;
					break;
				case 2:
					j_index = 0;
					break;
				}
				m_nextOrientation[x][i][j] = m_orientation[j_index][x][j];
			}
		}
	}
}
//Like with left and right rotation, we just swap the indecies on m_nextOrientation and m_orientation for the rotation
void Shape::negPitch()
{
	int  j_index;
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				switch (i)
				{
				case 0:
					j_index = 2;
					break;
				case 1:
					j_index = 1;
					break;
				case 2:
					j_index = 0;
					break;
				}
				m_nextOrientation[j_index][x][j] = m_orientation[x][i][j];
			}
		}
	}
}
//Translation list for rolling
// a11 = a13 a12 = b23 a13 = c33
// b21 = a12 b22 = b22 b23 = c32
// c31 = a11 c32 = b21 c33 = c31
//
//Example for what needs to happen:
// b21 -> a12 
// 221 -> 112
void Shape::posRoll()
{
	int  j_index;
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				switch (j)
				{
				case 0:
					j_index = 2;
					break;
				case 1:
					j_index = 1;
					break;
				case 2:
					j_index = 0;
					break;
				}
				m_nextOrientation[x][i][j] = m_orientation[j_index][i][x];
			}
		}
	}
}
void Shape::negRoll()
{
	int  j_index;
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				switch (j)
				{
				case 0:
					j_index = 2;
					break;
				case 1:
					j_index = 1;
					break;
				case 2:
					j_index = 0;
					break;
				}
				m_nextOrientation[j_index][i][x] = m_orientation[x][i][j];
			}
		}
	}
}

//This function does the equivalent to :  m_orientation = m_nextOrientation
void Shape::submitPositions()
{
	int counter = 0;
	for (auto it : realPositions)
	{
		engine::s_Ptr<Box> box = m_Boxes[counter];
		box->setVirtualPosition(realPositions[counter]);
		counter++;
	}

	realWorldCoordinate = newCoordinate;

	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
			
				m_orientation[x][i][j] = m_nextOrientation[x][i][j];
			}
		}
	}
}

//Revert both newCoordinate and orientation
void Shape::revertPositions()
{
	newCoordinate = realWorldCoordinate;

	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {

				m_nextOrientation[x][i][j] = m_orientation[x][i][j];
			}
		}
	}
}

void Shape::nextToCurrent() 
{
	for (int x = 0; x < 3; x++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {

				m_nextOrientation[x][i][j] = m_orientation[x][i][j];
			}
		}
	}
}