#include <queue>
#include "Enemy.h"
using std::queue;
using std::pair;

Enemy::Enemy(SDL_Renderer*& renderer, SDL_Texture* texture, SDL_Texture* emptyTexture):
	Object(renderer, texture, emptyTexture)
{
}


Enemy::~Enemy()
{
}

bool Enemy::move()
{
	if (path.size())
		Object::move(path[0]);
	
	if (path.size())
		if (centered())
			path.erase(path.begin());

	return false;
}

void Enemy::getPath(graph& map, visitedMatrix & visited, COORD end)
{
	COORD currCoord(currBlock());

	path.clear();
	while (!(currCoord == end))
	{
		unsigned currLevel = visited[currCoord.y][currCoord.x].second;

		for (size_t k = 0; k < 4; k++)
		{
			COORD child(currCoord + MOVES[k]);

			if (inRange_Blocks(child) && visited[child.y][child.x].second < currLevel && reachable(map, currCoord, child) )
			{
				path.push_back(Move(MOVES[k]));
				currCoord = child;
				break;
			}
		}
	}
}

bool Enemy::reachable(graph & g, COORD block1, COORD block2)
{
	if (block1.x != block2.x)
	{
		if (block1.x > block2.x)
			return ((g[block1.y][block1.x][WALL_LEFT] == false) && (g[block2.y][block2.x][WALL_RIGHT] == false));
		else
			return ((g[block1.y][block1.x][WALL_RIGHT] == false) && (g[block2.y][block2.x][WALL_LEFT] == false));
	}
	if (block1.y != block2.y)
	{
		if(block1.y > block2.y)
			return ((g[block1.y][block1.x][WALL_UP] == false && g[block2.y][block2.x][WALL_DOWN] == false));
		else
			return ((g[block1.y][block1.x][WALL_DOWN] == false) && (g[block2.y][block2.x][WALL_UP] == false));
	}
	return false;
}
