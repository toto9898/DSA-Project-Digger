#pragma once
#include <vector>
#include <array>
#include <bitset>
#include "Object.h"

using std::vector;
using std::array;
using std::bitset;
using std::pair;

using graph = array<array<bitset<8>, SCREEN_WIDTH / BLOCK_SIZE>, SCREEN_HEIGHT / BLOCK_SIZE>;
using visitedMatrix = array<array<pair<bool, unsigned>, SCREEN_WIDTH / BLOCK_SIZE>, SCREEN_HEIGHT / BLOCK_SIZE>;
using element = pair<COORD, unsigned>;

class Enemy :
	public Object
{
public:
	Enemy(SDL_Renderer*& renderer, SDL_Texture* texture, SDL_Texture* emptyTexture);
	virtual ~Enemy();

	bool move();

	void getPath(graph& map, visitedMatrix& visited, COORD end);

private:
	bool reachable(graph& g, COORD block1, COORD block2);
private:
	vector<Move> path;
};

