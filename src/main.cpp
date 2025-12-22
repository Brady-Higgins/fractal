#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

// Updates an array of vertex points to map the mandlebrot set for a given x min and x max
// sf::Vector2u size : represents the max x width of the screen. So size samples
// top left is 0,0 and bottom right is max,max
// void mandlebrot(sf::VertexArray& line, sf::Vector2u size){
//     float originY = static_cast<float>(size.y / 2.0);
//     float originX = static_cast<float>(size.x / 2.0);
//     float BigVal = 9999.0;
//     float SmallVal = -9999.0;
//     bool big;
//     float unitsPerPixel = 0.05f;
//     for (float x = 0; x < size.x; ++x) {
//         // relative to origin
//         float function_x = (static_cast<float>(x) - originX);
//         //equation-----------
//         float function_y = function_x * function_x + 3.0 * function_x + 9.0;
//         //-------------------
//         float relative_y = (originY - function_y);
//         if (relative_y < static_cast<float>(size.y) && relative_y > 0.0){
//             printf("%f,%f\n",x,function_y);
//             line[x].position = sf::Vector2f(x,relative_y);
//             line[x].color = sf::Color::White;
            
//         } else if (relative_y < 0.0) {
//             line[x].position = sf::Vector2f(x,SmallVal);
//             line[x].color = sf::Color::White;       
//         } else {
//             line[x].position = sf::Vector2f(x,BigVal);
//             line[x].color = sf::Color::White;             
//         }
//     }

// }

void mandlebrot(sf::VertexArray& line, sf::Vector2u size, float unitsPerPixel){
    float originY = static_cast<float>(size.y / 2.0);
    float originX = static_cast<float>(size.x / 2.0);
    float BigVal = 9999.0;
    float SmallVal = -9999.0;
    bool big;
    // float unitsPerPixel = 0.03f;
    for (float x = 0; x < size.x; ++x) {
        // relative to origin
        float function_x = (static_cast<float>(x) - originX);
        float scaled_x = function_x * unitsPerPixel;
        //equation-----------
        float function_y = scaled_x * scaled_x + 3.0 * scaled_x + 9.0;
        //-------------------
        int scaled_y = static_cast<int>(function_y / unitsPerPixel);
        float relative_y = (originY - scaled_y);
        if (relative_y < static_cast<float>(size.y) && relative_y > 0.0){
            line[x].position = sf::Vector2f(x,relative_y);
            line[x].color = sf::Color::White;
            
        } else if (relative_y < 0.0) {
            line[x].position = sf::Vector2f(x,SmallVal);
            line[x].color = sf::Color::White;       
        } else {
            line[x].position = sf::Vector2f(x,BigVal);
            line[x].color = sf::Color::White;             
        }
    }

}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800u, 800u}), "Graph");

    sf::Vector2u size = window.getSize();
    printf("%d\n",size.x);
    float unitsPerPixel = 1.0f;
    sf::VertexArray line(sf::PrimitiveType::LineStrip, size.x);
    while (window.isOpen())
    {
        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            } else if (event->is<sf::Event::MouseWheelScrolled>()){
                const auto& scroll = event->getIf<sf::Event::MouseWheelScrolled>();
                // -1 : scroll down   +1 : scroll up
                float delta = scroll->delta;
                if (delta == 1){
                    unitsPerPixel -= .01f;
                } 
                if (delta == -1){
                    unitsPerPixel += .01f;
                }
            }
        }
        
        mandlebrot(line,size,unitsPerPixel);

        window.clear();
        window.draw(line);
        window.display();
    }
}
