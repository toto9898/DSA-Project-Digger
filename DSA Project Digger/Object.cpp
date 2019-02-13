#include <iostream>
#include "Object.h"

Object::Object(SDL_Renderer*& renderer, SDL_Texture* texture, SDL_Texture* emptyTexture):
	texture(texture),
	emptyTexture(emptyTexture),
	pos{0,0,0,0},
	curr(currBlock()),
	prev(currBlock())
{
	if (emptyTexture == nullptr)
		emptyTexture = texture;

	curr = prev = COORD(0, 0);

	SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
}


Object::~Object()
{
}

bool Object::draw(SDL_Renderer *& onto)
{
	SDL_RenderSetViewport(onto, &pos);

	SDL_RenderCopy(onto, texture, NULL, NULL);

	//SDL_RenderPresent(onto);
	return false;
}

bool Object::erase(SDL_Renderer *& from)
{
	SDL_RenderSetViewport(from, &pos);
	SDL_RenderCopy(from, emptyTexture, NULL, NULL);

	return false;
}

bool Object::move(Move _move)
{
	// if we are at the center of a block
	if ((coords().x % BLOCK_SIZE == (BLOCK_SIZE - width()) / 2) 
	&& (coords().y % BLOCK_SIZE == (BLOCK_SIZE - height()) / 2))
	{
		basicMove(_move);
	}
	else
	{
		// if our last move was on x axis
		// and we want to keep it that way
		if ((facing() == LEFT || facing() == RIGHT)
			&& (_move == LEFT || _move == RIGHT))
		{
			basicMove(_move);
		}
		// if our last move was on y axis
		// and we want to keep it that way
		else if ((facing() == UP || facing() == DOWN)
			&& (_move == UP || _move == DOWN))
		{
			basicMove(_move);
		}

		else if(_move.X || _move.Y)
		{ // keep the same direction
			basicMove(getClosestDir());
			return false;
		}
	}
	lastMove = _move;
	
	prev = curr;
	curr = currBlock();

	return false;
}

bool Object::moveBack()
{
	Move tempLastMove = lastMove, tempLastActiveMove = lastActiveMove;
	basicMove(Move(lastMove.X*-1, lastMove.Y*-1));

	lastMove = tempLastMove;
	lastActiveMove = tempLastActiveMove;
	return false;
}

void Object::setPos(COORD coord)
{
	pos.x = coord.x;
	pos.y = coord.y;
}

void Object::update(SDL_Renderer *& renderer, Move _move)
{
	erase(renderer);
	move(_move);
	draw(renderer);
}

bool Object::centered()
{
	if (center().x % BLOCK_SIZE == BLOCK_SIZE / 2 && center().y % BLOCK_SIZE == BLOCK_SIZE / 2)
		return true;
	return false;
}

int Object::width() const
{
	return pos.w;
}

int Object::height() const
{
	return pos.h;
}

COORD Object::currBlock()
{
	COORD quadrant(center());
	quadrant.x /= BLOCK_SIZE;
	quadrant.y /= BLOCK_SIZE;

	return quadrant;
}

COORD Object::prevBlock()
{
	return prev;
}

COORD Object::coords()
{
	return COORD(pos.x, pos.y);
}

COORD Object::center()
{
	return COORD(pos.x + width() / 2, pos.y + height() / 2);
}

bool Object::basicMove(Move _move)
{
	if ((pos.x >= STEP && _move.X < 0) || (pos.x < SCREEN_WIDTH - width() && _move.X > 0))
		pos.x += _move.X * STEP;

	if ((pos.y >= STEP && _move.Y < 0) || (pos.y < SCREEN_HEIGHT - height() && _move.Y > 0))
		pos.y += _move.Y * STEP;

	lastMove = _move;
	
	if(_move.X || _move.Y)
		lastActiveMove = _move;
	
	return false;
}

Move Object::facing()
{
	return lastActiveMove;
}

Move Object::getClosestDir()
{
	if (facing() == UP || facing() == DOWN)
	{
		if (center().y % BLOCK_SIZE >= BLOCK_SIZE / 2)
			return UP;
		else
			return DOWN;
	}
	else
	{
		if (center().x % BLOCK_SIZE >= BLOCK_SIZE / 2)
			return LEFT;
		else
			return RIGHT;
	}
	return Move();
}
