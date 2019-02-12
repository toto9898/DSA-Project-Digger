#pragma once
#include "Object.h"
class Player :
	public Object
{
public:
	Player(SDL_Renderer*& renderer, SDL_Texture* texture, SDL_Texture* emptyTexture);
	virtual ~Player();
};

