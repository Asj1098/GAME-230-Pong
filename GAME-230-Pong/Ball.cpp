#include "Ball.h"

Ball::Ball(float _radius = 20.0f, sf::Vector2f _initial_position = sf::Vector2f(600.0f, 450.0f), sf::Color _color = sf::Color::White)
{
	shape.setRadius(_radius);
	shape.setPosition(_initial_position);
	shape.setFillColor(_color);
};

void Ball::ApplyVelocity(float _speed, sf::Vector2f _direction)
{
    velocity.x = _direction.x * _speed;
    velocity.y = _direction.y * _speed;
    speed = _speed;
}

void Ball::ApplyCollisionVelocity(sf::Vector2f direction, float speed_increment = 0.0f)
{
    speed += speed_increment;
    
    //Cap the speed
    if (speed > max_speed)
        speed = max_speed;

    //Calculate magnitude of direction vector
    float dir_mag = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
    
    velocity.x = direction.x * speed / dir_mag;
    velocity.y = direction.y * speed / dir_mag;
}

void Ball::UpdatePosition(float delta_time, sf::Vector2u window_size)
{
    //update ball position
    sf::Vector2f position = shape.getPosition();
    position.x += velocity.x * delta_time;
    position.y += velocity.y * delta_time;
    shape.setPosition(position);

    /*
    if (position.x + shape.getRadius() * 2.0f >= window_size.x)
        velocity.x = -abs(velocity.x);
    if (position.x <= 0.0f)
        velocity.x = abs(velocity.x);
    */

    if (position.y + shape.getRadius() * 2.0f >= window_size.y)
        velocity.y = -abs(velocity.y);
    if (position.y <= 0.0f)
        velocity.y = abs(velocity.y);
}

void Ball::Reset(sf::Vector2f position)
{
    shape.setPosition(position);
    velocity = sf::Vector2f(0.0f, 0.0f);
    speed = 0.0f;
}

void Ball::Reflect(int direction)
{
    velocity.x = direction * abs(velocity.x);
}

void Ball::Repel(float y_repel)
{
    velocity.y += y_repel;
}