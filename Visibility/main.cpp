#include <iostream>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <queue>
#include "Point2D.h"

//Window variables
const int WINDOW_HEIGHT = 500;
const int WINDOW_WIDTH = 800;

//resolution
const int SCALE = 10;
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

vector<Triangle> triangulate(vector<Point2D> Polygon, vector<Point2D> Concaves) {
	vector<Triangle> Result;
	size_t n = Polygon.size();
	while (n > 2) {
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
					//avem nou triunghi
					Result.push_back({ Polygon[(i - 1 + n) % n], Polygon[i], Polygon[(i + 1) % n] });
					//TODO: desenam diagonala
					//verificare vecini daca sunt convexi
					auto it = find(Concaves.begin(), Concaves.end(), Polygon[(i - 1) % n]);
					if (it != Concaves.end() && orientation(Polygon[(i - 2 + n) % n], Polygon[(i - 1 + n) % n], Polygon[(i + 1) % n]) > 0)
						Concaves.erase(it);
					it = find(Concaves.begin(), Concaves.end(), Polygon[(i + 1) % n]);
					if (it != Concaves.end() && orientation(Polygon[(i - 1 + n) % n], Polygon[(i + 1) % n], Polygon[(i + 2) % n]) > 0)
						Concaves.erase(it);
					//sterge varf taiat
					Polygon.erase(Polygon.begin() + i);
					n--;
				}
			}
		}
	}
	return Result;
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
#pragma region Init Geometry
	Point2D P;
	vector<Point2D> Polygon, Concaves;
	ifstream fin("date.in");
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

	vector<Triangle> Triangulation = triangulate(Polygon, Concaves);
	/*
	for (auto t : Triangulation) {
		cout << t.A << ' ' << t.B << ' ' << t.C << "\n";
	}
	map<Triangle, vector<Triangle>> Adjacence;
	map<Triangle, bool> Visited;
	for (Triangle t1 : Triangulation) {
		Visited[t1] = false;
		for (Triangle t2 : Triangulation) {
			if (isAdjacent(t1, t2))
				Adjacence[t1].push_back(t2);
		}
	}
	queue<Triangle> tQueue;
	for (Triangle t : Triangulation) {
		if (isInside(t.A, t.B, t.C, P, true)) {
			tQueue.push(t);
			cout << "Triangle: " << t << '\n';
			Visited[t] = true;
			break;
		}
	}
	cout << "Observable:\n";
	while (!tQueue.empty()) {
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
		}
	}
	Visited.clear();
	Adjacence.clear();
	Triangulation.clear();
	Concaves.clear();
	Polygon.clear();*/

#pragma region Init window
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

#pragma endregion


	//create vector of shapes for "points"
	vector<sf::CircleShape> points;

	//default point
	sf::CircleShape cs(SIZE);
	cs.setFillColor(sf::Color::Red);

	for (Point2D p : Polygon)
	{
		setPos(cs, { (float)p.X, (float)p.Y });
	}
	//point1.setOrigin({ SIZE / 2,SIZE / 2 });
	

	/*create lines
	sf::RectangleShape line1;
	line1.setSize({ getdistance(points[0],points[1]),SIZE });
	line1.setOrigin({ 0,SIZE / 2 });
	line1.setRotation(getangle(points[0], points[1]));
	setPos(line1, points[0]);


	sf::VertexArray vertexPoints(sf::LineStrip,3);
	setPos(vertexPoints[0], { 0,0 });
	setPos(vertexPoints[1], { 10,10 });
	setPos(vertexPoints[2], { 10,5 });*/


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

		//draw points
		for (auto drawable : points)
		{
			window.draw(drawable);
		}
		
		window.display();
	}


	return 0;
}