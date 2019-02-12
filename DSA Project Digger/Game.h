#pragma once
#include <vector>
#include <array>
#include <bitset>
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

using std::vector;
using std::array;
using std::bitset;
using std::pair;

using graph = array<array<bitset<8>, SCREEN_WIDTH / BLOCK_SIZE>, SCREEN_HEIGHT / BLOCK_SIZE>;
using edge = pair<COORD, COORD>;

class Game
{
public:
	Game(SDL_Window*& window);
	~Game();

	void update(Move move, bool shoot);

	void loadMap();
	void loadMap_random();
	void loadMapInGraph();
	void loadRandomMap();

// functions
private:
	void fillWithSoil(graph& map);
	void loadBlock(size_t row, size_t col, Object& obj, char objectType);
	COORD position(Object& obj);
	void showSoilLayer();
	void loadSoilBlockInGraph(graph& map, size_t row, size_t col);
	void loadTunnelBlockInGraph(size_t row, size_t col);
	char getBlockType(bitset<8>& block);
	void deleteWalls();
	bool collision(Object& obj1, Object& obj2);
	void spawnEnemy();
	void addEmerald(COORD coord);
	void addBullet();

	// functions for the pathfinding
	void setEnemyPaths();
	bool isValid(graph & g, visitedMatrix& visited, COORD coord, COORD oldCoord);
	void printVisited(visitedMatrix& visited);


	// functions for generating random maze --- NOT READY
	void mazeGenerationPrim();
	void addWalls(vector<edge>& wallList, graph& map, COORD block);
	void makeWallPassage(graph& map, edge& e);
	void markAsVisited(graph& map, edge& e);
	void addNeighbourWalls(vector<edge>& wallList, graph& map, edge& e);

	void showWallPassage(graph& map, edge& e);
	void showVisited(graph& map, edge& e);
	void showSoil(graph& map);

// SDL stuff
private:
	SDL_Renderer* renderer;
	Textures textures;

// Game objects
private:
	Player player;
	vector<Object> emeralds;
	vector<Object> bags;

	vector<Enemy> enemies;
	vector<Bullet> bullets;

	graph gMap;

	Object tunnel;

	int enemySpownTimer;
};

