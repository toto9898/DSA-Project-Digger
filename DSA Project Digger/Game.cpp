#include "Game.h"
#include <fstream>
#include <string>
#include <queue>
#include<exception>
#include <ctime>
#include <iostream>
#include <iomanip>
using std::cout;
using std::string;
using std::queue;
using std::ifstream;

Game::Game(SDL_Window*& window):
	renderer(SDL_CreateRenderer(window, -1, IMG_INIT_PNG)),
	textures(renderer),
	player(renderer, textures.playerTexture, textures.emptyPlayerTexture),
	tunnel(renderer, textures.tunnelBlockTexture),
	enemySpownTimer(0)
{
	srand(time(0));

	//loadMapInGraph();
	//loadMap();
	loadRandomMap();
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
}

void Game::update(Move move, bool shoot)
{
	for (Enemy& enemy : enemies)
	{
		if (collision(player, enemy))
		{
			player.erase(renderer);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			SDL_RenderDrawRect(renderer, &rect);

			Object gameover(renderer, textures.gameOverTexture);
			gameover.draw(renderer);

			SDL_RenderPresent(renderer);

			return;
		}
	}

	if (emeralds.empty())
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

		SDL_RenderDrawRect(renderer, &rect);

		Object win(renderer, textures.winTexture);
		win.draw(renderer);

		SDL_RenderPresent(renderer);

		return;
	}

	player.move(move);

	for (auto it = emeralds.begin(); it != emeralds.end(); ++it)
	{
		if (position(*it) == position(player))
		{
			emeralds.erase(it);
			break;
		}
	}

	player.moveBack();

	SDL_SetRenderTarget(renderer, textures.soilTexture);
	player.erase(renderer);

	showSoilLayer();

	player.move(move);
	// set block as unlocked, if we reach it's center
	if (player.centered())
		gMap[player.currBlock().y][player.currBlock().x].set(UNLOCKED);

	// remove a wall if we went through it
	deleteWalls();

	spawnEnemy();

	//if(!(player.prevBlock() == player.currBlock()))
		setEnemyPaths();
	
	for (auto& enemy : enemies)
	{
		enemy.erase(renderer);
		enemy.move();
		enemy.draw(renderer);
	}


	// drawing all the objects
	for (Object& emerald : emeralds)
		emerald.draw(renderer);

	for (Object& bag : bags)
		bag.draw(renderer);

	player.draw(renderer);

	// displaying the results to the window
	SDL_RenderPresent(renderer);
}

void Game::loadMap()
{
	SDL_Texture* soilBlockTexture = IMG_LoadTexture(renderer, SOIL_TEXTURE_PATH);
	Object soilBlock(renderer, soilBlockTexture);

	SDL_SetRenderTarget(renderer, textures.soilTexture);

	size_t i = 0;
	size_t j = 0;
	
	// reading the graph line by line and loading the textures
	for (auto& row : gMap)
	{
		j = 0;
		for (auto& block : row)
		{
			if (block[SOIL] == true)
				loadBlock(i, j, soilBlock, getBlockType(block));
			else
				loadBlock(i, j, tunnel, getBlockType(block));

			++j;
		}
		++i;
	}
	
	showSoilLayer();

	// drawing all the objects
	for (Object& emerald : emeralds)
		emerald.draw(renderer);

	for (Object& bag : bags)
		bag.draw(renderer);

	for (Enemy& enemy : enemies)
		enemy.draw(renderer);

	player.draw(renderer);

	// displaying the results to the window
	SDL_RenderPresent(renderer);
}

void Game::loadMap_random()
{
	showSoilLayer();

	// drawing all the objects
	for (Object& emerald : emeralds)
		emerald.draw(renderer);

	for (Object& bag : bags)
		bag.draw(renderer);

	for (Enemy& enemy : enemies)
		enemy.draw(renderer);

	player.draw(renderer);

	// displaying the results to the window
	SDL_RenderPresent(renderer);
}

void Game::loadMapInGraph()
{
	ifstream in(MAP_PATH);
	if (!in)
		throw std::exception("The file with the map did not open");

	string line;

	// reading the mao file line by line
	for (size_t i = 0; getline(in, line); i++)
	{
		size_t j = 0;
		if (line.length() != SCREEN_WIDTH / BLOCK_SIZE)
			throw std::exception("Incorrect map file");

		// for every char, load the corresponding block in the graph
		for (char blockType : line)
		{
			if (blockType != 'T')
				loadSoilBlockInGraph(gMap, i, j);
			else
				loadTunnelBlockInGraph(i, j);

			if (blockType == 'B')
				gMap[i][j].set(BAG);
			if(blockType == 'E')
				gMap[i][j].set(EMERALD);
			
			++j;
		}
	}
	in.clear();
	in.close();
}

void Game::loadRandomMap()
{
	mazeGenerationPrim();
	loadMap_random();
}

void Game::fillWithSoil(graph & map)
{
	size_t i = 0, j = 0;
	// setting all the walls up
	for (auto& row : map)
	{
		j = 0;
		for (auto& vertex : row)
		{
			loadSoilBlockInGraph(map, i, j);
			++j;
		}
		++i;
	}
}

void Game::loadBlock(size_t row, size_t col, Object& obj, char objectType)
{
	int offsetX = 0;
	int offsetY = 0;

	if (objectType == 'E')
	{
		// we make the emerald to be in the center of the block
		emeralds.push_back(Object(renderer, textures.emeraldTexture));
		offsetX = (BLOCK_SIZE - emeralds.back().width()) / 2;
		offsetY = (BLOCK_SIZE - emeralds.back().height()) / 2;

		COORD objectCoords(BLOCK_SIZE * col + offsetX, BLOCK_SIZE * row + offsetY);

		emeralds.back().setPos(objectCoords);
	}
	if(objectType == 'B')
	{
		// we make the bag to be in the center of the block
		bags.push_back(Object(renderer, textures.bagTexture));
		offsetX = (BLOCK_SIZE - bags.back().width()) / 2;
		offsetY = (BLOCK_SIZE - bags.back().height()) / 2;

		COORD objectCoords(BLOCK_SIZE * col + offsetX, BLOCK_SIZE* row + offsetY);
		bags.back().setPos(objectCoords);
	}
	COORD objectCoords(BLOCK_SIZE * col, BLOCK_SIZE * row);

	obj.setPos(objectCoords);
	obj.draw(renderer);
}

COORD Game::position(Object & obj)
{
	return obj.currBlock();
}

void Game::showSoilLayer()
{
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderSetViewport(renderer, NULL);
	SDL_RenderCopy(renderer, textures.soilTexture, NULL, NULL);

	//SDL_RenderPresent(renderer);
}

void Game::loadSoilBlockInGraph(graph& map, size_t row, size_t col)
{
	map[row][col].set(UNLOCKED, false);
	map[row][col].set(SOIL);
	map[row][col].set(WALL_DOWN);
	map[row][col].set(WALL_LEFT);
	map[row][col].set(WALL_RIGHT);
	map[row][col].set(WALL_UP);
}

void Game::loadTunnelBlockInGraph(size_t row, size_t col)
{
	gMap[row][col].set(SOIL, false);
	gMap[row][col].set(UNLOCKED);

}

char Game::getBlockType(bitset<8>& block)
{
	if (block[SOIL] == false) return 'T';
	if (block[EMERALD] == true) return 'E';
	if (block[BAG] == true) return 'B';

	return 'S';
}

void Game::deleteWalls()
{
	COORD playerXY(player.currBlock());

	gMap[player.currBlock().y][player.currBlock().x];

	// erasing walls if needed
	if (player.coords().y % BLOCK_SIZE == 5)
		gMap[playerXY.y][playerXY.x].set(WALL_UP, false);
	if (player.coords().y % BLOCK_SIZE == BLOCK_SIZE - player.height() - 5)
		gMap[playerXY.y][playerXY.x].set(WALL_DOWN, false);
	if (player.coords().x % BLOCK_SIZE == 5)
		gMap[playerXY.y][playerXY.x].set(WALL_LEFT, false);
	if (player.coords().x % BLOCK_SIZE == BLOCK_SIZE - player.width() - 5)
		gMap[playerXY.y][playerXY.x].set(WALL_RIGHT, false);

	// if we ulocked the block and it does't have any walls
	// erase the remainings
	if (gMap[playerXY.y][playerXY.x][UNLOCKED]
		&& gMap[playerXY.y][playerXY.x][WALL_DOWN] == false && gMap[playerXY.y][playerXY.x][WALL_LEFT] == false && gMap[playerXY.y][playerXY.x][WALL_RIGHT] == false && gMap[playerXY.y][playerXY.x][WALL_UP] == false)
	{
		SDL_SetRenderTarget(renderer, textures.soilTexture);
		tunnel.setPos(COORD(playerXY.x * BLOCK_SIZE, playerXY.y * BLOCK_SIZE));
		tunnel.draw(renderer);

		showSoilLayer();
	}
}

bool Game::collision(Object & obj1, Object & obj2)
{
	COORD c1(obj1.coords());
	COORD c2(obj2.coords());
	
	if (c2.x + obj2.width() > c1.x && c2.x < c1.x + obj1.width())
	{
		if (c2.y + obj2.height() > c1.y && c2.y < c1.y + obj1.height())
			return true;
	}
	return false;
}

void Game::spawnEnemy()
{
	if (enemySpownTimer == 0)
	{
		enemies.push_back(Enemy(renderer, textures.enemyTexture, textures.emptyPlayerTexture));
		enemies.back().setPos(COORD(enemyStartX, enemyStartY));

		enemySpownTimer = ENEMY_SPAWN_TIME;
		setEnemyPaths();
	}
	else
		--enemySpownTimer;
}

void Game::addEmerald(COORD coord)
{
	int offsetX = 0;
	int offsetY = 0;

	emeralds.push_back(Object(renderer, textures.emeraldTexture));
	offsetX = (BLOCK_SIZE - emeralds.back().width()) / 2;
	offsetY = (BLOCK_SIZE - emeralds.back().height()) / 2;

	COORD objectCoords(BLOCK_SIZE * coord.x + offsetX, BLOCK_SIZE * coord.y + offsetY);

	emeralds.back().setPos(objectCoords);

	gMap[coord.y][coord.x].set(EMERALD);

}

void Game::addBullet()
{
	Move direction = player.facing();

	bullets.push_back(Bullet(renderer, textures.bulletTexture, textures.bulletTexture, direction));

	bullets.back().setPos(player.coords());
}

void Game::setEnemyPaths()
{
	visitedMatrix visited;
	for (auto& row : visited)
	{
		std::fill(row.begin(), row.end(), std::make_pair(false, INT_MAX));
	}

	queue<element> que;

	visited[player.currBlock().y][player.currBlock().x].first = true;
	visited[player.currBlock().y][player.currBlock().x].second = 0;
	que.push(element(std::make_pair(player.currBlock(), 0)));

	int minDist = INT_MAX;

	while (!que.empty())
	{
		// pop front node from queue and process it
		element node = que.front();
		que.pop();

		// currCoord represents current cell and dist stores its
		// minimum distance from the source
		COORD currCoord(node.first.x, node.first.y);

		// check for all 4 possible movements from current cell
		// and enqueue each valid movement
		for (int k = 0; k < 4; k++)
		{
			if (isValid(gMap, visited, currCoord + MOVES[k], currCoord))
			{
				COORD newCoord(currCoord.x + MOVES[k].X, currCoord.y + MOVES[k].Y);
				// mark next cell as visited and enqueue it
				visited[newCoord.y][newCoord.x].first = true;
				visited[newCoord.y][newCoord.x].second = visited[currCoord.y][currCoord.x].second + 1;

				que.push(element(newCoord, visited[newCoord.y][newCoord.x].second));
			}
		}
	}

	//printVisited(visited);

	for(Enemy& enemy: enemies)
		enemy.getPath(gMap, visited, player.currBlock());
}

bool Game::isValid(graph & g, visitedMatrix & visited, COORD coord, COORD oldCoord)
{
	bool valid = (coord.y >= 0) && (coord.y < g.size()) && (coord.x >= 0) && (coord.x < g[0].size());

	valid = valid && g[coord.y][coord.x][UNLOCKED] && !visited[coord.y][coord.x].first;

	if (valid == false) return valid;

	Move m(coord.x - oldCoord.x, coord.y - oldCoord.y);

	if (m.X == 1 && (g[oldCoord.y][oldCoord.x][WALL_RIGHT] || g[coord.y][coord.x][WALL_LEFT]))
		valid = false;
	if (m.X == -1 && (g[oldCoord.y][oldCoord.x][WALL_LEFT] || g[coord.y][coord.x][WALL_RIGHT]))
		valid = false;
	if (m.Y == 1 && (g[oldCoord.y][oldCoord.x][WALL_DOWN] || g[coord.y][coord.x][WALL_UP]))
		valid = false;
	if (m.Y == -1 && (g[oldCoord.y][oldCoord.x][WALL_UP] || g[coord.y][coord.x][WALL_DOWN]))
		valid = false;

	return valid;
}

void Game::printVisited(visitedMatrix & visited)
{
	size_t i = 0, j = 0;
	for (auto& row : visited)
	{
		j = 0;
		for (auto& el : row)
		{
			cout << std::setw(5) << el.second;
			++j;
		}
		cout << '\n';
		++i;
	}
}

void Game::mazeGenerationPrim()
{
	//graph gMap;

	fillWithSoil(gMap);

	showSoil(gMap);

	vector<edge> wallList;

	COORD currBlock(player.currBlock());
	gMap[currBlock.y][currBlock.x].set(UNLOCKED);

	addWalls(wallList, gMap, currBlock);

	while (!wallList.empty())
	{
		// picking random wall
		size_t i = rand() % wallList.size();

		// If only one of the two blocks that the wall divides is visited
		if (gMap[wallList[i].first.y][wallList[i].first.x][UNLOCKED] == false || gMap[wallList[i].second.y][wallList[i].second.x][UNLOCKED] == false)
		{
			makeWallPassage(gMap, wallList[i]);
			showWallPassage(gMap, wallList[i]);

			addNeighbourWalls(wallList, gMap, wallList[i]);

			markAsVisited(gMap, wallList[i]);
			showVisited(gMap, wallList[i]);

			SDL_RenderPresent(renderer);
			SDL_Delay(SEC / 100);
		}
		// remove the wall
		std::swap(wallList[0], wallList[i]);
		wallList.erase(wallList.begin());
	}
	return;
}

void Game::addWalls(vector<edge>& wallList, graph& map, COORD block)
{
	// add left wall
	if (block.x != 0 && map[block.y][block.x - 1][UNLOCKED] == false)
	{
		COORD leftWall_left(block.x - 1, block.y);
		COORD leftWall_right(block);
		wallList.push_back(edge(leftWall_left, leftWall_right));
	}
	// add up wall
	if (block.y != 0 && map[block.y - 1][block.x][UNLOCKED] == false)
	{
		COORD upWall_up(block.x, block.y - 1);
		COORD upWall_down(block);
		wallList.push_back(edge(upWall_up, upWall_down));
	}
	// add right wall
	if (block.x < map[0].max_size() - 1 && map[block.y][block.x + 1][UNLOCKED] == false)
	{
		COORD rightWall_left(block);
		COORD rightWall_right(block.x + 1, block.y);
		wallList.push_back(edge(rightWall_left, rightWall_right));
	}
	// add down wall
	if (block.y < map.max_size() - 1 && map[block.y + 1][block.x][UNLOCKED] == false)
	{
		COORD downWall_up(block);
		COORD downWall_down(block.x, block.y + 1);
		wallList.push_back(edge(downWall_up, downWall_down));
	}
}

void Game::makeWallPassage(graph & map, edge & e)
{
	if (e.first.x != e.second.x)
	{
		if (e.first.x > e.second.x)
		{
			map[e.first.y][e.first.x].set(WALL_LEFT, false);
			map[e.second.y][e.second.x].set(WALL_RIGHT, false);
		}
		else
		{
			map[e.first.y][e.first.x].set(WALL_RIGHT, false);
			map[e.second.y][e.second.x].set(WALL_LEFT, false);
		}
	}
	if (e.first.y != e.second.y)
	{
		if (e.first.y > e.second.y)
		{
			map[e.first.y][e.first.x].set(WALL_UP, false);
			map[e.second.y][e.second.x].set(WALL_DOWN, false);
		}
		else
		{
			map[e.first.y][e.first.x].set(WALL_DOWN, false);
			map[e.second.y][e.second.x].set(WALL_UP, false);
		}
	}
}

void Game::markAsVisited(graph & map, edge & e)
{	
	map[e.first.y][e.first.x].set(UNLOCKED);
	map[e.second.y][e.second.x].set(UNLOCKED);

	map[e.first.y][e.first.x].set(SOIL, false);
	map[e.second.y][e.second.x].set(SOIL, false);

	if (rand() % 40 == 0)
	{
		addEmerald(e.second);
	}
}

void Game::addNeighbourWalls(vector<edge>& wallList, graph & map, edge & e)
{
	if (map[e.first.y][e.first.x][UNLOCKED] == false)
		addWalls(wallList, map, e.first);
	else
		addWalls(wallList, map, e.second);
}

void Game::showWallPassage(graph & map, edge & e)
{
	Object walls(renderer, textures.emptyPlayerTexture);
	if (e.first.x != e.second.x)
	{
		if (e.first.x > e.second.x)
			walls.setPos(COORD(e.first.x * BLOCK_SIZE - walls.width() / 2, e.first.y * BLOCK_SIZE + (BLOCK_SIZE - walls.height()) / 2));
		else
			walls.setPos(COORD(e.second.x * BLOCK_SIZE - walls.width() / 2, e.second.y * BLOCK_SIZE + (BLOCK_SIZE - walls.height()) / 2));
	}
	if (e.first.y != e.second.y)
	{
		if (e.first.y > e.second.y)
			walls.setPos(COORD(e.first.x * BLOCK_SIZE + (BLOCK_SIZE - walls.width()) / 2, e.first.y * BLOCK_SIZE - walls.height() / 2));
		else
			walls.setPos(COORD(e.second.x * BLOCK_SIZE + (BLOCK_SIZE - walls.width()) / 2, e.second.y * BLOCK_SIZE - walls.height() / 2));
	}
	SDL_SetRenderTarget(renderer, textures.soilTexture);
	walls.draw(renderer);
	showSoilLayer();
}

void Game::showVisited(graph & map, edge & e)
{
	Object block(renderer, textures.emptyPlayerTexture);

	SDL_SetRenderTarget(renderer, textures.soilTexture);

	block.setPos(COORD(e.first.x * BLOCK_SIZE + (BLOCK_SIZE - block.width()) / 2, e.first.y * BLOCK_SIZE + (BLOCK_SIZE - block.height())/ 2));
	block.draw(renderer);

	block.setPos(COORD(e.second.x * BLOCK_SIZE + (BLOCK_SIZE - block.width()) / 2, e.second.y * BLOCK_SIZE + (BLOCK_SIZE - block.height()) / 2));
	block.draw(renderer);

	showSoilLayer();	
}

void Game::showSoil(graph & map)
{
	SDL_Texture* soilBlockTexture = IMG_LoadTexture(renderer, SOIL_TEXTURE_PATH);
	Object soilBlock(renderer, soilBlockTexture);

	SDL_SetRenderTarget(renderer, textures.soilTexture);

	size_t i = 0;
	size_t j = 0;

	for (auto& row : gMap)
	{
		j = 0;
		for (auto& block : row)
		{
			if (block[SOIL] == true)
				loadBlock(i, j, soilBlock, getBlockType(block));
			++j;
		}
		++i;
	}
	showSoilLayer();
}
