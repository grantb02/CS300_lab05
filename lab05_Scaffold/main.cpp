#include "screenSaver.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

/*Represents the Dimensions of the window
Your code should work for any given values here*/
const int SCREENX = 600;
const int SCREENY = 800;

/*Used to fix the maximum framerate to 200 FPS, this results
in smoother, and more "realistic" movement on the screen. However
you may still notice some jittering/lag; if you find a solution to that let me know */
const int MAXFRAMERATE = 200;
const int TIME_PER_FRAME = 1000 / MAXFRAMERATE;

int main()
{
  /*This just uses antialiasing to smooth the edges of shapes
  so they look a little nicer*/
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  /*This window object represents the actual window that will be drawn on your screen
      sf::VideoMode(SCREENX, SCREENY) -> sets the dimensions of the window
      "Screen Saver Demo" -> The name of the window (look at the bar at the top of the window)
      sf::Style ... -> These are options we are passing into the window, we currently give the window the ability to close, and give it a titlebar*/
  sf::RenderWindow window(sf::VideoMode(SCREENX, SCREENY), "Screen Saver Demo", sf::Style::Titlebar | sf::Style::Close);

  /*This is used to measure "delta time" or the time between frames being drawn*/
  sf::Clock deltaClock;
  int deltaTime = 1;

  /*Here is our collection of ScreenSaver-Derived Objects
      We store them as pointers so we can use polymorphic behaviour later on
      If we were store these as normal objects we would encounter the problem of "Object Slicing" */
  std::vector<ScreenSaver*> screenSavers;
  screenSavers.push_back(new ClassicSaver(15.f, sf::Vector2f(.3, .3), sf::Color::Blue));
  screenSavers.push_back(new ColorChangingSaver(20.f, sf::Vector2f(.5, .5), .2));
  screenSavers.push_back(new CustomSaver(12.f, sf::Vector2f(.5, .5), .15));

  /*This is called the "Main Loop" or "Game loop"
      This is where all of our signifgant code is going to be called
      In order to get smooth 2d animation we will follow a simple plan
        1.) Clear the screen (When we go to draw our objects we want a blank canvas)
        2.) Move the various objects we want to draw to where they need to be
        3.) Draw our objects to the screen 
        4.) Display the screen
        5.) Repeat this process for every frame!*/
  while (window.isOpen())
  {
    /*We can consider this boiler-plate, it just makes sure that the window 
    closes when we click the little x in the top right corner*/
    sf::Event event;
    while (window.pollEvent(event))
    {

      if (event.type == sf::Event::Closed)
        window.close();
    }

    /*This stores the amount of time since the last frame was drawn, we can use this 
        number to determine how much we need to move objects across the screen every frame
        Example: if its been 40 milliseconds since the last frame was drawn we may move the object 4 pixels,
        but if its only been 20 milliseconds, then we may move the object 2 pixels. This way the object is 
        always moving 1 pixel every 10 milliseconds, and is independent of framerate which may be different 
        on different machines.
        */
    deltaTime = deltaClock.restart().asMilliseconds();

    /*Here we are calling update on every ScreenSaver-derived object, for each object the definition of this 
        function should be different. Thats the advantage of Polymorphism!*/
    for(ScreenSaver* s : screenSavers){
      /*the update function needs to move the location of the sf::CircleShape Object (found in the parent class ScreenSaver)
          according to the desired behaviour. Make sure that you account for deltaTime in your update function*/
      s->update(deltaTime, SCREENX, SCREENY);
    }

    //Here we are simply erasing everything off the window, so we have a clean slate to draw on. Try commenting this out to see what happens!
    window.clear();
      
    //Here we are drawing all of the ScreenSaver-derived objects, you should not have to override the draw function (but you can for some interesting behaviour)
    for(ScreenSaver* s : screenSavers){
      s->draw(window);
    }

    //Here we are simply displaying everything we just drew to the window
    window.display();

    /*This is how we fix the max Framerate, this is a common practice to
    reduce jittering motion, you shouldnt have to worry about this
    but if you have any questions let me know*/
    float sleepTime = TIME_PER_FRAME - deltaTime;
    if (sleepTime > 0){
      sf::sleep(sf::milliseconds(static_cast<int>(sleepTime)));
    }
  }

  //Dont forget to clean up after yourself
  for(ScreenSaver* s : screenSavers){
    delete s; 
  }

  return 0;
}
