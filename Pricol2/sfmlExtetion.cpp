#include "sfmlExtantion.h"

BaseButton::BaseButton(sf::Vector2f _pos, sf::Vector2f _size) : pos{ _pos }, size{ _size } {}

const sf::Vector2f& BaseButton::getPosition() const { return pos; }

bool BaseButton::isClicked(sf::Vector2i& mousePos)
{
	float x0 = pos.x, x1 = pos.x + size.x;
	float y0 = pos.y, y1 = pos.y + size.y;

	if (mousePos.x >= x0 && mousePos.x <= x1 && mousePos.y >= y0 && mousePos.y <= y1)
	{
		return true;
	}
	return false;
}

Button::Button(sf::Vector2f _pos, sf::Vector2f _size, sf::Texture& _text, sf::IntRect textureRect) : BaseButton(_pos, _size)
{
	shape.setPosition(_pos);
	shape.setSize(_size);
	shape.setTexture(&_text);
	shape.setTextureRect(textureRect);
}

void Button::drawButton(sf::RenderTarget& window) { window.draw(shape); }

DialogButton::DialogButton(sf::RectangleShape _shape, sf::Text& _text) :
	BaseButton(_shape.getPosition(), _shape.getSize())
{
	group.shape = _shape;
	group.text = _text;
}

DialogButton::DialogButton(Group _group) :
	BaseButton(group.shape.getPosition(), group.shape.getSize())
{
	group = _group;
}

void DialogButton::drawButton(sf::RenderTarget* window)
{
	window->draw(group.shape);
	window->draw(group.text);
}

void DialogButton::setFunc(std::function<void()> _fn)
{
	fn = _fn;
}

void DialogButton::update()
{
	if (fn != NULL) fn();
}

void EdingButton::setFunc(std::function<void()> _fn)
{
	fn = _fn;
}

void EdingButton::update()
{
	if (fn != NULL) fn();
}