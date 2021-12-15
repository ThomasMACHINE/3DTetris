#include <engine/engine.h>
#include <blockbuster/include/game-objects/box.h>
#include <blockbuster/include/game-objects/grid.h>
class Player {
public:
	Player();
	
	virtual void onUpdate(engine::Time ts);
	
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
void Player::onUpdate(engine::Time ts) 
{
	//Update time since last update with timestep
	m_lastDrop += ts;
	m_lastInput += ts;

	m_movement = { 0,0,0 };

	if (m_lastDrop > m_updateTimer) 
	{
		m_lastDrop = 0.f;
		m_movement = { 0,0,-1 };
	}
	
	//Set a limit to how often we check for inputs
	if (m_lastInput < m_inputCooldown)
	{ 
		APP_INFO("Stopped multiple input");
		return;  
	}
	//Upwards
	if (engine::Input::isKeyPressed(GLFW_KEY_UP)) 
	{
		m_lastInput = 0.f;
		m_movement = { 0, 1, 0 };
	}
	//Downwards
	if (engine::Input::isKeyPressed(GLFW_KEY_DOWN)) 
	{
		m_lastInput = 0.f;
		m_movement = { 0, -1, 0 };
	}
	//Right
	if (engine::Input::isKeyPressed(GLFW_KEY_RIGHT)) 
	{
		m_lastInput = 0.f;
		m_movement = { 1, 0, 0 };
	}
	//Left
	if (engine::Input::isKeyPressed(GLFW_KEY_LEFT)) 
	{
		m_lastInput = 0.f;
		m_movement = { -1, 0, 0 };
	}
	if (engine::Input::isKeyPressed(GLFW_KEY_SPACE))
	{
		m_lastInput = 0.f;
		m_lastDrop = 0.f;
		m_movement = { 0, 0, -1 };
	}

}


