#include "Application.hpp"

void Application::onInit()
{
	typedef std::chrono::high_resolution_clock c;
	c::time_point beginning = c::now();

	// obtain a seed from the timer
	c::duration d = c::now() - beginning;

	unsigned seed = d.count();

	generator = std::mt19937(seed);   // mt19937 is a standard mersenne_twister_engine

	//Old code to add players -- didnt remove for reference
	/*for (int i = 0; i < 100; i++) {
		int index = addPlayer(V2(5000.0f*generator() / generator.max(), 5000.0f*generator() / generator.max()), sf::Color(100, 0, 0, 255), 2.0f*generator() / generator.max());
	}*/
}

void Application::onBeforeUpdate(V2 pos)
{
	//Old code to manage players -- didnt remove for reference
	//for (int i = 1; i < 101; i++) {
	//	setPlayer(i, V2(5000.0f*generator() / generator.max(),
	//		5000.0f*generator() / generator.max()));
	//}
}







