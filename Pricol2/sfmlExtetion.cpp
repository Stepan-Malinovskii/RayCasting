#include "sfmlExtantion.h"

Button::Button(sf::Vector2f _pos, sf::Vector2f _size, sf::Texture& _text, sf::IntRect textureRect)
{
	shape.setPosition(_pos);
	shape.setSize(_size);
	shape.setTexture(&_text);
	shape.setTextureRect(textureRect);
}

bool Button::isClicked(sf::Vector2i& mousePos)
{
	float x0 = shape.getPosition().x, x1 = x0 + shape.getSize().x;
	float y0 = shape.getPosition().y, y1 = y0 + shape.getSize().y;

	if (mousePos.x >= x0 && mousePos.x <= x1 && mousePos.y >= y0 && mousePos.y <= y1)
	{
		return true;
	}
	return false;
}

void Button::drawButton(sf::RenderTarget& window)
{
	window.draw(shape);
}

const sf::Vector2f& Button::getPosition() const { return shape.getPosition(); };