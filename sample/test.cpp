#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

//This function returns vertex array by given number of triangles
sf::VertexArray newShape(int numberOfTriangles) {
    sf::VertexArray shape(sf::Triangles);
    //we are going trough every point in each triangle
    for (int i=0;i<3*numberOfTriangles;i++){
        //creating points of triangles as vertexes 1, 2 and 3
        sf::Vertex v1(sf::Vector2f(rand() % 800, rand() % 600));
        sf::Vertex v2(sf::Vector2f(v1.position.x - 3, v1.position.y - 10));
        sf::Vertex v3(sf::Vector2f(v1.position.x + 3, v1.position.y - 10));
        //setting color
        v1.color = v2.color = v3.color = sf::Color(rand() % 200, rand() % 200, 156, 150);
        //rotating for random angle
        sf::Transform transform;
        transform.rotate(rand()%90, (v2.position.x + v3.position.x) / 2,v1.position.y - 5);
        v1.position = transform.transformPoint(v1.position);
        v2.position = transform.transformPoint(v2.position);
        v3.position = transform.transformPoint(v3.position);
        //appending them into vertex array
        shape.append(v1);
        shape.append(v2);
        shape.append(v3);
    }
    return shape;   
}

//rotate function finds the middle of 3 vertexes and rotates them
void rotate(sf::VertexArray& array, double angle){
    for (int i=0;i<array.getVertexCount();i+=3){
        sf::Vector2f center(
            (array[i+1].position.x + array[i+2].position.x) / 2, 
            (array[i].position.y + array[i+1].position.y) / 2);

        sf::Transform transform;
        transform.rotate((float)rand() / RAND_MAX * 2 - 1 , 
            (array[i+1].position.x + array[i+2].position.x) / 2, 
            (array[i].position.y + array[i+1].position.y) / 2);
        array[i].position = transform.transformPoint(array[i].position);
        array[i+1].position = transform.transformPoint(array[i+1].position);
        array[i+2].position = transform.transformPoint(array[i+2].position);

        float angle = atan2(array[i].position.y - center.y, array[i].position.x - center.x);
        float dx = cos(angle);
        float dy = sin(angle);
        array[i].position.x += dx;
        array[i+1].position.x += dx;
        array[i+2].position.x += dx;        
        array[i].position.y += dy;
        array[i+1].position.y += dy;
        array[i+2].position.y += dy;        

        if (center.x < 0) {
            array[i].position.x += 800;
            array[i+1].position.x += 800;
            array[i+2].position.x += 800;
        }
        if (center.y < 0) {
            array[i].position.y += 600;
            array[i+1].position.y += 600;
            array[i+2].position.y += 600;            
        }

        if (center.x > 800) {
            array[i].position.x -= 800;
            array[i+1].position.x -= 800;
            array[i+2].position.x -= 800;
        }
        if (center.y > 600) {
            array[i].position.y -= 600;        
            array[i+1].position.y -= 600;        
            array[i+2].position.y -= 600;        
        }
    }

}
int main() { 
    sf::Clock dtClock, fpsTimer;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Too Slow");
    //creating new array with 30000 triangles
    sf::VertexArray shapes = newShape(5000);
    window.setFramerateLimit(120);
    while (window.isOpen()) {
        //event to close window on close button
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(50, 50, 50));
        //no need for for now, as you can rotate them all in function and draw them together
        rotate(shapes,1); 
        window.draw(shapes); 
        window.display();
        float dt = dtClock.restart().asSeconds();
        if (fpsTimer.getElapsedTime().asSeconds() > 1) {
            fpsTimer.restart();
            std::cout << ((1.0 / dt > 60) ? 60 : (1.0 / dt)) << std::endl;
        }
    }
}