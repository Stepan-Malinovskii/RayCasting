#pragma once
#ifndef RENDERER
#define RENDERER

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <thread>

#include "Map.h"
#include "Sprite.h"
#include "Raycast.h"
#include "Resources.h"

constexpr float PLAYER_FOV = 60.0f, CAMERA_Z = SCREEN_H / 2.0f, ASPECT = SCREEN_W / SCREEN_H * 0.5f;
constexpr int THREAD_COUNT = 8, MAX_DETH = 64;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init();

	void Draw3DView(sf::RenderTarget& target, sf::Vector2f position, float angle,
		const Map& map, std::vector<std::shared_ptr<Sprite>>& sprites);
private:
	sf::Texture floorTexture;
	sf::Sprite floorSprite;
	sf::Texture cellingTexture;
	sf::Sprite cellingSprite;
	uint8_t* screenPixels;

	sf::VertexArray walls{ sf::Lines };
	sf::VertexArray spriteColumns{ sf::Lines };
	sf::VertexArray debugColumns{ sf::Lines };
	float* distanceBuffer;

	std::vector<std::jthread>* threads;

	void DrawFloor(sf::Vector2f& pDirection, sf::Vector2f& cameraPlane, sf::Vector2f& rayPos,
		const Map& map, int startH, int endH);
	void DrawSprite(sf::Vector2f& pDirection, sf::Vector2f& cameraPlane, const sf::Vector2f& playerPos,
		std::vector<std::shared_ptr<Sprite>>& sprites, float invDet, float plAngle);
};

#endif // !RENDERER
