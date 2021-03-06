#include <iostream>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <queue>
#include <ctime>
#include "Point2D.h"

//Window variables
const int WINDOW_HEIGHT = 900;
const int WINDOW_WIDTH = 1300;

//resolution
const int SCALE = 40;
const int SIZE = 5;

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

void drawTriangle(Triangle t,vector<sf::ConvexShape> &drawVector)
{

}

vector<sf::RectangleShape> diagonals;
Triangle triangulate(vector<Point2D> &Polygon, vector<Point2D> &Concaves) {
	size_t n = Polygon.size();

	//default line for diagonals
	sf::RectangleShape line({ 10,SIZE });
	line.setOrigin({ 0,SIZE / 2 });
	line.setFillColor(sf::Color::Green);

	if (n > 3) {
		for (size_t i = 0; i < n; ++i) {
			//varf convex
			if (orientation(Polygon[(i - 1 + n) % n], Polygon[i], Polygon[(i + 1) % n]) > 0) {
				bool valid = true;
				//verificare nu contine varf concav
				for (Point2D point : Concaves)
					if (isInside(Polygon[(i - 1 + n) % n], Polygon[i], Polygon[(i + 1) % n], point)) {
						valid = false;
						break;
					}
				if (valid) {

					//desenam diagonala
					line.setSize({ getdistance(Polygon[(i - 1 + n) % n],Polygon[(i + 1) % n]),SIZE });
					line.setRotation(getangle(Polygon[(i - 1 + n) % n], Polygon[(i + 1) % n]));
					setPos(line, Polygon[(i - 1 + n) % n]);

					diagonals.push_back(line);

					//verificare vecini daca sunt convexi
					auto it = find(Concaves.begin(), Concaves.end(), Polygon[(i - 1 + n) % n]);
					if (it != Concaves.end() && orientation(Polygon[(i - 2 + n) % n], Polygon[(i - 1 + n) % n], Polygon[(i + 1) % n]) > 0)
						Concaves.erase(it);
					it = find(Concaves.begin(), Concaves.end(), Polygon[(i + 1) % n]);
					if (it != Concaves.end() && orientation(Polygon[(i - 1 + n) % n], Polygon[(i + 1) % n], Polygon[(i + 2) % n]) > 0)
						Concaves.erase(it);

					//save solution
					Triangle solution = { Polygon[(i - 1 + n) % n], Polygon[i], Polygon[(i + 1) % n] };

					//sterge varf taiat
					Polygon.erase(Polygon.begin() + i);

					return solution;
				}
			}
		}
	}
	Triangle solution = { Polygon[0],Polygon[1],Polygon[2] };
	Polygon.erase(Polygon.end()-1);
	Polygon.erase(Polygon.end()-1);
	Polygon.erase(Polygon.end()-1);
	return solution;
	
}

int main()
{
	ifstream fin("poligonmare.in");

#pragma region Init Geometry
	Point2D P;
	vector<Point2D> Polygon, Concaves;
	vector<Triangle> Triangulation;

	size_t n;
	fin >> n;
	for (size_t i = 0; i < n; ++i) {
		Point2D newPoint;
		fin >> newPoint;
		Polygon.push_back(newPoint);
	}
	fin >> P;
	fin.close();
	for (size_t i = 0; i < n; ++i) {
		if (orientation(Polygon[(i - 1) % n], Polygon[i], Polygon[(i + 1) % n]) < 0)
			Concaves.push_back(Polygon[i]);
	}
#pragma endregion


#pragma region Init window
	//Create window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Visibility", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	//create axes:
	sf::RectangleShape axisx({ WINDOW_WIDTH,SIZE });
	axisx.setOrigin({ WINDOW_WIDTH / 2,SIZE / 2 });
	axisx.setFillColor(sf::Color::Black);
	setPos(axisx, { 0,0 });

	sf::RectangleShape axisy({ SIZE,WINDOW_HEIGHT });
	axisy.setOrigin({ SIZE / 2,WINDOW_HEIGHT / 2 });
	axisy.setFillColor(sf::Color::Black);
	setPos(axisy, { 0,0 });

#pragma endregion

	//create vector of shapes for "points"
	vector<sf::CircleShape> points;

	//create points
	sf::CircleShape cs(SIZE);
	cs.setFillColor(sf::Color::Red);
	cs.setOrigin({ SIZE,SIZE });
	for (Point2D p : Polygon)
	{
		setPos(cs, p);
		points.push_back(cs);
	}

	//create vector of shapes for lines
	vector<sf::RectangleShape> lines;

	//create lines
	sf::RectangleShape line({ 10,SIZE });
	line.setOrigin({ 0,SIZE / 2 });
	line.setFillColor(sf::Color::Blue);
	for (size_t i = 0; i < Polygon.size(); i++)
	{
		line.setSize({ getdistance(Polygon[i],Polygon[(i + 1) % n]),SIZE });
		line.setRotation(getangle(Polygon[i], Polygon[(i + 1) % n]));
		setPos(line, Polygon[i]);

		lines.push_back(line);
	}

	//vector of triangle background
	vector<sf::ConvexShape> triangleBackground;

	vector<Point2D> polyCopy = Polygon;
	bool drawn_point = false;
	bool done_visibility = false;
	map<Triangle, vector<Triangle>> Adjacence;
	map<Triangle, bool> Visited;
	queue<Triangle> tQueue;

	int milis = 0;
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
		
		/*
		if (milis < 1000)
		{
			milis+=
		}*/

		//draw one more triangle
		if (polyCopy.size() > 3)
		{
			Triangulation.push_back(triangulate(polyCopy, Concaves));
			
		}
		else if(!drawn_point)
		{
			setPos(cs, P);
			cs.setFillColor(sf::Color(252, 186, 3));
			points.push_back(cs);
			drawn_point = true;

			for (Triangle t1 : Triangulation) {
				Visited[t1] = false;
				for (Triangle t2 : Triangulation) {
					if (isAdjacent(t1, t2))
						Adjacence[t1].push_back(t2);
				}
			}

			for (Triangle t : Triangulation) {
				if (isInside(t.A, t.B, t.C, P, true)) {
					tQueue.push(t);
					cout << "Triangle: " << t << '\n';
					Visited[t] = true;
					break;
				}
			}
			
			//draw principal triangle
			drawTriangle(tQueue.front(), triangleBackground);
		}
		else if(!tQueue.empty())
		{
			Triangle Front = tQueue.front();
			tQueue.pop();
			size_t count = 0;

			count += orientation(Front.A, Front.B, Front.C) * orientation(P, Front.B, Front.C) >= 0;
			count += orientation(Front.A, Front.B, Front.C) * orientation(Front.A, P, Front.C) >= 0;
			count += orientation(Front.A, Front.B, Front.C) * orientation(Front.A, Front.B, P) >= 0;
			if (count >= 2) {
				cout << Front << '\n';
				for (Triangle t : Adjacence[Front]) {
					if (!Visited[t]) {
						Visited[t] = true;
						tQueue.push(t);
					}
				}

				//draw triangle
			}
		}

		window.clear(sf::Color(158, 158, 157));
		//draw axes first
		window.draw(axisx);
		window.draw(axisy);

		//draw backgrounds
		for (auto drawable : triangleBackground)
			window.draw(drawable);

		//draw lines
		for (auto drawable : lines)
			window.draw(drawable);

		//draw diagonals
		for (auto drawable : diagonals)
			window.draw(drawable);

		//draw points
		for (auto drawable : points)
			window.draw(drawable);
		
		window.display();
		sf::sleep(sf::milliseconds(1000));
		
	}

	Visited.clear();
	Adjacence.clear();
	Triangulation.clear();
	Concaves.clear();
	Polygon.clear();

	return 0;
}