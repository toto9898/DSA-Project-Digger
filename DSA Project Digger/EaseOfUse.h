#pragma once
#include "Settings.h"

struct Move
{
	Move() : X(0), Y(0) {}
	Move(short X, short Y) : X(X), Y(Y) {}
	bool operator==(const Move& rhs)
	{
		return (X == rhs.X && Y == rhs.Y);
	}
	Move operator!()
	{
		Move temp(*this);
		temp.X *= -1;
		temp.Y *= -1;

		return temp;
	}

	void reset() { X = 0; Y = 0; }
	short X;
	short Y;
};

const Move RIGHT(1, 0);
const Move LEFT(-1, 0);
const Move DOWN(0, 1);
const Move UP(0, -1);

const Move MOVES[4] = { RIGHT, LEFT, DOWN, UP };

struct COORD
{
	COORD(short x = 0, short y = 0):
		x(x),
		y(y)
	{}
	COORD(const COORD& rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}

	COORD operator+(const Move& rhs)
	{
		COORD temp(*this);
		temp.x += rhs.X;
		temp.y += rhs.Y;
		return temp;
	}
	bool operator==(const COORD& rhs)
	{
		return (x == rhs.x && y == rhs.y);
	}

	short x, y;
};


const struct Textures
{
	Textures(SDL_Renderer* renderer):
		playerTexture(IMG_LoadTexture(renderer, PLAYER_TEXTURE_PATH)),
		emptyPlayerTexture(IMG_LoadTexture(renderer, PLAYER_TEXTURE_EMPTY_PATH)),
		enemyTexture(IMG_LoadTexture(renderer, ENEMY_TEXTURE_PATH)),
		soilTexture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT)),
		emeraldTexture(IMG_LoadTexture(renderer, EMERALD_TEXTURE_PATH)),
		bagTexture(IMG_LoadTexture(renderer, BAG_TEXTURE_PATH)),
		bulletTexture(IMG_LoadTexture(renderer, BULLET_TEXTURE_PATH)),
		tunnelBlockTexture(IMG_LoadTexture(renderer, TUNNEL_TEXTURE_PATH)),
		gameOverTexture(IMG_LoadTexture(renderer, GAME_OVER_PATH)),
		winTexture(IMG_LoadTexture(renderer, WIN_TEXTURE_PATH))
	{
	}
	SDL_Texture* playerTexture;
	SDL_Texture* emptyPlayerTexture;
	SDL_Texture* enemyTexture;
	SDL_Texture* soilTexture;
	SDL_Texture* emeraldTexture;
	SDL_Texture* bagTexture;
	SDL_Texture* bulletTexture;
	SDL_Texture* tunnelBlockTexture;
	SDL_Texture* gameOverTexture;
	SDL_Texture* winTexture;

	~Textures()
	{
		SDL_DestroyTexture(playerTexture);
		SDL_DestroyTexture(emptyPlayerTexture);
		SDL_DestroyTexture(soilTexture);
		SDL_DestroyTexture(emeraldTexture);
		SDL_DestroyTexture(bagTexture);
		SDL_DestroyTexture(bulletTexture);
		SDL_DestroyTexture(tunnelBlockTexture);
		SDL_DestroyTexture(gameOverTexture);
		SDL_DestroyTexture(winTexture);
	}
};

static bool inRange_Blocks(COORD coord)
{
	if (coord.x < 0 || coord.y < 0) return false;
	if (coord.x >= SCREEN_WIDTH / BLOCK_SIZE || coord.y >= SCREEN_HEIGHT / BLOCK_SIZE) return false;

	return true;
}

static void Delay()
{
	SDL_Delay(SEC / FPS);
}
