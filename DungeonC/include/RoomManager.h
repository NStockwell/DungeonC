//
//  RoomManager.h
//  DungeonCreator
//
//  Created by Nuno Moreira on 4/27/14.
//  Copyright (c) 2014 NunoMoreira. All rights reserved.
//

#ifndef __DungeonCreator__ROOMMANAGER__
#define __DungeonCreator__ROOMMANAGER__

#include "Grid.h"
#include "Room.h"
#include <vector>
#include <list>

typedef enum
{
	NORTH = 0,
		SOUTH = 1,
		WEST,
		EAST, 
		DIRECTION_COUNT
} Direction;

struct MergedRooms {
    int roomA;
    int roomB;
};

class RoomManager
{
public:
	RoomManager(Grid* g);
	void findRooms();
	void exploreTile(DungeonTile* dt, Room* r, Direction d);
	bool expandRooms(int range);
	bool expandRoom(Room* r, int range);
	void connectTheseTiles(DungeonTile* startingTile, DungeonTile* endTile, Room* r);
    bool checkThisTile(DungeonTile* dT, vector<DungeonTile*>tilesToCheck, vector<int>*roomsToMerge, Room* r);

private:
	Grid* mGrid;
	vector<Room*> mRooms;
	list<DungeonTile*> mUnvisitedTiles;
	list<DungeonTile*> mVisitedWalls;
    vector<MergedRooms> mMergedRooms;
};

#endif
