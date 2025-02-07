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

	auto sprite_func = [&]() {
		DrawSprite(pDirection, cameraPlane, position, sprites,
			invDet, angle);
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
	for (int i = 0; i <= SCREEN_W; i++)
	{
		float cameraX = i * 2.0f / SCREEN_W - 1.0f;
		rayDir = pDirection + cameraPlane * cameraX;

		RayHit hit = raycast(map, rayPos, rayDir);

		if (hit.cell)
		{
			float wallHeight = SCREEN_H / hit.perpWallDist;
			float wallStart = (SCREEN_H - wallHeight) / 2.0f;
			float wallEnd = (SCREEN_H + wallHeight) / 2.0f;

			float wallX = hit.isHitVert ? rayPos.x + hit.perpWallDist * rayDir.x :
				rayPos.y + hit.perpWallDist * rayDir.y;

			wallX -= floor(wallX);
			float textureX = wallX * TEXTURE_SIZE;

			float brightnes = 1.0f - (hit.perpWallDist / brightnesDist);
			if (hit.isHitVert) brightnes -= 0.2f;
			if (brightnes < 0.0f) brightnes = 0.0f;
			sf::Color colorShade(255 * brightnes, 255 * brightnes, 255 * brightnes);

			walls.append(sf::Vertex(sf::Vector2f((float)i, wallStart), colorShade,
				sf::Vector2f(textureX + (hit.cell - 1) * TEXTURE_SIZE, 0.0f)));
			walls.append(sf::Vertex(sf::Vector2f((float)i, wallEnd), colorShade,
				sf::Vector2f(textureX + (hit.cell - 1) * TEXTURE_SIZE, TEXTURE_SIZE)));

			distanceBuffer[i] = hit.perpWallDist;
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
	target.draw(debugColumns);

	spriteColumns.clear();
	debugColumns.clear();
	walls.clear();
}

void Renderer::DrawSprite(sf::Vector2f& pDirection, sf::Vector2f& cameraPlane, const sf::Vector2f& playerPos,
	std::vector<std::shared_ptr<Sprite>>& sprites, float invDet, float plAngle)
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

		float startYtext= sp->texture * SPRITE_SIZE, endYtext = SPRITE_SIZE * (sp->texture + 1);
		float deltaRotateText = 0.0f;
		if (sp->isDirectional)
		{
			sf::Vector2f dir = spritePos;
			float len = sqrt(dir.x * dir.x + dir.y * dir.y);
			dir /= len;

			float spAngle = sp->angle * PI / 180.0f;
			float angle = spAngle - atan2(dir.y, dir.x);
			angle = angle * 180.0f / PI;
			angle = round(angle / 45.0f) * 45.0f;
			angle = fmod(angle, 360.0f);
			if (angle < 0.0f)
			{
				angle += 360.0f;
			}
			deltaRotateText = SPRITE_SIZE * (angle / 45.0f);
		}

		int spriteStart = -spriteSize * sp->size / 2 + screenX,
			spriteEnd = spriteSize * sp->size / 2 + screenX;
		for (int i = std::max(drawStart, 0); i < std::min(drawEnd, (int)SCREEN_W - 1); i++)
		{
			if (transforme.y > 0 && transforme.y < distanceBuffer[i])
			{
				float textX = (i - drawStart) * SPRITE_SIZE / spriteSize;
				sf::Vector2f textStart(textX + deltaRotateText, startYtext + 0.1f);
				sf::Vector2f textEnd(textX + deltaRotateText, endYtext);

				sf::Vector2f vertStart(i, -spriteSize / 2.0f + SCREEN_H / 2.0f);
				sf::Vector2f vertEnd(i, spriteSize / 2.0f + SCREEN_H / 2.0f);

				spriteColumns.append(sf::Vertex(vertStart, textStart));
				spriteColumns.append(sf::Vertex(vertEnd, textEnd));

				if (i == drawStart || i == drawEnd - 1)
				{
					debugColumns.append(sf::Vertex(vertStart, sf::Color::Green));
					debugColumns.append(sf::Vertex(vertEnd, sf::Color::Green));
				}
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

			int floorText = map.GetOnGrid(cell.x, cell.y, FLOOR_LAYER);
			int cellingText = map.GetOnGrid(cell.x, cell.y, CELL_LAYER);

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

