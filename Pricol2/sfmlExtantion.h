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
	virtual ~BaseButton() = default;
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
	virtual ~Button() = default;
	void drawButton(sf::RenderTarget& window);
private:
	sf::RectangleShape shape;
};

struct Group
{
	Group(sf::RectangleShape _shape, sf::Text _text) : shape{_shape}, text{_text}
	{
		shape.setOrigin({ shape.getSize().x / 2, shape.getSize().y / 2 });
		text.setOrigin({ text.getLocalBounds().width / 2, text.getLocalBounds().height / 2 });
		text.setPosition({ shape.getPosition().x, shape.getPosition().y});
	}

	Group() = default;

	void setPosition(sf::Vector2f position)
	{
		shape.setPosition(position);
		text.setPosition({ position.x, position.y - text.getCharacterSize() / 4 });
	}

	sf::Vector2f getSize() { return shape.getSize(); }
	sf::Vector2f getPosition() { return shape.getPosition(); }

	sf::RectangleShape shape;
	sf::Text text;
};

class DialogButton : public BaseButton
{
public:
	DialogButton(sf::RectangleShape _shape, sf::Text& _text);
	DialogButton(Group _group);
	DialogButton() = default;

	void setFunc(std::function<void()> _fn);

	void drawButton(sf::RenderTarget* window);

	void update() override;

	Group group;
private:
	std::function<void()> fn;
};

class EdingButton : public Button
{
public:
	EdingButton(sf::Vector2f _pos, sf::Vector2f _size, sf::Texture& _text, sf::IntRect teztureRect) :
		Button(_pos, _size, _text, teztureRect) { }
	EdingButton() = default;

	void setFunc(std::function<void()> _fn);

	void update() override;
private:
	std::function<void()> fn;
};
#endif // !SFMLEXT