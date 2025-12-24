#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <complex>


// < 1 or weird
const float x_stretch = 0.5;

void function(sf::VertexArray& line, sf::Vector2u size, float unitsPerPixel, float xOffset, float yOffset){
    float originY = static_cast<float>(size.y / 2.0);
    float originX = static_cast<float>(size.x / 2.0);
    float BigVal = 9999.0;
    float SmallVal = -9999.0;
    if (unitsPerPixel <= 0.03f){
        unitsPerPixel = 0.03f;
    }

    for (float x = 0; x < size.x; ++x) {
        float math_x = (x - originX) * unitsPerPixel * x_stretch + xOffset;
        //equation-----------
        // float math_y = math_x * math_x + 4*math_x + 9
        float math_y = 10.0 * cos(math_x);
        //-------------------
        float screen_y = originY - (math_y / unitsPerPixel) + yOffset;
        if (screen_y < static_cast<float>(size.y) && screen_y > 0.0){
            line[x].position = sf::Vector2f(x,screen_y);
            line[x].color = sf::Color::White;
            
        } else if (screen_y <= 0.0) {
            line[x].position = sf::Vector2f(x,SmallVal);
            line[x].color = sf::Color::White;       
        } else {
            line[x].position = sf::Vector2f(x,BigVal);
            line[x].color = sf::Color::White;             
        }
    }

}

void mandlebrot(sf::VertexArray& line, sf::Vector2u size, float unitsPerPixel,
                float xOffset,
                float yOffset)
{
    const int maxDepth = 200;

    const int width  = static_cast<int>(size.x);
    const int height = static_cast<int>(size.y);

    line.setPrimitiveType(sf::PrimitiveType::Points);
    line.resize(width * height);

    const float originX = size.x / 2.f;
    const float originY = size.y / 2.f;

    if (unitsPerPixel < 0.0005f)
        unitsPerPixel = 0.0005f;

    for (int py = 0; py < height; ++py)
    {
        for (int px = 0; px < width; ++px)
        {
            float x = (px - originX) * unitsPerPixel + xOffset;
            float y = (originY - py) * unitsPerPixel + yOffset;

            std::complex<float> z = 0.0f;
            std::complex<float> c(x, y);

            int iter = 0;
            for (; iter < maxDepth; ++iter)
            {
                z = z * z + c;
                // not a real norm. squared values of real and imaginary values
                // circle of radius 2 around origin that quantifies escape radius
                if (std::norm(z) > 4.0f)
                    break;
            }

            int index = py * width + px;

            line[index].position = sf::Vector2f(
                static_cast<float>(px),
                static_cast<float>(py)
            );

            if (iter == maxDepth)
            {
                line[index].color = sf::Color::Black;
            }
            else
            {
                line[index].color = sf::Color::White;
            }
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800u, 800u}), "Graph");

    // // Right-side control panel background
    // const float panelWidth = 250.f;
    // sf::RectangleShape panel;
    // panel.setSize({panelWidth, 600});
    // panel.setPosition({750, 0});
    // panel.setFillColor(sf::Color(30, 30, 30));
    bool mandlebrotGraph = false;

    sf::Vector2u size = window.getSize();
    printf("%d\n",size.x);
    float unitsPerPixel = 1.0f;
    float xOffset = 0.0;
    float yOffset = 0.0;
    // sf::VertexArray line(sf::PrimitiveType::LineStrip, size.x);
    
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
                    unitsPerPixel -= .03f;
                } 
                if (delta == -1){
                    unitsPerPixel += .03f;
                }
            } else if (event->is<sf::Event::KeyPressed>()){
                const auto& key = event->getIf<sf::Event::KeyPressed>();
                // std::cout << static_cast<int>(key->code) << std::endl;
                if (key->scancode == sf::Keyboard::Scan::Z){
                    unitsPerPixel -= .03f;
                }
                if (key->scancode == sf::Keyboard::Scan::X){
                    unitsPerPixel += .03f;
                }
                //left
                if (static_cast<char>(key->scancode) == 'W'){
                    if (mandlebrotGraph) {
                        xOffset-=0.5;
                    } else {
                        xOffset-=5.0;
                    }
                }
                //right
                if (static_cast<char>(key->scancode) == 'V'){
                    if (mandlebrotGraph) {
                        xOffset+=0.5;
                    } else {
                        xOffset+=5.0;
                    }
                }
                if (static_cast<char>(key->scancode) == 'X'){
                    if (mandlebrotGraph) {
                        yOffset-=0.5;
                    } else {
                        yOffset-=10.0;
                    }
                }
                if (static_cast<char>(key->scancode) == 'Y'){
                    if (mandlebrotGraph) {
                        yOffset+=0.5;
                    } else {
                        yOffset+=10.0;
                    }
                    
                }
                //c = switch to function
                if (static_cast<int>(key->code)==2){
                    if (mandlebrotGraph){
                        mandlebrotGraph = false;
                    } else {
                        mandlebrotGraph = true;
                    }
                    
                }
            
            }

        }

        if (mandlebrotGraph){
            unitsPerPixel = 0.005;
            sf::VertexArray line(sf::PrimitiveType::Points, size.x);
            mandlebrot(line,size,unitsPerPixel,xOffset,yOffset);
            window.clear();
            // window.draw(panel);
            window.draw(line);
        } else {
            sf::VertexArray line(sf::PrimitiveType::LineStrip, size.x);
            function(line,size,unitsPerPixel,xOffset,yOffset);
            window.clear();
            // window.draw(panel);
            window.draw(line);
        }
        window.display();
    }
}
