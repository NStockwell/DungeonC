
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

void DungeonTile::setRoomId(int room)
{
	mRoomId = room;
}
int DungeonTile::getRoomId()
{
	return mRoomId;
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

bool DungeonTile::isEqual(DungeonTile* dt1)
{
	if(dt1 == NULL || dt1->getX() != mX || dt1->getY() != mY || dt1->getType() != mType)
		return false;

	return true;
}