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
		exploreTile(mUnvisitedTiles.front(), room, NORTH);
		if(!room->isEmpty())
		{
			mRooms.push_back(room);
			numRooms++;
		}
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
            mUnvisitedTiles.remove(dt);
            mVisitedWalls.push_front(dt);
            break;
            
        default:
            break;
	}
}


void RoomManager::expandRooms()
{
	for each(Room* r in mRooms)
	{
		expandRoom(r, 2);
	}
}

void RoomManager::expandRoom(Room* r, int range)
{
	int westLimit = r->getWestLimit();
	int eastLimit = r->getEastLimit();
	int northLimit = r->getNorthLimit();
	int southLimit = r->getSouthLimit();

	vector<int> roomsToMerge;

	for(int i = westLimit;  i <= eastLimit; i++)
	{
		DungeonTile* dtN = r->getNorthernTileAtColumn(i);
		int checkingNorthIndex = dtN->getY() - range;
		DungeonTile* checkingNorthernTile = mGrid->getTile(i, checkingNorthIndex);
		bool found = false;
		if(checkingNorthernTile)
		{
			if(checkingNorthernTile->getType() != DungeonTile::WALL)
			{
				for each(int k in roomsToMerge) // find if the room is already on the rooms to merge
				{
					if(k == checkingNorthernTile->getRoomId())
					{
						found = true;
						break;
					}
				}

				if(!found)
				{
					r->addTile(mGrid->getTile(i,checkingNorthernTile->getY()+1));
					mGrid->getTile(i, checkingNorthernTile->getY()+1)->setType(DungeonTile::CLEAR);
					roomsToMerge.push_back(checkingNorthernTile->getRoomId());
				}
			}
		}
		 
		found = false;
		DungeonTile* dtS = r->getSouthernTileAtColumn(i);
		int checkingSIndex = dtS->getY() + range;
		DungeonTile* checkingSTile = mGrid->getTile(i, checkingSIndex);
		if(checkingSTile)
		{
			if(checkingSTile->getType() != DungeonTile::WALL)
			{
				for each(int k in roomsToMerge) // find if the room is already on the rooms to merge
				{
					if(k == checkingSTile->getRoomId())
					{
						found = true;
						break;
					}
				}

				if(!found)
				{
					r->addTile(mGrid->getTile(i,checkingSTile->getY()-1));
					mGrid->getTile(i, checkingSTile->getY()-1)->setType(DungeonTile::CLEAR);
					roomsToMerge.push_back(checkingSTile->getRoomId());
				}
			}
		}

	}


	
	for(int i = northLimit;  i <= southLimit; i++)
	{
		DungeonTile* dtW = r->getWesternTileAtLine(i);
		int checkingWIndex = dtW->getX() - range;
		DungeonTile* checkingWTile = mGrid->getTile(checkingWIndex,i );
		bool found = false;
		if(checkingWTile)
		{
			if(checkingWTile->getType() != DungeonTile::WALL)
			{
				for each(int k in roomsToMerge) // find if the room is already on the rooms to merge
				{
					if(k == checkingWTile->getRoomId())
					{
						found = true;
						break;
					}
				}

				if(!found)
				{
					r->addTile(mGrid->getTile(checkingWTile->getY()+1,i)); // connection between the two diff rooms
					mGrid->getTile(checkingWTile->getY()+1,i)->setType(DungeonTile::CLEAR); // clear the connection
					roomsToMerge.push_back(checkingWTile->getRoomId());
				}
			}
		}
		 
		found = false;
		DungeonTile* dtE = r->getEasternTileAtLine(i);
		int checkingEIndex = dtE->getX() + range;
		DungeonTile* checkingETile = mGrid->getTile(i, checkingEIndex);
		if(checkingETile)
		{
			if(checkingETile->getType() != DungeonTile::WALL)
			{
				for each(int k in roomsToMerge) // find if the room is already on the rooms to merge
				{
					if(k == checkingETile->getRoomId())
					{
						found = true;
						break;
					}
				}

				if(!found)
				{
					r->addTile(mGrid->getTile(checkingETile->getY()-1,i));
					mGrid->getTile(checkingETile->getY()-1, i)->setType(DungeonTile::CLEAR);
					roomsToMerge.push_back(checkingETile->getRoomId());
				}
			}
		}

	}

	for each(int roomId in roomsToMerge)
	{
		for each(Room* room in mRooms)
		{
			if(room->getId() == r->getId())
				continue;
			
			if(room->getId() == roomId)
			{
				room->setId(r->getId());
			}
		}
	}

}


void RoomManager::connectTheseTiles(DungeonTile* startingTile, DungeonTile* endTile)
{
	int startingRoomId = startingTile->getRoomId();

	int x1 = startingTile->getX();
	int y1 = startingTile->getY();

	int x2 = endTile->getX();
	int y2 = endTile->getY();

	int diffX = x2 - x1;
	int diffY = y2 - y1;


}