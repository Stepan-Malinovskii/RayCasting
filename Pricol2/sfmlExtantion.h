#pragma once
#ifndef SFMLEXT
#define SFMLEXT

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>

class BaseButton
{
public:
	BaseButton(sf::Vector2f _pos, sf::Vector2f _size);
	BaseButton() = default;
	bool isClicked(sf::Vector2i& mousePos);
	virtual void update() = 0;
	const sf::Vector2f& getPosition() const;
private:
	sf::Vector2f pos, size;
};

class Button : public BaseButton
{
public:
	Button(sf::Vector2f _pos, sf::Vector2f _size, sf::Texture& _text, sf::IntRect teztureRect);
	Button() = default;

	void drawButton(sf::RenderTarget& window);
private:
	sf::RectangleShape shape;
};

struct Group
{
	sf::RectangleShape shape;
	sf::Text text;
};

class DialogButton : public BaseButton
{
public:
	DialogButton(sf::RectangleShape _shape, sf::Text& _text);
	DialogButton() = default;

	void setFunc(std::function<void()> _fn);

	void drawButton(sf::RenderTarget* window);

	void update() override;

private:
	std::function<void()> fn;
	Group group;
};

class EdingButton : public Button
{
public:
	EdingButton(sf::Vector2f _pos, sf::Vector2f _size, sf::Texture& _text, sf::IntRect teztureRect) :
		Button(_pos, _size, _text, teztureRect) { }
	EdingButton() = default;

	void setFunc(std::function<void()> _fn) { fn = _fn; }

	void update() override
	{
		fn();
	}
private:
	std::function<void()> fn;
};
#endif // !SFMLEXT