#include "Paddle.h"

Paddle::Paddle(sf::Vector2f _size = sf::Vector2f(10.0f, 50.0f), sf::Vector2f initial_position = sf::Vector2f(1170.0f, 450.0f), float _speed = 5.0f, int _hit_dir = -1, sf::Color _color = sf::Color::Red)
{
	shape.setSize(_size);
	shape.setPosition(initial_position);
	shape.setFillColor(_color);
    speed = _speed;
    hit_dir = _hit_dir;
    reset_position = initial_position;
}

void Paddle::UpdatePosition(float delta_time, int vertical_direction, float y_bounds)
{
    sf::Vector2f position = shape.getPosition();

    if (position.y + shape.getSize().y  >= y_bounds && vertical_direction == 1)
        vertical_direction = 0;
    if (position.y <= 0.0f && vertical_direction == -1)
        vertical_direction = 0;

    position.y += vertical_direction * speed * delta_time;
    shape.setPosition(position);
}

void Paddle::SetPaddleSpeed(float _speed)
{
    speed = _speed;
}

void Paddle::Reset()
{
    shape.setPosition(reset_position);
}