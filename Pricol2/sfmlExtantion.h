#pragma once
#ifndef SFMLEXT
#define SFMLEXT

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

class Button
{
public:
	Button(sf::Vector2f _pos, sf::Vector2f _size, sf::Texture& _text, sf::IntRect teztureRect);
	Button() = default;

	void drawButton(sf::RenderTarget& window);
	bool isClicked(sf::Vector2i& mousePos);
	virtual void update() = 0;

	const sf::Vector2f& getPosition() const;
private:
	sf::RectangleShape shape;
};
#endif // !SFMLEXT