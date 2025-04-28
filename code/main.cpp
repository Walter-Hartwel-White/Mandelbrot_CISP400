#include <iostream>
#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

using namespace sf;

int main()
{
    int computerSavior = 1;
    VideoMode vm(sf::VideoMode::getDesktopMode().width / computerSavior, VideoMode::getDesktopMode().height / computerSavior);

    RenderWindow window(vm, "wowza", Style::Default);

    ComplexPlane plane(VideoMode::getDesktopMode().width / computerSavior, VideoMode::getDesktopMode().height / computerSavior);
    Vector2i coor;

    //text objects
    Font font;
    font.loadFromFile("fonts/lucon.ttf");

    Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setPosition(10, 10);

    //
    while (window.isOpen())
    {
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        Event event;
        while (window.pollEvent(event))
        {
            coor.x = event.mouseButton.x;
            coor.y = event.mouseButton.y;

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left) {
                    plane.setCenter(coor);
                    plane.zoomIn();
                }
                if (event.mouseButton.button == Mouse::Right) {
                    plane.setCenter(coor);
                    plane.zoomOut();
                }
            }
            coor.y = event.mouseMove.y;
            coor.x = event.mouseMove.x;

            if (event.type == Event::Closed) window.close();
            if (event.type == Event::MouseMoved) plane.setMouseLocation(coor);
        }

        //Update
        
        plane.updateRender();
        plane.loadText(text);
       
        //Draw

        window.clear();

        window.draw(plane);
        window.draw(text);

        window.display();
    }
}   