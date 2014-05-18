#ifndef DUNGEONTILE_H
#define DUNGEONTILE_H

#include <iostream>

using namespace std;



class DungeonTile
{
    

public:
    
    typedef	enum
	{
		INVALID = -1,
		WALL = 0,
		CLEAR,
		TILE_TYPE_COUNT
	}TileType;

    
	DungeonTile(int x, int y, TileType tt);
	int getX();
	int getY();
	
	TileType getType();
	
	void setType(TileType type);
	void print(FILE* f);

	void setRoomId(int room);
	int getRoomId();
	
	bool isEqual(DungeonTile* dt1);

private:
	int mX;
	int mY;
	TileType mType;
	int mRoomId;
};

#endif