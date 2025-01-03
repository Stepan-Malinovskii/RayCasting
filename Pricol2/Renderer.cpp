#include "Renderer.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <mutex>

Renderer::Renderer()
{
	Init();
	screenPixels = new uint8_t[(int)SCREEN_H * (int)SCREEN_W * 4]();
	distanceBuffer = new float[(int)SCREEN_W]();
	threads = new std::vector<std::jthread>(THREAD_COUNT);
}

Renderer::~Renderer()
{
	delete threads;
	/*???????????????????????delete[] distanceBuffer;*/
	delete[] screenPixels;
}

void Renderer::Init()
{
	floorTexture.create(SCREEN_W, SCREEN_H);
	floorSprite.setTexture(floorTexture);

	cellingTexture.create(SCREEN_W, SCREEN_H);
	cellingSprite.setTexture(cellingTexture);
	cellingSprite.setPosition(0, -SCREEN_H / 2.0f);
}

void Renderer::Draw3DView(sf::RenderTarget& target, sf::Vector2f position, float angle,
	const Map& map, std::vector<std::shared_ptr<Sprite>>& sprites)
{
	//StaticCalculations
	float pRadians = angle * PI / 180.0f;
	sf::Vector2f pDirection{ cosf(pRadians), sinf(pRadians) };
	sf::Vector2f cameraPlane = sf::Vector2f(- pDirection.y, pDirection.x) * ASPECT;
	sf::Vector2f rayPos = position;

	//FloorPart
	sf::Vector2f rayDirLeft{ pDirection - cameraPlane },
		rayDirRight{ pDirection + cameraPlane };
	auto floor_func = [&](int thread_id)
		{
			int start = (int)((SCREEN_H / 2.0f) * (1 + 1.0f / (THREAD_COUNT - 1) * thread_id));
			int end = (int)((SCREEN_H / 2.0f) * (1 + 1.0f / (THREAD_COUNT - 1) * (thread_id + 1)));
			DrawFloor(rayDirLeft, rayDirRight, rayPos, map, start, end);
		};

	//SpritePart
	auto getDist = [position](const std::shared_ptr<Sprite> sp)
		{
			return pow(position.x - sp->position.x, 2) +
				pow(position.y - sp->position.y, 2);
		};
	auto comperer = [getDist](const std::shared_ptr<Sprite> a, const std::shared_ptr<Sprite> b)
		{
			return getDist(a) > getDist(b);
		};
	std::sort(sprites.begin(), sprites.end(), comperer);

	float invDet = 1.0f / (cameraPlane.x * pDirection.y - cameraPlane.y * pDirection.x);
	float spriteTextSizeY = Resources::spritesTexture.getSize().y;

	auto sprite_func = [&]() {
		DrawSprite(pDirection, cameraPlane, position, sprites,
			invDet, spriteTextSizeY);
		};

	for (int cnt = 0; cnt < THREAD_COUNT - 1; cnt++)
	{
		(*threads)[cnt] = std::jthread(floor_func, cnt);
	}

	(*threads)[THREAD_COUNT - 1] = std::jthread(sprite_func);

	//MiniShaderPart
	float brightnesDist = MAX_DETH / 2.0f;

	//SkyPart
	sf::Vector2u skyTextureSize = Resources::skyTextures.getSize();
	int textureOffsetX = (int)(angle / 90.0f * skyTextureSize.x);
	while (textureOffsetX < 0)
	{
		textureOffsetX += skyTextureSize.x;
	}

	sf::Vertex sky[] = {
		sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(textureOffsetX, 0.0f)),
		sf::Vertex(sf::Vector2f(0.0f, SCREEN_H), sf::Vector2f(textureOffsetX, skyTextureSize.y)),
		sf::Vertex(sf::Vector2f(SCREEN_W, SCREEN_H),
		sf::Vector2f(textureOffsetX + skyTextureSize.x, skyTextureSize.y)),
		sf::Vertex(sf::Vector2f(SCREEN_W, 0), sf::Vector2f(textureOffsetX + skyTextureSize.x, 0.0f))
	};

	target.draw(sky, 4, sf::Quads, sf::RenderStates(&Resources::skyTextures));

	//NewAlgoritmPart
	sf::Vector2f rayDir;
	sf::Vector2i rayStep;
	sf::Vector2f deltaDist;
	sf::Vector2i rayMapPos;
	sf::Vector2f sideDist;
	for (int i = 1; i <= SCREEN_W; i++)
	{
		float cameraX = i * 2.0f / SCREEN_W - 1.0f;
		rayDir = pDirection + cameraPlane * cameraX;
		rayStep;
		deltaDist.x = abs(1.0f / rayDir.x); deltaDist.y = abs(1.0f / rayDir.y);
		rayMapPos.x = rayPos.x; rayMapPos.y = rayPos.y;
		sideDist;

		int isHit = 0;
		bool isHitVer = false;
		int rayDeth = 0;

		if (rayDir.x < 0.0f)
		{
			rayStep.x = -1;
			sideDist.x = (rayPos.x - rayMapPos.x) * deltaDist.x;
		}
		else
		{
			rayStep.x = 1;
			sideDist.x = (rayMapPos.x - rayPos.x + 1.0f) * deltaDist.x;
		}

		if (rayDir.y < 0.0f)
		{
			rayStep.y = -1;
			sideDist.y = (rayPos.y - rayMapPos.y) * deltaDist.y;
		}
		else
		{
			rayStep.y = 1;
			sideDist.y = (rayMapPos.y - rayPos.y + 1.0f) * deltaDist.y;
		}

		while (isHit == 0 && rayDeth < MAX_DETH)
		{
			if (sideDist.x < sideDist.y)
			{
				sideDist.x += deltaDist.x;
				rayMapPos.x += rayStep.x;
				isHitVer = false;
			}
			else
			{
				sideDist.y += deltaDist.y;
				rayMapPos.y += rayStep.y;
				isHitVer = true;
			}

			isHit = map.GetNewOnGrid(rayMapPos.x, rayMapPos.y, WALL_LAYER);
			rayDeth++;
		}
		if (isHit != 0)
		{
			float perpWallDist = isHitVer ? sideDist.y - deltaDist.y :
				sideDist.x - deltaDist.x;
			float wallHeight = SCREEN_H / perpWallDist;
			float wallStart = (SCREEN_H - wallHeight) / 2.0f;
			float wallEnd = (SCREEN_H + wallHeight) / 2.0f;

			float wallX = isHitVer ? rayPos.x + perpWallDist * rayDir.x :
				rayPos.y + perpWallDist * rayDir.y;

			wallX -= floor(wallX);
			float textureX = wallX * TEXTURE_SIZE;

			float brightnes = 1.0f - (perpWallDist / brightnesDist);
			if (isHitVer) brightnes -= 0.2f;
			if (brightnes < 0.0f) brightnes = 0.0f;
			sf::Color colorShade(255 * brightnes, 255 * brightnes, 255 * brightnes);

			walls.append(sf::Vertex(sf::Vector2f((float)i, wallStart), colorShade,
				sf::Vector2f(textureX + (isHit - 1) * TEXTURE_SIZE, 0.0f)));
			walls.append(sf::Vertex(sf::Vector2f((float)i, wallEnd), colorShade,
				sf::Vector2f(textureX + (isHit - 1) * TEXTURE_SIZE, TEXTURE_SIZE)));

			distanceBuffer[i] = perpWallDist;
		}
	}

	//ThreadPart
	for (int cnt = 0; cnt < THREAD_COUNT; cnt++)
	{
		(*threads)[cnt].join();
	}

	//DrawPart
	floorTexture.update(screenPixels);
	target.draw(floorSprite);
	target.draw(cellingSprite);
	sf::RenderStates states{ &Resources::textures };
	target.draw(walls, states);
	
	states.texture = &Resources::spritesTexture;
	target.draw(spriteColumns, states);

	spriteColumns.clear();
	walls.clear();
}

void Renderer::DrawSprite(sf::Vector2f& pDirection, sf::Vector2f& cameraPlane, const sf::Vector2f& playerPos,
	std::vector<std::shared_ptr<Sprite>>& sprites, float invDet, float spriteTextSizeY)
{

	for (auto sp : sprites)
	{
		if (sp->texture < 0) continue;
		sf::Vector2f spritePos = sp->position - playerPos;

		//transform sprite with the inverse camera matrix
		// [ planeX   dirX ] -1    [x]                                     [ dirY      -dirX ]   [x]
		// [               ]     * [ ]   =  1/(planeX*dirY-dirX*planeY) *  [                 ] * [ ]
		// [ planeY   dirY ]       [y]                                     [ -planeY  planeX ]   [y]


		sf::Vector2f transforme(pDirection.y * spritePos.x - pDirection.x * spritePos.y,
			-cameraPlane.y * spritePos.x + cameraPlane.x * spritePos.y);
		transforme *= invDet;

		int screenX = SCREEN_W / 2.0f * (1 + transforme.x / transforme.y);
		int spriteSize = abs(SCREEN_H / transforme.y);
		int drawStart = -spriteSize / 2 + screenX;
		int drawEnd = spriteSize / 2 + screenX;

		float spText = sp->texture * (spriteTextSizeY + 1);
		for (int i = std::max(drawStart, 0); i < std::min(drawEnd, (int)SCREEN_W - 1); i++)
		{
			if (transforme.y > 0 && transforme.y < distanceBuffer[i])
			{
				float textX = spText + (i - drawStart) * spriteTextSizeY / spriteSize;

				spriteColumns.append(sf::Vertex({ (float)i, -spriteSize / 2.0f + SCREEN_H / 2.0f },
					{ textX, 0 }));
				spriteColumns.append(sf::Vertex({ (float)i, spriteSize / 2.0f + SCREEN_H / 2.0f },
					{ textX, spriteTextSizeY }));
			}
		}

	}
}

void Renderer::DrawFloor(sf::Vector2f& rayDirLeft, sf::Vector2f& rayDirRight, sf::Vector2f& rayPos,
	const Map& map, int startH, int endH)
{
	for (int y = startH; y < endH; y++)
	{
		float rowDist = CAMERA_Z / ((float)y - SCREEN_H / 2.0f);
		sf::Vector2f floorStep = rowDist * (rayDirRight - rayDirLeft) / SCREEN_W;
		sf::Vector2f floor = rayPos + rowDist * rayDirLeft;

		for (int x = 0; x < SCREEN_W; x++)
		{
			sf::Vector2i cell{ floor };
			sf::Vector2i textCoords{ TEXTURE_SIZE * (floor - (sf::Vector2f)cell) };
			textCoords.x &= (int)TEXTURE_SIZE - 1;
			textCoords.y &= (int)TEXTURE_SIZE - 1;

			int floorText = map.GetNewOnGrid(cell.x, cell.y, FLOOR_LAYER);
			int cellingText = map.GetNewOnGrid(cell.x, cell.y, CELL_LAYER);

			sf::Color floorColor, cellingColor;

			floorColor = floorText == 0 ? sf::Color(70, 70, 70) :
				Resources::textureImage.getPixel((floorText - 1) * TEXTURE_SIZE + textCoords.x, textCoords.y);

			cellingColor = cellingText == 0 ? sf::Color(0, 0, 0, 0) :
				Resources::textureImage.getPixel((cellingText - 1) * TEXTURE_SIZE + textCoords.x, textCoords.y);

			screenPixels[(x + y * (int)SCREEN_W) * 4 + 0] = floorColor.r;
			screenPixels[(x + y * (int)SCREEN_W) * 4 + 1] = floorColor.g;
			screenPixels[(x + y * (int)SCREEN_W) * 4 + 2] = floorColor.b;
			screenPixels[(x + y * (int)SCREEN_W) * 4 + 3] = floorColor.a;

			screenPixels[(x + ((int)SCREEN_H - y - 1) * (int)SCREEN_W) * 4 + 0] = cellingColor.r;
			screenPixels[(x + ((int)SCREEN_H - y - 1) * (int)SCREEN_W) * 4 + 1] = cellingColor.g;
			screenPixels[(x + ((int)SCREEN_H - y - 1) * (int)SCREEN_W) * 4 + 2] = cellingColor.b;
			screenPixels[(x + ((int)SCREEN_H - y - 1) * (int)SCREEN_W) * 4 + 3] = cellingColor.a;

			floor += floorStep;
		}
	}
}

