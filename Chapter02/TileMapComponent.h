#pragma once
#include "SpriteComponent.h"
#include <string>

class TileMapComponent : public SpriteComponent
{
public:
	TileMapComponent(class Actor* owner, int drawOrder = 100);
	~TileMapComponent();

	std::vector<std::string> split(const std::string& s, char delimiter);
	std::vector<std::vector<std::string>> loadCSV(const std::string& filename);

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);
};

