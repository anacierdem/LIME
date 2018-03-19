//
// Created by anacierdem on 6/23/16.
//

#pragma once

//Dependencies
#include <random>
#include <chrono>

#include "../Renderer/GameRenderer.hpp"
#include "../Misc/VectorExtension.hpp"

// General control logic to manipulate engine state. Originally was intended for
// running high-level game logic.
class Application : public GameRenderer {
public:
	Application() : GameRenderer() {};

	void onBeforeUpdate(V2 pos) override;
	void onInit() override;
	std::mt19937 generator;
};