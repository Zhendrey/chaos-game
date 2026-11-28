// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

//Make the code easier to type with "using namespace"
using namespace sf;
using namespace std;

//function definitions
void adjustVertices(vector<Vector2f>& v)
{
	v.at(1).y = v.at(0).y;
	v.at(3).y = v.at(2).y;
	v.at(2).x = v.at(0).x;
	v.at(3).x = v.at(1).x;

	int horizontalDistance = abs(v.at(1).x - v.at(0).x);
	int finalYCoordinate = v.at(0).y + horizontalDistance;

	v.at(2).y = finalYCoordinate;
	v.at(3).y = finalYCoordinate;
}

void printVertices(const vector<Vector2f>& v)
{
	std::cout << "Verticies coordinates: " << std::endl;
	for(int i = 0; i < v.size(); i++)
    {
		std::cout << "vertice " << to_string(i+1) << ": (" << to_string(v.at(i).x) << ", " << to_string(v.at(i).y) << ")" << std::endl;
	}
}

int main()
{
	srand(time(0));
	// Create a video mode object
	VideoMode vm(1920, 1080);
	// Create and open a window for the game
	RenderWindow window(vm, "Chaos Game!!", Style::Default);
	
	int numVertices = rand() % (4 - 3 + 1) + 3;
	vector<Vector2f> vertices;
	vector<Vector2f> points;

	// Load the font
	Font font;
	font.loadFromFile("fonts/arialbd.ttf");
	
	// Display instructions
	string startingPoint = numVertices == 3 ? "fourth" : "fifth";
	string text = "Click on any " + to_string(numVertices) + " points on the screen, then click on a " + startingPoint + " point to start the algorithm";
	//"Click on any 3 points on the screen, then click on a fourth point to start the algorithm"
	Text instructions;
	instructions.setFont(font);
	instructions.setString(text);
	instructions.setCharacterSize(22);

	// Center the text on the screen
	FloatRect textRect = instructions.getLocalBounds();
	instructions.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top);
	instructions.setPosition(window.getSize().x / 2.0f, 15.0f);

	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
		    if (event.type == Event::Closed)
		    {
					// Quit the game when the window is closed
					window.close();
		    }
		    if (event.type == sf::Event::MouseButtonPressed)
		    {
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "verticies: " << vertices.size() << std::endl;
			    std::cout << "the left button was pressed" << std::endl;
			    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
			    std::cout << "mouse y: " << event.mouseButton.y << std::endl;
	
			    if(vertices.size() < numVertices)
			    {
					vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
			    }
			    else if(points.size() == 0)
			    {
				//fourth or fifth click
				//For a square fractal, adjust coordinates so that they make a perfect outline of a rectangle
				if(numVertices == 4) adjustVertices(vertices);
				printVertices(vertices);

				///push back to points vector
				points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
			    }
			}
		    }
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		/*
		****************************************
		Update
		****************************************
		*/
	
		if(points.size() > 0)
		{
			// Generates more than one point per frame
			for(int i = 0; i < 15; i++)
			{
				///generate more point(s)
				//tracks the index of a previous vertice
                int prevVertex;
				///select random vertex
				int randVertexIndex = rand() % numVertices;
				//(for the square fractal) if the same vertex has been selected twice, switch to a different vertex
				if(numVertices == 4 && prevVertex == randVertexIndex)
				{
					while(prevVertex == randVertexIndex) randVertexIndex = rand() % numVertices;
				}
				Vector2f selectedVertice = vertices.at(randVertexIndex);
				///calculate midpoint between random vertex and the last point in the vector
				Vector2f lastPoint = points.at(points.size() - 1);
				Vector2f newPoint((selectedVertice.x + lastPoint.x) / 2, (selectedVertice.y + lastPoint.y) / 2);
				///push back the newly generated coord.
				points.push_back(newPoint);
				prevVertex = randVertexIndex;
			}
		}
	
		/*
		****************************************
		Draw
		****************************************
		*/
		window.clear();
		window.draw(instructions);
		// vertices loop
		for(int i = 0; i < vertices.size(); i++)
		{
		    RectangleShape rect(Vector2f(10,10));
		    rect.setPosition(Vector2f(vertices[i].x, vertices[i].y));
		    rect.setFillColor(Color::Blue);
		    window.draw(rect);
		}
		// points loop
		for (int i = 0; i < points.size(); i++)
		{
			RectangleShape rect(Vector2f(5, 5));
			rect.setPosition(Vector2f(points[i].x, points[i].y));
			rect.setFillColor(Color::Blue);
			window.draw(rect);
		}
		window.display();
	}
}
