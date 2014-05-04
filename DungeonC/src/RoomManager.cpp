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
	
	for(int i = 0; i < mGrid->getHeight(); i++)
	{
		for(int j = 0; j < mGrid->getWidth(); j++)
		{
			mUnvisitedTiles.push_front(mGrid->getTile(j,i));
		}
	}
}


void RoomManager::findRooms()
{
	int numRooms = 1;


	while(!mUnvisitedTiles.empty())
	{
		Room *room = new Room(numRooms);
		mRooms.push_back(room);
		exploreTile(*(mUnvisitedTiles.begin()), room, NORTH);
		numRooms++;
		if(numRooms > 300)
			break;
	}
}

void RoomManager::exploreTile(DungeonTile* dt, Room* r, Direction d)
{
	if(dt == NULL)
		return;

	switch(dt->getType())
	{
	case DungeonTile::CLEAR:
		if(r->addTile(dt))
		{
			mUnvisitedTiles.remove(dt);
			for(int i = NORTH; i < DIRECTION_COUNT; i++)
			{	
				switch(i)
				{
				case NORTH:
					if(static_cast<Direction>(i) != SOUTH)
						exploreTile(mGrid->getTile(dt->getX(), dt->getY() +1),r,NORTH);
					break;

				case SOUTH:
					if(static_cast<Direction>(i) != NORTH)
						exploreTile(mGrid->getTile(dt->getX(), dt->getY() -1),r,SOUTH);
					break;

				case WEST:				
					if(static_cast<Direction>(i) != EAST)
						exploreTile(mGrid->getTile(dt->getX()+1, dt->getY()),r,WEST);
					break;

				case EAST:
					if(static_cast<Direction>(i) != WEST)
						exploreTile(mGrid->getTile(dt->getX()-1, dt->getY() ),r,EAST);
					break;

				default:
					break;
				}
			}
		}
		break;

	case DungeonTile::WALL:

		mVisitedWalls.push_front(dt);
		break;

	default:
		break;
	}
}
