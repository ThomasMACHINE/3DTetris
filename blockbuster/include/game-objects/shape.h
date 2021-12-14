#pragma once
#include <engine/engine.h>
#include <math.h>
class Shape {
public:
	Shape();


	//Rotation functions
	void rotateRight();
	void rotateLeft();
private:
	int[3][3][3] m_orientation;

	int[3][3][3] m_nextOrientation;
	std::vector<engine::s_Ptr<Box>> m_Boxes;
};
/*
* All 1`s apart from the centre is rotated 45 degrees
* 
*  [1, 0, 0] RotateRight [0, 1, 1] RotateRight [0, 1, 0]    
*  [1, 1, 0]     ==>	 [1, 1, 0]	  ====>    [0, 1, 1]
*  [0, 1, 0]			 [0, 0, 0]			   [0, 0, 1]
* 
* Substitute each row with column -- c1 = r1^T, c2 = r2^T, c3 = r3^T
* [0, 0, 1]    [0, 0, 0]     [0, 0, 1]    [0, 0, 0]
* [0, 1, 1] -> [0, 1, 1] ->	 [0, 1, 1] -> [0, 1, 1]
* [0, 1, 0]	   [1, 1, 0]	 [0, 1, 0]	  [1, 1, 0]
* 
* Substitute each column with row -- r1 = c1^T ...
* [0, 0, 1]     [0, 0, 0]
* [0, 1, 1] -->	[0, 1, 1]  
* [0, 1, 0]		[1, 1, 0]
* 
* Both methods are equal, 
* 
* For the T letter
* [1, 1, 1]     [1, 0, 0]    [1, 1, 1]
* [0, 1, 0] ->	[1, 1, 1] -> [0, 1, 0]
* [0, 1, 0]		[1, 0, 0]	 [0, 1, 0]
* 
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
	for (x = 0; x < 3; x++) {
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
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
				m_nextOrientation[x][i][j] = m_Orientation[x][j][j_index]
			}
		}
	}
}
//Using the logic from rotateRight, we can do the opposite by reversing the indexing!
void Shape::rotateLeft()
{
	int  j_index;
	for (x = 0; x < 3; x++) {
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
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
				m_nextOrientation[x][j][j_index] = m_Orientation[x][i][j]
			}
		}
	}
}

