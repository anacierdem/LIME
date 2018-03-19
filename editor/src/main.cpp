#include "Core/EngineManager.hpp"

//Entry point
int main()
{
	try{
		AlEngine::EngineManager alEngine;
		alEngine.start();
	}
	catch (std::exception e) {
	}

	return 0;
}
