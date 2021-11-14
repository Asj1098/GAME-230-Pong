#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>

#include "Ball.h"
#include "Paddle.h"
#include "Extras.h"


int main()
{

    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(1200, 900), "Pong");

    //set up V-Sync
    window.setVerticalSyncEnabled(true);

    //Load game logo (image)
    sf::Texture logo_texture;
    if (!logo_texture.loadFromFile("A:/My Folder/Masters/Courses/GAME 230/GAME-230-Pong/GAME-230-Pong/Images/Pong.png"))
    {
        std::cout << "Image not loaded\n";
        return EXIT_FAILURE;
    }
    sf::Sprite logo;
    logo.setTexture(logo_texture);
    logo.setPosition(250.0f, 20.0f);


    //Sounds and Music
    sf::SoundBuffer point_buffer;
    if(!point_buffer.loadFromFile("A:/My Folder/Masters/Courses/GAME 230/GAME-230-Pong/GAME-230-Pong/Music/ding.wav"))
        return EXIT_FAILURE;
    sf::Sound point_sound;
    point_sound.setBuffer(point_buffer);

    //Font loading
    sf::Font font;
    if (!font.loadFromFile("A:/My Folder/Masters/Courses/GAME 230/GAME-230-Pong/GAME-230-Pong/Fonts/Roboto-Regular.ttf"))
    {
        std::cout << "Font not loaded\n";
        return EXIT_FAILURE;
    }

    //Text set up
    sf::Text game_text;
    game_text.setFont(font);
    game_text.setString("Pick a game mode\nPlayer vs AI     [ Enter A ]\nTwo Players    [ Enter B ]\nThree Players  [ Enter C ]");
    game_text.setCharacterSize(75.0f);
    game_text.setFillColor(sf::Color::White);
    game_text.setPosition(sf::Vector2f(200.0f, 400.0f));

    sf::Text left_score_text;
    left_score_text.setFont(font);
    left_score_text.setString("0");
    left_score_text.setCharacterSize(100.0f);
    left_score_text.setFillColor(sf::Color::Blue);
    left_score_text.setPosition(sf::Vector2f(window.getSize().x / 4, 100.0f));

    sf::Text right_score_text;
    right_score_text.setFont(font);
    right_score_text.setString("0");
    right_score_text.setCharacterSize(100.0f);
    right_score_text.setFillColor(sf::Color::Red);
    right_score_text.setPosition(sf::Vector2f(3 * window.getSize().x / 4, 100.0f));

    //Setup the Ball
    Ball ball(20.0f, sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2), sf::Color::White);
    sf::Vector2f _initial_ball_velocity_direction = sf::Vector2f(1.0f, 0.0f);
    float _initial_ball_speed = 400.0f;

    //Set up Paddles
    float _paddle_speed = 750.0f;
    float _paddle_offset_x = 30.0f;
    sf::Vector2f _paddle_size = sf::Vector2f(25.0f, 150.0f);
    Paddle player1(_paddle_size, sf::Vector2f(window.getSize().x - _paddle_size.x - _paddle_offset_x, window.getSize().y / 2 - _paddle_size.y / 2), _paddle_speed, -1, sf::Color::Red);
    Paddle player2(_paddle_size, sf::Vector2f(_paddle_offset_x, window.getSize().y / 2 - _paddle_size.y / 2), _paddle_speed, 1, sf::Color::Blue);
    Paddle player3(_paddle_size, sf::Vector2f(_paddle_offset_x, window.getSize().y / 2 + _paddle_size.y), _paddle_speed, 1, sf::Color::Green);
        
    //Clock to count the time taken for a frame
    sf::Clock clock;

    //Extras
    sf::RectangleShape obstacle;
    sf::Vector2f obstacle_position;
    float obstacle_speed = 300.0f;
    sf::CircleShape black_hole;
    //sf::CircleShape power_up;
    bool has_power_up_spawned = false;
    int powerup_frame_counter = 0;
    PowerUp powerup(sf::Vector2f(200.0f, 200.0f), sf::Vector2f(window.getSize().x - 200.0f, window.getSize().y - 200.0f), 30.0f, sf::Color::Magenta);
    bool apply_power_up = false;

    RESTART:
    //Reset Game Stuff
    int left_points = 0;
    int right_points = 0;
    int points_to_win = 5;
    float speed_increment = 50.0f;
    int winner = 0;
    bool game_over = false;

    bool game_started = false;
    bool has_second_player = false;
    bool has_third_player = false;
    bool ai_enabled = true;

    //Core SFML
    game_text.setString("Pick a game mode\nPlayer vs AI     [ Enter A ]\nTwo Players    [ Enter B ]\nThree Players  [ Enter C ]");
    right_score_text.setString("0");
    left_score_text.setString("0");
    ball.Reset(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    player1.Reset();
    player2.Reset();
    player2.SetPaddleSpeed(_paddle_speed);
    player1.shape.setSize(_paddle_size);

    //extras
    obstacle.setSize(sf::Vector2f(40.0f, 100.0f));
    obstacle.setPosition(window.getSize().x / 2 - obstacle.getSize().x / 2, 0.0f);
    obstacle.setFillColor(sf::Color::Yellow);
    black_hole.setRadius(50.0f);
    black_hole.setPosition(window.getSize().x / 2 - black_hole.getRadius(), window.getSize().y / 2 - black_hole.getRadius());
    black_hole.setOutlineThickness(15.0f);
    black_hole.setFillColor(sf::Color::Blue);
    black_hole.setOutlineColor(sf::Color::Cyan);
    //power_up.setFillColor(sf::Color::Magenta);
    //power_up.setRadius(30.0f);
    has_power_up_spawned = false;
    apply_power_up = false;
    int time_to_spawn_powerup = 400;
    Obstacle wall_left(sf::Vector2f(10.0f, window.getSize().y), sf::Vector2f(0.0f, 0.0f), sf::Color::Blue);
    Obstacle wall_right(sf::Vector2f(10.0f, window.getSize().y), sf::Vector2f(window.getSize().x - 10.0f, 0.0f), sf::Color::Red);


    window.clear();
    window.draw(logo);
    window.draw(game_text);
    window.display();

    //AI
    int ai_frame = 0;
    int ai_vertical_direction = 0;

    while (window.isOpen())
    {
        //Update clock
        float delta_time = clock.getElapsedTime().asSeconds();
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (game_over)
        {
            window.clear();
            window.draw(left_score_text);
            window.draw(right_score_text);
            window.draw(game_text);
            window.draw(player1.shape);
            window.draw(player2.shape);
            window.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                goto RESTART;

            continue;
        }

        if (!game_started)
        {
            //Player vs AI
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                window.clear();
                game_started = true;
                ball.ApplyVelocity(_initial_ball_speed, _initial_ball_velocity_direction);
                has_second_player = false;
                has_third_player = false;
                ai_enabled = true;
                player2.SetPaddleSpeed(_paddle_speed * 0.75f);
            }

            //Two player mode
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B))
            {
                window.clear();
                game_started = true;
                ball.ApplyVelocity(_initial_ball_speed, _initial_ball_velocity_direction);
                has_second_player = true;
                has_third_player = false;
                ai_enabled = false;
            }

            //Three player mode
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
            {
                window.clear();
                game_started = true;
                ball.ApplyVelocity(_initial_ball_speed, _initial_ball_velocity_direction);
                has_second_player = true;
                has_third_player = true;
                ai_enabled = false;
                player2.shape.setPosition(_paddle_offset_x, window.getSize().y / 2 - _paddle_size.y);
                player1.shape.setSize(sf::Vector2f(_paddle_size.x, _paddle_size.y * 1.5));
            }
        }
        else
        {
            //Player 1 Input
            int _vertical_direction = 0;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                _vertical_direction -= 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                _vertical_direction += 1;

            player1.UpdatePosition(delta_time, _vertical_direction, window.getSize().y);
            
            if (has_second_player)
            {
                _vertical_direction = 0;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    _vertical_direction -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                    _vertical_direction += 1;
                player2.UpdatePosition(delta_time, _vertical_direction, window.getSize().y);
            }

            if (has_third_player)
            {
                _vertical_direction = 0;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U))
                    _vertical_direction -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J))
                    _vertical_direction += 1;
                player3.UpdatePosition(delta_time, _vertical_direction, window.getSize().y);
            }

            //Simple AI
            if (ai_enabled)
            {
                ai_frame++;
                if (ai_frame >= 15)
                {
                    ai_frame = 0;
                    if (ball.shape.getPosition().y + ball.shape.getRadius() > player2.shape.getPosition().y + player2.shape.getSize().y / 2)
                    {
                        int r = rand() % 100;

                        if (r < 70)
                            ai_vertical_direction = 1;
                        else if(r>70 && r<90)
                            ai_vertical_direction = 0;
                        else
                            ai_vertical_direction = -1;
                    }
                    else if (ball.shape.getPosition().y + ball.shape.getRadius() < player2.shape.getPosition().y + player2.shape.getSize().y / 2)
                    {
                        int r = rand() % 100;
                        
                        if (r < 70)
                            ai_vertical_direction = -1;
                        else if (r > 70 && r < 90)
                            ai_vertical_direction = 0;
                        else
                            ai_vertical_direction = 1;
                    }
                }
                player2.UpdatePosition(delta_time, ai_vertical_direction, window.getSize().y);
            }

            //Extras
            {
                auto o_position = obstacle.getPosition();
                if (o_position.y < 0)
                    obstacle_speed = abs(obstacle_speed);

                if (o_position.y + obstacle.getSize().y > window.getSize().y)
                    obstacle_speed = -abs(obstacle_speed);
                
                o_position.y += obstacle_speed * delta_time;
                obstacle.setPosition(o_position);
                
                if (obstacle.getGlobalBounds().intersects(ball.shape.getGlobalBounds()))
                {
                    if (ball.shape.getPosition().x <= o_position.x)
                        ball.Reflect(-1);
                    else
                        ball.Reflect(1);
                }
                if (black_hole.getGlobalBounds().intersects(ball.shape.getGlobalBounds()))
                {
                    float delta_y = black_hole.getPosition().y - ball.shape.getPosition().y;
                    ball.Repel(delta_y/2);
                }

                powerup_frame_counter++;
                if (powerup_frame_counter % time_to_spawn_powerup == 0)
                {
                    if (!powerup.is_active)
                    {
                        powerup.Spawn();
                        time_to_spawn_powerup = 400 + (rand() % 200);
                    }
                    powerup_frame_counter = 0;
                }
                if (powerup.is_active)
                {
                    if (powerup.shape.getGlobalBounds().intersects(ball.shape.getGlobalBounds()))
                    {
                        powerup.Despawn();
                        apply_power_up = true;
                    }
                }
                if (wall_left.is_active)
                {
                    if (wall_left.shape.getGlobalBounds().intersects(ball.shape.getGlobalBounds()))
                    {
                        ball.Reflect(1);
                        wall_left.is_active = false;
                    }
                }
                if (wall_right.is_active)
                {
                    if (wall_right.shape.getGlobalBounds().intersects(ball.shape.getGlobalBounds()))
                    {
                        ball.Reflect(-1);
                        wall_right.is_active = false;
                    }
                }
            }
            
            //Check for collisions
            if (player1.shape.getGlobalBounds().intersects(ball.shape.getGlobalBounds()))
            {
                float ymultiplier = (ball.shape.getPosition().y - ball.shape.getRadius()) - (player1.shape.getPosition().y + player1.shape.getSize().y / 2);
                ball.ApplyCollisionVelocity(sf::Vector2f(player1.hit_dir, ymultiplier / 100), speed_increment);

                if (apply_power_up)
                {
                    wall_right.is_active = true;
                    apply_power_up = false;
                }
            }

            if (player2.shape.getGlobalBounds().intersects(ball.shape.getGlobalBounds()))
            {
                float ymultiplier = (ball.shape.getPosition().y - ball.shape.getRadius()) - (player2.shape.getPosition().y + player2.shape.getSize().y / 2);
                ball.ApplyCollisionVelocity(sf::Vector2f(player2.hit_dir, ymultiplier / 100), speed_increment);

                if (apply_power_up)
                {
                    wall_left.is_active = true;
                    apply_power_up = false;
                }
            }

            if (has_third_player)
            {
                if (player3.shape.getGlobalBounds().intersects(ball.shape.getGlobalBounds()))
                {
                    float ymultiplier = (ball.shape.getPosition().y - ball.shape.getRadius()) - (player3.shape.getPosition().y + player3.shape.getSize().y / 2);
                    ball.ApplyCollisionVelocity(sf::Vector2f(player3.hit_dir, ymultiplier / 100), speed_increment);
                }
                if (apply_power_up)
                {
                    wall_left.is_active = true;
                    apply_power_up = false;
                }
            }

            ball.UpdatePosition(delta_time, window.getSize());

            //Check if a team scored

            if (ball.shape.getPosition().x > window.getSize().x + 20.0f)
            {
                //Right side scored a point
                left_points++;
                left_score_text.setString(std::to_string(left_points));
                ball.Reset(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
                point_sound.play();
                apply_power_up = false;

                if (left_points >= points_to_win)
                {
                    winner = -1;
                    game_text.setString("Left Side Wins!\nPress Space to play again");
                    game_over = true;
                }
                else
                {
                    int i = 1;
                    if (rand() % 100 < 50)
                        i = -1;
                    ball.ApplyVelocity(i* _initial_ball_speed, _initial_ball_velocity_direction);
                }
            }
            if (ball.shape.getPosition().x < -20.0f)
            {
                //Left side scored a point
                right_points++;
                right_score_text.setString(std::to_string(right_points));
                ball.Reset(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
                point_sound.play();
                apply_power_up = false;

                if (right_points >= points_to_win)
                {
                    winner = 1;
                    game_text.setString("Right Side Wins!\nPress Space to play again");
                    game_over = true;
                }
                else
                {
                    int i = 1;
                    if (rand() % 100 < 50)
                        i = -1;
                    ball.ApplyVelocity(i * _initial_ball_speed, _initial_ball_velocity_direction);
                }

            }

            window.clear();
            window.draw(left_score_text);
            window.draw(right_score_text);
            window.draw(black_hole);
            window.draw(ball.shape);
            window.draw(player1.shape);
            window.draw(player2.shape);
            if(has_third_player)
                window.draw(player3.shape);
            window.draw(obstacle);
            if (powerup.is_active)
                window.draw(powerup.shape);
            if (wall_left.is_active)
                window.draw(wall_left.shape);
            if (wall_right.is_active)
                window.draw(wall_right.shape);
            window.display();
        }
    }

    return 0;
}
