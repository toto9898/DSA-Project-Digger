#include "Bullet.h"



Bullet::Bullet(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Texture* emptyTexture, Move direction):
	Object(renderer,texture, emptyTexture),
	direction(direction)
{
}


Bullet::~Bullet()
{
}

bool Bullet::move(graph& map)
{

	//COORD playerXY(currBlock());

	//map[currBlock().y][currBlock().x];

	//// erasing walls if needed
	//if (coords().y % BLOCK_SIZE == 5)
	//{
	//	gMap[playerXY.y][playerXY.x].set(WALL_UP, false);

	//}
	//if (coords().y % BLOCK_SIZE == BLOCK_SIZE - height() - 5)
	//{
	//	gMap[playerXY.y][playerXY.x].set(WALL_DOWN, false);

	//}
	//if (coords().x % BLOCK_SIZE == 5)
	//	gMap[playerXY.y][playerXY.x].set(WALL_LEFT, false);
	//if (coords().x % BLOCK_SIZE == BLOCK_SIZE - width() - 5)
	//	gMap[playerXY.y][playerXY.x].set(WALL_RIGHT, false);

	//Object::move(direction);
	return false;
}
