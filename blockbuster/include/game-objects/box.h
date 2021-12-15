#pragma once
#include <engine/engine.h>
#include <blockbuster/include/color.h>
#include <blockbuster/include/logic/interpolation.h>
#include <math.h>

class Box {
public:
	Box(glm::vec3 pos, glm::vec3 size, glm::vec4 colour);
	//Render specific
	void onRender();
	//
	void onUpdate(engine::Time ts);
	//alpha value changing functions
	void transperice();
	void solidify();
	//Accessor and mutator methods
	void setSize(glm::vec3 newSize) { m_Size = newSize; }
	glm::vec3 getSize() { return m_Size; }

	void setVirtualPosition(glm::vec3 newPosition) { m_VirtualPosition = newPosition; }
	glm::vec3 getVirtualPosition() { return m_VirtualPosition; }

	void setPosition(glm::vec3 newPosition) { m_Pos = newPosition; }
	
	void setColour(glm::vec4 newColour) { m_Colour = newColour; }
	void setColourOnHeight(int height);
	glm::vec4 getColour() { return m_Colour; }

private:
	//Visual parameters
	glm::vec3 m_Size;
	glm::vec3 m_Pos;
	glm::vec3 m_VirtualPosition;
	glm::vec4 m_Colour;
	//TimeTracking
	float m_time;
	//Texture
	engine::s_Ptr<engine::Texture> m_texture;
	//Interpolation specific 
	engine::s_Ptr<Interpolation> m_interpolator;
	bool moving = false;
	glm::vec3 startPos;
	float travelTime = 0.6f,
		startTime = 0.f;
	//CONSTANTS
	const float m_transparecyLevel = 0.8f;

};

Box::Box(glm::vec3 pos, glm::vec3 size, glm::vec4 colour)
{
	setSize(size);
	setVirtualPosition(pos);
	setPosition(pos);
	setColour(colour);
}

void Box::onUpdate(engine::Time ts)
{
	m_time += ts;
	//If currently moving, move 
	if (moving == true) 
	{ 
		m_Pos = m_interpolator->linear(startPos, m_VirtualPosition, startTime, startTime+travelTime, m_time); 
		//If pos is equal to the virtual position, the movement is complete
		if (m_Pos == m_VirtualPosition) { moving = false; }
		return; //Nothing more should be done 
	}
	//If the virtual and real position are not the same, start moving
	if(m_VirtualPosition != m_Pos) 
	{
		//Set all intial values for interpolation on the next frame
		moving = true;
		startTime = m_time;
		startPos = m_Pos;
	}
}

void Box::setColourOnHeight(int height) 
{
	//Cos is a periodic function that returns -1,1 with a period of 2pi
	float r = abs(cos(height)),
		g = abs(cos(height + 1)),
		b = abs(cos(height + 2));
	setColour({ r, g, b, 1.f });
}
void Box::transperice() { m_Colour = { m_Colour.x, m_Colour.y, m_Colour.z, m_transparecyLevel }; }
void Box::solidify() { m_Colour = {m_Colour.x, m_Colour.y, m_Colour.z, 1.f }; }
void Box::onRender() {
	//engine::Renderer::drawQuad(m_Pos ,m_Size ,m_Colour);
	engine::Renderer::draw3DObject(m_Pos,
		{ m_Size },
		{ 0, 0, 0 },
		m_Colour,
		"./assets/models/box", 
		"box"
		);
}