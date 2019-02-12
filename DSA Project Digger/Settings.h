#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL.h>
#include <bitset>

const unsigned SCREEN_WIDTH = 1280;
const unsigned SCREEN_HEIGHT = 720;
const unsigned FPS = 30;
const unsigned SEC = 1000;
 
const int ENEMY_SPAWN_TIME = (SEC / FPS) * 2;

const short BLOCK_SIZE = 80;

const short STEP = 10;

const char PLAYER_TEXTURE_PATH[] = "Images\\player.png";
const char PLAYER_TEXTURE_EMPTY_PATH[] = "Images\\emptyPlayer.png";
const char ENEMY_TEXTURE_PATH[] = "Images\\enemy.png";
const char SOIL_TEXTURE_PATH[] = "Images\\soil.png";
const char TUNNEL_TEXTURE_PATH[] = "Images\\tunnel.png";
const char EMERALD_TEXTURE_PATH[] = "Images\\emerald.png";
const char BAG_TEXTURE_PATH[] = "Images\\bag.png";
const char BULLET_TEXTURE_PATH[] = "Images\\bullet.png";
const char WIN_TEXTURE_PATH[] = "Images\\win.png";
const char GAME_OVER_PATH[] = "Images\\gameover.png";
const char MAP_PATH[] = "map.txt";

const int SOIL           = 0;
const int EMERALD        = 1;
const int BAG            = 2;
const int UNLOCKED       = 3;
const int WALL_LEFT      = 4;
const int WALL_RIGHT     = 5;
const int WALL_UP        = 6;
const int WALL_DOWN      = 7;

// player, bullet, enemy, ulocked, left wall, right wall, up wall, down wall
// 0 0 0 0 0 0 0 0

// S - soil
// T - tunnel
// B - bag
// E - emerald

#define WINDOW_ARGS "Digger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN

const int playerStartX = (BLOCK_SIZE - 50) / 2;
const int playerStartY = (BLOCK_SIZE - 50) / 2 + BLOCK_SIZE * 3;

const int enemyStartX = SCREEN_WIDTH - BLOCK_SIZE + (BLOCK_SIZE - 50) / 2;
const int enemyStartY = (BLOCK_SIZE - 50) / 2 + BLOCK_SIZE * 2;

#endif // !SETTINGS_H
