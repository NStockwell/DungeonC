//
//  Room.h
//  DungeonCreator
//
//  Created by Nuno Moreira on 4/27/14.
//  Copyright (c) 2014 NunoMoreira. All rights reserved.
//

#ifndef __DungeonCreator__ROOM__
#define __DungeonCreator__ROOM__

#include "DungeonTile.h"
#include <vector>

class Room
{

public:
	Room(int id);
	int getId();
	void setId(int roomId);
	bool addTile(DungeonTile* dt);
	void removeTile(DungeonTile* dt);
	bool isEmpty();

	DungeonTile* getEasternTileAtLine(int line);
	DungeonTile* getWesternTileAtLine(int line);
	DungeonTile* getSouthernTileAtColumn(int column);
	DungeonTile* getNorthernTileAtColumn(int column);

	int getNorthLimit();
	int getSouthLimit();
	int getWestLimit();
	int getEastLimit();


private:
	int mId;
	vector<DungeonTile*> mTiles;
	int mNorthLimit;
	int mSouthLimit;
	int mWestLimit;
	int mEastLimit;

};

#endif