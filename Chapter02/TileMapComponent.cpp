#include "TileMapComponent.h"
#include "Actor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner)
{
    //game->GetTexture("Assets/Character01.png"),
    std::vector<std::vector<std::string>> data = loadCSV("Assets/MapLayer1.csv");
    int temp = 1;
}

TileMapComponent::~TileMapComponent()
{
	
}

std::vector<std::string> TileMapComponent::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::vector<std::string>> TileMapComponent::loadCSV(const std::string& filename)
{
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string line;
    while (std::getline(file, line)) {
        data.push_back(split(line, ','));
    }
    file.close();

    return data;
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
}
void TileMapComponent::SetTexture(SDL_Texture* texture)
{

}