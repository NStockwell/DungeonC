//
//  RoomManager.cpp
//  DungeonCreator
//
//  Created by Nuno Moreira on 4/27/14.
//  Copyright (c) 2014 NunoMoreira. All rights reserved.
//

#include "RoomManager.h"

RoomManager::RoomManager(Grid* g)
{
	mGrid = g;
	mRooms = vector<Room*>();
}


void RoomManager::findRooms()
{
	int width = mGrid->getWidth();
	int height = mGrid->getHeight();
	int roomsFound = 0;

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			DungeonTile::TileType dtt = mGrid->getTileType(i,j);
			switch (dtt)
			{
			case DungeonTile::WALL:

				break;

			case DungeonTile::CLEAR:

				break;

			default:
				break;
			}

			
		}
	}
}

