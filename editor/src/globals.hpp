//
// Created by anacierdem on 6/14/16.
//
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#if __APPLE__
//#define BASE_PATH std::string("/Users/alpercinar/Github/desertRPG/editor/Resources/") +
#define BASE_PATH std::string("/Users/alinacierdem/Documents/OtherCode/desertRPG/editor/Resources/") +
#else
#define BASE_PATH std::string("") +
#endif

#define MAX_TILE_COUNT 10000
#define IS_FULLSCREEN false
#define VERTICAL_SYNC false
