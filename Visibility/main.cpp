#include <iostream>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>

//Window variables
const int WINDOW_HEIGHT = 1500;
const int WINDOW_WIDTH = 3000;

//resolution
const int SCALE = 20;
const int SIZE = 10;

float getangle(sf::Vector2f a, sf::Vector2f b) // x1 and y1 are pos of mouse and x2 and y2 are pos of player
{
	double ang, pi = 3.14159265359;
	ang = atan2(b.y - a.y, b.x - a.x) * (180 / pi);
	return 360 - ang;
}

float getdistance(sf::Vector2f a, sf::Vector2f b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y))*SCALE;
}

void setPos(sf::Shape& shape, sf::Vector2f pos)
{
	shape.setPosition({ WINDOW_WIDTH / 2 + pos.x * SCALE , WINDOW_HEIGHT / 2 - pos.y * SCALE });
}
void setPos(sf::Vertex& shape, sf::Vector2f pos)
{
	shape.position = { (WINDOW_WIDTH - SIZE) / 2 + pos.x * SCALE, (WINDOW_HEIGHT + SIZE) / 2 - pos.y * SCALE };
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
	sf::RectangleShape axisx({ WINDOW_WIDTH,SIZE });
	axisx.setOrigin({ WINDOW_WIDTH / 2,SIZE / 2 });
	axisx.setFillColor(sf::Color::Yellow);
	setPos(axisx, { 0,0 });

	sf::RectangleShape axisy({ SIZE,WINDOW_HEIGHT });
	axisy.setOrigin({ SIZE / 2,WINDOW_HEIGHT / 2 });
	axisy.setFillColor(sf::Color::Yellow);
	setPos(axisy, { 0,0 });

	//vector of points
	std::vector<sf::Vector2f> points(3);
	points[0] = { 0,0 };
	points[1] = { 10,10 };
	points[2] = { 10,5 };
		//create "points"
	sf::CircleShape point1(SIZE);
	//point1.setOrigin({ SIZE / 2,SIZE / 2 });
	point1.setFillColor(sf::Color::Red);
	setPos(point1, points[0]);

	//create lines
	sf::RectangleShape line1;
	line1.setSize({ getdistance(points[0],points[1]),SIZE });
	line1.setOrigin({ 0,SIZE / 2 });
	line1.setRotation(getangle(points[0], points[1]));
	setPos(line1, points[0]);


	sf::VertexArray vertexPoints(sf::LineStrip,3);
	setPos(vertexPoints[0], { 0,0 });
	setPos(vertexPoints[1], { 10,10 });
	setPos(vertexPoints[2], { 10,5 });


	//Event loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{ 
				window.close();
			}
		}
		

		window.clear();
		//draw axes first
		window.draw(axisx);
		window.draw(axisy);

		//draw lines/points
		window.draw(vertexPoints);
		window.draw(line1);
		window.draw(point1);
		
		window.display();
	}


	return 0;
}