#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

class PowerUp
{
	sf::Vector2f min_pos;
	sf::Vector2f max_pos;
	float radius = 30.0f;

public:
	bool is_active = false;
	sf::CircleShape shape;
	PowerUp(sf::Vector2f _min_pos, sf::Vector2f _max_pos, float _radius, sf::Color _color);
	void Spawn();
	void Despawn();
};

class Obstacle
{
public:
	sf::RectangleShape shape;
	bool is_active = false;
	Obstacle(sf::Vector2f _size, sf::Vector2f _position, sf::Color _color);
};