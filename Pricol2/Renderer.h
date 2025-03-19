#pragma once
#ifndef RENDERER
#define RENDERER

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <thread>

#include "Player.h"
#include "Map.h"
#include "Sprite.h"
#include "Raycast.h"
#include "Resources.h"

constexpr int THREAD_COUNT = 10, MAX_DETH = 64;
constexpr float PLAYER_FOV = 60.0f, CAMERA_Z = SCREEN_H / 2.0f,
ASPECT = SCREEN_W / SCREEN_H * 0.5f, BRIGHTNESTDIST = MAX_DETH / 7.0f;

class Renderer
{
public:
	Renderer(sf::RenderWindow* window);
	~Renderer();

	void Init();

	void Draw3DView(Player* player, Map* map, std::vector<std::shared_ptr<Sprite>>& sprites);
private:
	sf::RenderWindow* window;
	sf::Texture floorTexture;
	sf::Sprite floorSprite;
	uint8_t* screenPixels;

	sf::VertexArray walls{ sf::Lines };
	sf::VertexArray spriteColumns{ sf::Lines };
	float* distanceBuffer;

	std::vector<std::jthread> threads;

	void DrawFloor(sf::Vector2f& rayDirLeft, sf::Vector2f& rayDirRight, sf::Vector2f& rayPos, 
		Player* player, Map* map, int startH, int endH);
	void DrawSprite(sf::Vector2f& pDirection, sf::Vector2f& cameraPlane, Player* player,
		std::vector<std::shared_ptr<Sprite>>& sprites, float invDet);
};

#endif // !RENDERER
