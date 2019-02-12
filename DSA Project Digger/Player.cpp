#include "Player.h"


Player::Player(SDL_Renderer*& renderer, SDL_Texture* texture, SDL_Texture* emptyTexture):
	Object(renderer, texture, emptyTexture)
{
	setPos(COORD(playerStartX, playerStartY));
}


Player::~Player()
{
}
