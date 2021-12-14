#pragma once
#include <engine/engine.h>
#include <blockbuster/include/color.h>

class Box {
public:
	Box(glm::vec3 pos, glm::vec3 size, glm::vec4 colour);

	void onRender();

	void transperice();
	void solidify();
	//Accessor and mutator methods
	void setSize(glm::vec3 newSize) { m_Size = newSize; }
	glm::vec3 getSize() { return m_Size; }

	void setPosition(glm::vec3 newPosition) { m_Pos = newPosition; }
	glm::vec3 getPosition() { return m_Pos; }

	void setColour(glm::vec4 newColour) { m_Colour = newColour; }
	glm::vec4 getColour() { return m_Colour; }

private:
	glm::vec3 m_Size;
	glm::vec3 m_Pos;
	glm::vec4 m_Colour;
	
	//CONSTANTS
	const float m_transparecyLevel = 0.8f;

};

Box::Box(glm::vec3 pos, glm::vec3 size, glm::vec4 colour)
{
	setSize(size);
	setPosition(pos);
	setColour(colour);
}

void Box::transperice() { m_Colour = { m_Colour.x, m_Colour.y, m_Colour.z, m_transparecyLevel }; }
void Box::solidify() { m_Colour = {m_Colour.x, m_Colour.y, m_Colour.z, 1.f }; }
void Box::onRender() {
	//engine::Renderer::drawQuad(m_Pos ,m_Size ,m_Colour);
	engine::Renderer::draw3DObject(m_Pos,
		{ m_Size },
		{ 0, 0, 0 },
		m_Colour,
		"./assets/models/wall",
		"wall");
}