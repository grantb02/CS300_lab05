#include "screenSaver.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <cmath>

//Self-explanatory
ScreenSaver::ScreenSaver() {
  shape = sf::CircleShape(10.f);
  velocity = sf::Vector2f(0.0f, 0.0f);
  shape.setFillColor(sf::Color::White);
}

//Self-explanatory
ScreenSaver::ScreenSaver(float radius, sf::Vector2f cVelocity, sf::Color color) {
  shape = sf::CircleShape(radius);
  velocity = cVelocity;
  shape.setFillColor(color);
}

//We have to return shape as a reference!
sf::CircleShape &ScreenSaver::getShape() { return shape; }

//this will draw the sf::CircleShape member of ScreenSaver to the screen
//this will be inherited by all derived class, and shouldn't have to be overriden
void ScreenSaver::draw(sf::RenderWindow &window) { window.draw(getShape()); }

/*Here's some nifty code to resolve any collsions with a wall, by reversing
the velocity of the object to mimic bouncing, you can call this function
in your update functions to make sure your ScreenSaver doesn't wander off-screen*/
void ScreenSaver::checkWallCollision(int screenWidth, int screenHeight){
  sf::Vector2f position = getShape().getPosition();
  float diameter = getShape().getRadius() * 2;

  // Check if the circle hits the right wall
  if (position.x + diameter > screenWidth)
  {
    getShape().setPosition(screenWidth - diameter, position.y);
    velocity.x *= -1.f;
  }

  // Check if the circle hits the left wall
  if (position.x < 0)
  {
    getShape().setPosition(0, position.y);
    velocity.x *= -1.f;
  }

  // Check if the circle hits the top wall
  if (position.y < 0)
  {
    getShape().setPosition(position.x, 0);
    velocity.y *= -1.f;
  }

  // Check if the circle hits the bottom wall
  if (position.y + diameter > screenHeight)
  {
    getShape().setPosition(position.x, screenHeight - diameter);
    velocity.y *= -1.f;
  }
}

// ClassicSaver class inherits from ScreenSaver
class ClassicSaver : public ScreenSaver {
public:
    // Constructor to initialize the ClassicSaver with radius, velocity, and color
    ClassicSaver(float radius, sf::Vector2f velocity, sf::Color color)
        : ScreenSaver(radius, velocity, color) {
        // Set initial position if needed
        shape.setPosition(100.f, 100.f);  // You can start it at any point
    }

    // Override the update function to move the saver and check wall collisions
    void update(float deltaTime, int screenWidth, int screenHeight) override {
        // Scale velocity by deltaTime to ensure smooth movement independent of frame rate
        shape.move(velocity * deltaTime);

        // Check for wall collisions using the provided function
        checkWallCollision(screenWidth, screenHeight);
    }
};

sf::Color transitionColor(float time, float speed) {
    // Use sine and cosine functions to smoothly transition between RGB colors
    int r = static_cast<int>((std::sin(speed * time) + 1.0f) * 127.5f); // Range [0, 255]
    int g = static_cast<int>((std::sin(speed * time + 2.0f) + 1.0f) * 127.5f); // Slight offset for variation
    int b = static_cast<int>((std::sin(speed * time + 4.0f) + 1.0f) * 127.5f); // Slight offset for variation
    return sf::Color(r, g, b);
}

class ColorChangingSaver : public ClassicSaver {
public:
    // Constructor with radius, velocity, and color speed for transitions
    ColorChangingSaver(float radius, sf::Vector2f velocity, float colorSpeed)
        : ClassicSaver(radius, velocity, sf::Color::White), colorSpeed(colorSpeed), elapsedTime(0.f) {}

    // Override the update function to include color changing and movement
    void update(float deltaTime, int screenWidth, int screenHeight) override {
        // Call the parent class's update to handle movement and wall collisions
        ClassicSaver::update(deltaTime, screenWidth, screenHeight);

        // Update elapsed time
        elapsedTime += deltaTime;

        // Smoothly transition the color based on elapsed time and colorSpeed
        sf::Color newColor = Color(elapsedTime, colorSpeed);

        // Apply the new color to the shape
        getShape().setFillColor(newColor);
    }

private:
    float colorSpeed;   // Determines how fast the color transitions occur
    float elapsedTime;  // Keeps track of the time passed to compute color transitions
};

// CustomSaver class - Moves in a spiral path and grows/shrinks in size
class CustomSaver : public ScreenSaver {
public:
    // Constructor with arbitrary float values
    CustomSaver(float radius, sf::Vector2f startVelocity, float spiralSpeed)
        : ScreenSaver(radius, startVelocity, sf::Color::Cyan), spiralSpeed(spiralSpeed), timeElapsed(0.f) {}

    // Override the update function to handle spiral movement and size modulation
    void update(float deltaTime, int screenWidth, int screenHeight) override {
        timeElapsed += deltaTime;  // Keep track of time

        // Spiral movement: Update the position based on time (circular with increasing radius)
        float radius = initialRadius + spiralSpeed * timeElapsed;  // Gradually increase radius
        float angle = spiralSpeed * timeElapsed;  // Angle increases with time

        // Calculate new position using polar coordinates (r, theta)
        float x = screenWidth / 2 + radius * std::cos(angle);  // Horizontal spiral movement
        float y = screenHeight / 2 + radius * std::sin(angle); // Vertical spiral movement
        getShape().setPosition(x, y);

        // Size modulation: Change the size of the shape dynamically
        float newSize = 20.f + 10.f * std::sin(spiralSpeed * timeElapsed);  // Oscillate size
        getShape().setRadius(newSize);  // Update radius of the shape
        getShape().setOrigin(newSize, newSize);  // Ensure correct positioning

        // Ensure that the saver always stays within the bounds of the screen
        checkWallCollision(screenWidth, screenHeight);
    }

private:
    float spiralSpeed;    // Speed at which the saver spirals out
    float timeElapsed;    // Elapsed time to control movement and size modulation
    const float initialRadius = 50.f;  // Initial radius for the spiral path
};
//TODO Define all necessary member functions for ClassicSaver, ColorChangingSaver, and CustomSaver Here