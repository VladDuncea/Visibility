#include <iostream>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>

//Window variables
const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 1500;

//resolution
const int SCALE = 4;


void setPos(sf::CircleShape& cs, sf::Vector2f pos)
{
	cs.setPosition({WINDOW_WIDTH / 2 + pos.x * SCALE, WINDOW_HEIGHT / 2 - pos.y * SCALE });
}

int main()
{
	//Create window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Visibility", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	//Create a small circle to use for points
	sf::CircleShape shape(1 * SCALE);
	shape.setFillColor(sf::Color::Red);
	setPos(shape, { 1,1 });

	//create axes:
	sf::RectangleShape axisx({ WINDOW_WIDTH,1*SCALE });
	axisx.setFillColor(sf::Color::Yellow);
	axisx.setPosition({ 0,(WINDOW_HEIGHT + axisx.getSize().y)/2 });

	sf::RectangleShape axisy({ 1*SCALE,WINDOW_HEIGHT });
	axisy.setFillColor(sf::Color::Yellow);
	axisy.setPosition({ (WINDOW_WIDTH + axisy.getSize().x)/2,0 });

	sf::VertexArray vertexPoints(sf::LinesStrip,3);
	vertexPoints[0].position = { 100,100 };
	vertexPoints[1].position = { 500,500 };
	vertexPoints[2].position = { 500,300 };


	//Event loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		//draw axes first
		window.draw(axisx);
		window.draw(axisy);

		//draw lines/points
		window.draw(vertexPoints);
		
		window.display();
	}


	return 0;
}