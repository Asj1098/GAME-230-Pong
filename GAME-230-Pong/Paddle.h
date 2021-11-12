#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

class Paddle
{
private:
	float speed;
	sf::Vector2f reset_position;
public:
	sf::RectangleShape shape;
	int hit_dir;

	Paddle(sf::Vector2f _size, sf::Vector2f initial_position, float _speed, int _hit_dir, sf::Color _color);
	void UpdatePosition(float delta_time, int vertical_direction, float y_bounds);
	void SetPaddleSpeed(float _speed);
	void Reset();
};
