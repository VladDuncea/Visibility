#include <iostream>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>

//Window variables
const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 900;

int main()
{
	//Create window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Visibility", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

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
		//window.draw(shape);
		window.display();
	}


	return 0;
}