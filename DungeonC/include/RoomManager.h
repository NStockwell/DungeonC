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

class RoomManager
{
public:
	RoomManager(Grid* g);
	void findRooms();

private:
	Grid* mGrid;
	vector<Room*> mRooms;
};

#endif
