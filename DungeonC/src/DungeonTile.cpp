
#include "DungeonTile.h"


DungeonTile::DungeonTile(int x, int y, TileType tt)
{
	mX = x;
	mY = y; 
	mType = tt;
}


int DungeonTile::getX()
{
	return mX;
}

int DungeonTile::getY()
{
	return mY;
}

DungeonTile::TileType DungeonTile::getType()
{
	return mType;
}
void DungeonTile::setType(TileType type)
{
	mType = type;
}

void DungeonTile::print(FILE* f)
{
	switch (mType)
	{
		case TileType::CLEAR:
		{
			fprintf(f,".");
			break;
		}
		case TileType::WALL:
		{
			fprintf(f, "@");
			break;
		}
		default:
			break;
	}
}