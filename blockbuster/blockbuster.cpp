#include <engine/engine.h>

//#include "pacman/include/layers/sandbox-layer.h"
#include "blockbuster/include/layers/gameLayer.h"

class PacmanGame : public engine::AppFrame {
public:
	PacmanGame() : AppFrame() {
		//pushLayer(NEW ExampleLayer());
		pushLayer(NEW GameLayer);
	}


	PacmanGame(engine::WindowSpecs specs) : AppFrame(specs) {
		setAppIcon("assets/textures/desktop-icon.png");	// Window icon

		pushLayer(NEW GameLayer);
	}


	~PacmanGame() {

	}
};

/*
	Returns a new application sandbox defined and executed engine side.
*/
engine::u_Ptr<engine::AppFrame> engine::createApp() {
	auto windowSpecs = engine::WindowSpecs("Blockbuster", 1600, 900);

	return engine::m_UPtr<PacmanGame>(windowSpecs);
}