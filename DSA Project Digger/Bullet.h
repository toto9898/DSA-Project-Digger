#pragma once
#include "Object.h"
#include <array>
#include<bitset>
using std::bitset;
using std::array;


using graph = array<array<bitset<8>, SCREEN_WIDTH / BLOCK_SIZE>, SCREEN_HEIGHT / BLOCK_SIZE>;

class Bullet :
	public Object
{
public:
	Bullet(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Texture* emptyTexture, Move direction);
	virtual ~Bullet();

	bool move(graph& map);


private:
	Move direction;
};

