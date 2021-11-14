#include "Extras.h"

PowerUp::PowerUp(sf::Vector2f _min_pos, sf::Vector2f _max_pos, float _radius, sf::Color _color) :min_pos(_min_pos), max_pos(_max_pos), radius(_radius)
{
	shape.setFillColor(_color);
	is_active = false;
}

void PowerUp::Spawn()
{
	int x_pos = rand() % (int)max_pos.x;
	int y_pos = rand() % (int)max_pos.y;

	if (x_pos < min_pos.x)
		x_pos = (int)min_pos.x;
	if (y_pos < min_pos.y)
		y_pos = (int)min_pos.y;

	is_active = true;
	shape.setRadius(radius);
	shape.setPosition(sf::Vector2f(x_pos, y_pos));
}

void PowerUp::Despawn()
{
	shape.setRadius(0.0f);
	is_active = false;
}

Obstacle::Obstacle(sf::Vector2f _size, sf::Vector2f _position, sf::Color _color)
{
	shape.setSize(_size);
	shape.setPosition(_position);
	shape.setFillColor(_color);
	is_active = false;
}