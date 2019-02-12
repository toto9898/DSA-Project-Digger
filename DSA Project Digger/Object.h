 #pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Settings.h"
#include "EaseOfUse.h"

class Object
{
public:
	Object(SDL_Renderer*& renderer, SDL_Texture* texture, SDL_Texture* emptyTexture = nullptr);
	virtual ~Object();

	virtual bool draw(SDL_Renderer*& onto);
	virtual bool erase(SDL_Renderer*& from);
	bool move(Move _move);
	bool moveBack();

	virtual void setPos(COORD coord);

	virtual void update(SDL_Renderer*& renderer, Move move);

	bool centered();
	virtual int width() const;
	virtual int height() const;

	virtual	COORD currBlock();
	virtual	COORD prevBlock();
	virtual COORD coords();
	virtual COORD center();

	Move facing();

private:
	bool basicMove(Move _move);
	Move getClosestDir();
private:
	SDL_Rect pos;
	SDL_Texture* texture;
	SDL_Texture* emptyTexture;

	Move lastMove;
	Move lastActiveMove;

	COORD curr;
	COORD prev;
};

