#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

// Adds basic vector operations to SFML vectors
class V2 : public sf::Vector2<float>{

public:

	V2() : sf::Vector2<float>() {};
	V2(float x, float y) : sf::Vector2<float>(x, y) {};
	V2(const sf::Vector2<float>& vector) : sf::Vector2<float>(vector){};


    friend float operator*(V2 lhs,        // passing lhs by value helps optimize chained a+b+c
                       const V2& rhs) // otherwise, both parameters may be const references
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

	float length() const {
		return std::sqrt(x*x + y*y);
	};

	V2 normalized() const {
		return (*this / length());
	};
};