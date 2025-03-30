#pragma once
#ifndef SFMLEXT
#define SFMLEXT

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>

struct Group
{
	Group(sf::RectangleShape _shape, sf::Text _text) : shape{ _shape }, text{ _text }
	{
		shape.setOrigin({ shape.getLocalBounds().width / 2, shape.getLocalBounds().height / 2 });
		text.setOrigin({ text.getLocalBounds().width / 2, text.getLocalBounds().height / 2 });
		text.setPosition({ shape.getPosition().x, shape.getPosition().y - text.getCharacterSize() / 4});
	}

	Group() = default;

	void setString(std::wstring data)
	{
		text.setString(data);
		text.setOrigin({ text.getLocalBounds().width / 2, text.getLocalBounds().height / 2 });
		text.setPosition({ shape.getPosition().x, shape.getPosition().y - text.getCharacterSize() / 4 });
	}

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

class Button : public sf::Drawable
{
public:
	Button(sf::RectangleShape _shape, sf::Text& _text);
	Button(Group _group);
	Button() = default;

	void setFillColor(sf::Color color);
	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition();
	sf::Vector2f getSize();
	void setString(std::wstring data);
	void setTexture(sf::Texture* text);
	void setTextureRect(sf::IntRect rect);

	void setFunc(std::function<void()> _fn);
	bool isClicked(sf::Vector2i& mousePos);
	void use();
private:
	std::function<void()> fn;
	Group group;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // !SFMLEXT