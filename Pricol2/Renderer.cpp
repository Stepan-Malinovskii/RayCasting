#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow* _window)
{
	window = _window;
	Init();
	screenPixels = new uint8_t[(int)SCREEN_H * (int)SCREEN_W * 4]();
	distanceBuffer = new float[(int)SCREEN_W + 1] {};
	threads = std::vector<std::thread>(THREAD_COUNT);
}

Renderer::~Renderer()
{
	delete[] distanceBuffer;
	delete[] screenPixels;
}

void Renderer::Init()
{
	floorTexture.create(SCREEN_W, SCREEN_H);
	floorSprite.setTexture(floorTexture);
}

void Renderer::Draw3DView(Player* player, Map* map, std::vector<std::shared_ptr<Sprite>>& sprites)
{
	//StaticCalculations
	float pRadians = player->sprite->spMap.angle * PI / 180.0f;
	sf::Vector2f pDirection{ cosf(pRadians), sinf(pRadians) };
	sf::Vector2f cameraPlane = sf::Vector2f(- pDirection.y, pDirection.x) * ASPECT;
	sf::Vector2f rayPos = player->sprite->spMap.position;

	//FloorPart
	sf::Vector2f rayDirLeft{ pDirection - cameraPlane },
		rayDirRight{ pDirection + cameraPlane };
	auto floor_func = [&](int thread_id)
		{
			int start = (int)((SCREEN_H / (THREAD_COUNT - 1) * thread_id));
			int end = (int)((SCREEN_H / (THREAD_COUNT - 1) * (thread_id + 1)));
			DrawFloor(rayDirLeft, rayDirRight, rayPos, player, map, start, end );
		};
	//SpritePart
	auto getDist = [player](const std::shared_ptr<Sprite> sp)
		{
			return SQUARE(player->sprite->spMap.position.x - sp->spMap.position.x) +
				SQUARE(player->sprite->spMap.position.y - sp->spMap.position.y);
		};
	auto comperer = [player](const std::shared_ptr<Sprite> a, const std::shared_ptr<Sprite> b)
		{
			return COMPARER(a.get()->spMap.position, b.get()->spMap.position, player->sprite->spMap.position);
		};

	float invDet = 1.0f / (cameraPlane.x * pDirection.y - cameraPlane.y * pDirection.x);

	auto sprite_func = [&]() {
		std::sort(sprites.begin(), sprites.end(), comperer);
		};

	for (int cnt = 0; cnt < THREAD_COUNT - 1; cnt++)
	{
		threads[cnt] = std::thread(floor_func, cnt);
	}

	threads[THREAD_COUNT - 1] = std::thread(sprite_func);

	//SkyPart
	sf::Vector2u skyTextureSize = Resources::skyTextures.getSize();
	int textureOffsetX = (int)(player->sprite->spMap.angle / 90.0f * skyTextureSize.x);
	while (textureOffsetX < 0)
	{
		textureOffsetX += skyTextureSize.x;
	}

	sf::Vertex sky[] = {
		sf::Vertex(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(textureOffsetX, -player->pitch)),
		sf::Vertex(sf::Vector2f(0.0f, SCREEN_H), sf::Vector2f(textureOffsetX, skyTextureSize.y - player->pitch)),
		sf::Vertex(sf::Vector2f(SCREEN_W, SCREEN_H),
		sf::Vector2f(textureOffsetX + skyTextureSize.x, skyTextureSize.y - player->pitch)),
		sf::Vertex(sf::Vector2f(SCREEN_W, 0), sf::Vector2f(textureOffsetX + skyTextureSize.x, -player->pitch))
	};

	window->draw(sky, 4, sf::Quads, sf::RenderStates(&Resources::skyTextures));
	//NewAlgoritmPart
	sf::Vector2f rayDir{};
	for (int i = 0; i <= SCREEN_W; i++)
	{
		float cameraX = i * 2.0f / SCREEN_W - 1.0f;
		rayDir = pDirection + cameraPlane * cameraX;

		RayHit hit = raycast(map, rayPos, rayDir);
		
		if (hit.cell)
		{
			float wallHeight = SCREEN_H / hit.perpWallDist;
			float wallStart = (SCREEN_H - wallHeight) / 2.0f + player->pitch + player->posZ / hit.perpWallDist;
			float wallEnd = (SCREEN_H + wallHeight) / 2.0f + player->pitch + player->posZ / hit.perpWallDist;
			

			float wallX = hit.isHitVert ? rayPos.x + hit.perpWallDist * rayDir.x :
				rayPos.y + hit.perpWallDist * rayDir.y;

			wallX -= floor(wallX);
			float textureX = wallX * TEXTURE_SIZE;

			float brightnes = 1.0f - (hit.perpWallDist / BRIGHTNESTDIST);
			if (brightnes < 0.11f) brightnes = 0.11f;
			if (hit.isHitVert) brightnes -= 0.09f;
			sf::Color colorShade(255 * brightnes, 255 * brightnes, 255 * brightnes);
			
			walls.append(sf::Vertex(sf::Vector2f((float)i, wallStart), colorShade,
				sf::Vector2f(textureX + (hit.cell - 1) % (int)TEXTURE_COUNT * TEXTURE_SIZE, 
					(hit.cell - 1) / (int)TEXTURE_COUNT * TEXTURE_SIZE)));
			walls.append(sf::Vertex(sf::Vector2f((float)i, wallEnd + 1), colorShade,
				sf::Vector2f(textureX + (hit.cell - 1) % (int)TEXTURE_COUNT * TEXTURE_SIZE, 
					TEXTURE_SIZE + (hit.cell - 1) / (int)TEXTURE_COUNT * TEXTURE_SIZE)));

			distanceBuffer[i] = hit.perpWallDist;
		}
	}

	//ThreadPart
	for (int cnt = 0; cnt < THREAD_COUNT; cnt++)
	{
		threads[cnt].join();
	}

	//DrawPart
	floorTexture.update(screenPixels);
	window->draw(floorSprite);

	sf::RenderStates states{ &Resources::textures };
	window->draw(walls, states);
	
	DrawSprite(pDirection, cameraPlane, player, sprites, invDet);

	spriteColumns.clear();
	walls.clear();
}

void Renderer::DrawSprite(sf::Vector2f& pDirection, sf::Vector2f& cameraPlane, Player* player,
	std::vector<std::shared_ptr<Sprite>>& sprites, float invDet)
{
	for (auto sp : sprites)
	{
		if (sp->spDef.texture < 0) continue;
		sf::Vector2f spritePos = sp->spMap.position - player->sprite->spMap.position;
		float spDist = sqrt(SQUARE(spritePos.x) + SQUARE(spritePos.y));
		float brightnes = 1 - spDist / BRIGHTNESTDIST;
		if (brightnes < 0.1f) brightnes = 0.1f;
		sf::Color colorShade(255 * brightnes, 255 * brightnes, 255 * brightnes);
		if (sp->isDamages) 
		{ 
			if (colorShade.r * 1.5f > 255) colorShade.r = 255;
			colorShade.g /= 2;
			colorShade.b /= 2;
		}

		//transform sprite with the inverse camera matrix
		// [ planeX   dirX ] -1    [x]                                     [ dirY      -dirX ]   [x]
		// [               ]     * [ ]   =  1/(planeX*dirY-dirX*planeY) *  [                 ] * [ ]
		// [ planeY   dirY ]       [y]                                     [ -planeY  planeX ]   [y]


		sf::Vector2f transforme(pDirection.y * spritePos.x - pDirection.x * spritePos.y,
			-cameraPlane.y * spritePos.x + cameraPlane.x * spritePos.y);
		transforme *= invDet;

		int screenX = SCREEN_W / 2.0f * (1 + transforme.x / transforme.y);
		int spriteSize = abs(SCREEN_H / transforme.y);

		int drawStartX = -spriteSize / 2 + screenX;
		int drawEndX = spriteSize / 2 + screenX;

		float deltaRotateText = 0.0f;
		if (sp->spDef.isDirectional)
		{
			sf::Vector2f dir = spritePos;
			float len = sqrt(dir.x * dir.x + dir.y * dir.y);
			dir /= len;

			float spAngle = sp->spMap.angle * PI / 180.0f;
			float angle = spAngle - atan2(dir.y, dir.x);
			angle = angle * 180.0f / PI;
			angle = round(angle / 45.0f) * 45.0f;
			angle = fmod(angle, 360.0f);
			if (angle < 0.0f)
			{
				angle += 360.0f;
			}
			deltaRotateText = sp->textSize * (angle / 45.0f);
		}

		int spriteStart = -spriteSize * sp->spDef.size / 2 + screenX,
			spriteEnd = spriteSize * sp->spDef.size / 2 + screenX;

		for (int i = std::max(drawStartX, 0); i < std::min(drawEndX, (int)SCREEN_W - 1); i++)
		{
			if (transforme.y > 0 && transforme.y < distanceBuffer[i])
			{
				float textX = (i - drawStartX) * sp->textSize / spriteSize;
				sf::Vector2f textStart(textX + deltaRotateText, 0);
				sf::Vector2f textEnd(textX + deltaRotateText, sp->textSize);

				sf::Vector2f vertStart(i, -spriteSize / 2.0f + SCREEN_H / 2.0f + player->pitch + player->posZ / transforme.y);
				sf::Vector2f vertEnd(i, spriteSize / 2.0f + SCREEN_H / 2.0f + player->pitch + player->posZ / transforme.y);

				spriteColumns.append(sf::Vertex(vertStart, colorShade, textStart));
				spriteColumns.append(sf::Vertex(vertEnd, colorShade, textEnd));
			}
		}
		sf::RenderStates state(sp->texture);
		window->draw(spriteColumns, state);
		spriteColumns.clear();
	}
}

void Renderer::DrawFloor(sf::Vector2f& rayDirLeft, sf::Vector2f& rayDirRight, sf::Vector2f& rayPos, Player* player, Map* map, int startH, int endH)
{
	for (int y = startH; y < endH; y++)
	{
		bool is_floor = y > SCREEN_H / 2 + player->pitch;
		int p = is_floor ? (y - SCREEN_H / 2 - player->pitch) : (SCREEN_H / 2 - y + player->pitch);
		float rowDist = is_floor ? ( CAMERA_Z + player->posZ ) / p : ( CAMERA_Z - player->posZ )/ p ;
		sf::Vector2f floorStep = rowDist * (rayDirRight - rayDirLeft) / SCREEN_W;
		sf::Vector2f floor = rayPos + rowDist * rayDirLeft;
		float brightnes = 1 - rowDist / BRIGHTNESTDIST;
		if (brightnes < 0.08f) brightnes = 0.08f;
		
		for (int x = 0; x < SCREEN_W; x++)
		{
			sf::Vector2i cell{ floor };
			sf::Vector2i textCoords{ (float)TEXTURE_SIZE * (floor - (sf::Vector2f)cell) };
			textCoords.x &= TEXTURE_SIZE - 1;
			textCoords.y &= TEXTURE_SIZE - 1;

			int floorText = map->GetOnGrid(cell.x, cell.y, FLOOR_LAYER);
			int cellingText = map->GetOnGrid(cell.x, cell.y, CELL_LAYER);

			sf::Color color;
			if (is_floor)
			{
				color = floorText == 0 ? sf::Color(0, 0, 0, 0) :
				Resources::textureImage.getPixel((floorText - 1) % TEXTURE_COUNT * TEXTURE_SIZE + textCoords.x, 
					(floorText - 1) / TEXTURE_COUNT * TEXTURE_SIZE + textCoords.y);
			}
			else
			{
				color = cellingText == 0 ? sf::Color(0, 0, 0, 0) :
					Resources::textureImage.getPixel((cellingText - 1) % (int)TEXTURE_COUNT * TEXTURE_SIZE + textCoords.x, 
						(cellingText - 1) / (int)TEXTURE_COUNT * TEXTURE_SIZE + textCoords.y);
			}
			screenPixels[(x + y * (int)SCREEN_W) * 4 + 0] = color.r * brightnes;
			screenPixels[(x + y * (int)SCREEN_W) * 4 + 1] = color.g * brightnes;
			screenPixels[(x + y * (int)SCREEN_W) * 4 + 2] = color.b * brightnes;
			screenPixels[(x + y * (int)SCREEN_W) * 4 + 3] = color.a;

			floor += floorStep;
		}

	}
}

