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
	void addTile(DungeonTile* dt);
	void removeTile(DungeonTile* dt);

	DungeonTile* getEasternTileAtLine(int line);
	DungeonTile* getWesternTileAtLine(int line);
	DungeonTile* getSoutherTileAtColumn(int column);
	DungeonTile* getNorthernTileAtColumn(int column);



private:
	int mId;
	vector<DungeonTile*> mTiles;

};

#endif