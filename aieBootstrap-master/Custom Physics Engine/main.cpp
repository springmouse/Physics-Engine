#include "Custom_Physics_EngineApp.h"

int main() {
	
	// allocation
	auto app = new Custom_Physics_EngineApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}