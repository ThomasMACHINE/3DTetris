#include <engine/engine.h>
#include <blockbuster/include/game-objects/box.h>
#include <blockbuster/include/game-objects/grid.h>
class Player {
public:
	Player();
	
	virtual int onUpdate(engine::Time ts);
	
	//Accessor & Mutator methods
	engine::s_Ptr<Box> getActiveBlock() { return m_activeBlock; }
	void setActiveBlock(engine::s_Ptr<Box> newBlock) { m_activeBlock = newBlock; }

	glm::vec3 getMovement() { return m_movement; }
	void resetMovement() { m_movement = { 0,0,0 }; }
private:
	float m_lastDrop;
	float m_lastInput;
	float m_updateTimer = 2.f;
	float m_inputCooldown = 0.7f;
	glm::vec3 m_movement;
	engine::s_Ptr<Box> m_activeBlock;
};

Player::Player()
{
	return;
}
int Player::onUpdate(engine::Time ts) 
{
	//Update time since last update with timestep
	m_lastDrop += ts;
	m_lastInput += ts;

	if (m_lastDrop > m_updateTimer) 
	{
		m_lastDrop = 0.f;
		return 1;
	}
	
	//Game runs too quick so we
	//set a limit to how often we check for inputs
	if (m_lastInput < m_inputCooldown)
	{ 
		return 0;  
	}
	//downwards
	if (engine::Input::isKeyPressed(GLFW_KEY_SPACE))		//	===   Return Codes and their meaning ====
	{														//
		m_lastInput = 0.f;									// 0 - do nothing
		return 1;											// 1 - go downwards
	}														// 2 - go forwards
	//Upwards												// 3 - go backwards
	if (engine::Input::isKeyPressed(GLFW_KEY_UP))    		// 4 - go right	
	{														// 5 - go left
		m_lastInput = 0.f;									// 6 - posYaw
		return 2;											// 7 - negYaw	
	}														// 8 - posRoll	
	//Backwards												// 9 - negRoll	
	if (engine::Input::isKeyPressed(GLFW_KEY_DOWN)) 		//10 - posPitch	
	{														//11 - negPitch
		m_lastInput = 0.f;
		return 3;												
	}															
	//Right														
	if (engine::Input::isKeyPressed(GLFW_KEY_RIGHT)) 			
	{								
		m_lastInput = 0.f;
		return 4;												
	}															
	//Left
	if (engine::Input::isKeyPressed(GLFW_KEY_LEFT)) 
	{
		m_lastInput = 0.f;
		return 5;
	}
	//posYaw
	if (engine::Input::isKeyPressed(GLFW_KEY_E))
	{
		m_lastInput = 0.f;
		return 6;
	}
	if (engine::Input::isKeyPressed(GLFW_KEY_D))
	{
		m_lastInput = 0.f;
		return 7;
	}
	if (engine::Input::isKeyPressed(GLFW_KEY_W))
	{
		m_lastInput = 0.f;
		return 8;
	}
	if (engine::Input::isKeyPressed(GLFW_KEY_S))
	{
		m_lastInput = 0.f;
		return 9;
	}
	if (engine::Input::isKeyPressed(GLFW_KEY_Q))
	{
		m_lastInput = 0.f;
		return 10;
	}
	if (engine::Input::isKeyPressed(GLFW_KEY_Q))
	{
		m_lastInput = 0.f;
		return 11;
	}
	return 0;
}
















