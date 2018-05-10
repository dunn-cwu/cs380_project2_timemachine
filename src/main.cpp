/* =============================================
* Assignmet: Project 2
* Course: CS380 - Intro to Software Engineering
* Student: Andrew Dunn (no team)
*
* File: main.cpp
* Description: Program entry point and main game loop.
* ==============================================
*/

#include <string>
#include <SFML/Graphics.hpp>
#include "CTimeMachineGame.h"

int main(int argc, char **argv)
{
	// SFML Window stuff
	sf::RenderWindow window(sf::VideoMode(720, 720), "CS380: Project 2 - Time Machine (Press '+' to speed up, '-' to slow down game.)");
	sf::Color clearColor = sf::Color(247, 241, 217, 255);
	sf::Time frameWaitTime = sf::seconds(1.0f / 60.0f);

	// Create time machine game class
	CTimeMachineGame tmGame(window.getSize().x, window.getSize().y);

	// Initialize new game state
	tmGame.initNewGame();

	// Main game loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close(); // User closed window
			else if (event.type == sf::Event::Resized)
			{
				// User resized window, resize grid rect to fit within window
				sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
				window.setView(sf::View(visibleArea));
				tmGame.resizeBoardRect(window.getSize().x, window.getSize().y);
			}
		}

		// Clear window and set background color
		window.clear(clearColor);

		// Update and draw game
		tmGame.update(&window);

		// Present display
		window.display();

		// Sleep for 1/60 seconds
		sf::sleep(frameWaitTime);
	}

	return 0;
}

/* =========================================
*  End of File
*  =========================================
*/