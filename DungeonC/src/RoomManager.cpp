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


bool RoomManager::expandRooms()
{
	bool expandedAtLeastOneRoom = false;
	for each(Room* r in mRooms)
	{
		expandedAtLeastOneRoom = expandRoom(r, 2) | expandedAtLeastOneRoom;
	}

	if(expandedAtLeastOneRoom)
	{
		for each(Room* r in mRooms)
		{	
			for each(Room* r2 in mRooms)
			{
				if(r2->getId() == r->getId() && r != r2)
				{
					vector<DungeonTile*> addTilesToOriginalRoom = r2->getTiles();
					for(int i = 0; i < addTilesToOriginalRoom.size(); i++)
					{
						r->addTile(addTilesToOriginalRoom.at(i));
					}
					r2->clearTiles();
				}
			}
		}

		vector<Room*> newRooms = vector<Room*>();
		for each(Room* r in mRooms)
		{
			if(!r->isEmpty())
			{
				newRooms.push_back(r);
			}
		}
		mRooms.clear();
		mRooms = newRooms;
	}
	return expandedAtLeastOneRoom;
}

bool RoomManager::expandRoom(Room* r, int range)
{
	bool expandedThisRoom = false;

	int westLimit = r->getWestLimit();
	int eastLimit = r->getEastLimit();
	int northLimit = r->getNorthLimit();
	int southLimit = r->getSouthLimit();

	vector<int> roomsToMerge;

	for(int i = westLimit;  i <= eastLimit; i++)
	{
		bool found = false;
		DungeonTile* dtN = r->getNorthernTileAtColumn(i);
		if(dtN != NULL)
		{

			int checkingNorthIndex = dtN->getY() - range;
			DungeonTile* checkingNorthernTile = mGrid->getTile(i, checkingNorthIndex);
			if(checkingNorthernTile)
			{
				if(checkingNorthernTile->getType() != DungeonTile::WALL  && checkingNorthernTile->getRoomId() != dtN->getRoomId())
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
						DungeonTile* wallTile = mGrid->getTile(i,checkingNorthernTile->getY()+1);
						//if(wallTile->getType() == DungeonTile::WALL || (wallTile->getType() == DungeonTile::CLEAR && wallTile->getRoomId() != checkingNorthernTile->getRoomId()))
						{
							r->addTile(wallTile);
							wallTile->setType(DungeonTile::CLEAR);
							roomsToMerge.push_back(checkingNorthernTile->getRoomId());

							expandedThisRoom = true;
						}
					}
				}
			}
		}

		found = false;
		DungeonTile* dtS = r->getSouthernTileAtColumn(i);
		if(dtS != NULL)
		{
			int checkingSIndex = dtS->getY() + range;
			DungeonTile* checkingSTile = mGrid->getTile(i, checkingSIndex);
			if(checkingSTile)
			{
				if(checkingSTile->getType() != DungeonTile::WALL  && checkingSTile->getRoomId() != dtS->getRoomId())
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
						DungeonTile* wallTile = mGrid->getTile(i,checkingSTile->getY()-1);
						//if(wallTile->getType() == DungeonTile::WALL || (wallTile->getType() == DungeonTile::CLEAR && wallTile->getRoomId() != checkingSTile->getRoomId()))
						{

						r->addTile(wallTile);
						wallTile->setType(DungeonTile::CLEAR);
						roomsToMerge.push_back(checkingSTile->getRoomId());
						expandedThisRoom = true;
						}
					}
				}
			}
		}
	}



	for(int i = northLimit;  i <= southLimit; i++)
	{

		bool found = false;
		DungeonTile* dtW = r->getWesternTileAtLine(i);
		if(dtW != NULL)
		{
			int checkingWIndex = dtW->getX() - range;
			DungeonTile* checkingWTile = mGrid->getTile(checkingWIndex,i );
			if(checkingWTile)
			{
				if(checkingWTile->getType() != DungeonTile::WALL && checkingWTile->getRoomId() != dtW->getRoomId())
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
						DungeonTile* wallTile = mGrid->getTile(checkingWTile->getY()+1,i);
						//if(wallTile->getType() == DungeonTile::WALL || (wallTile->getType() == DungeonTile::CLEAR && wallTile->getRoomId() != checkingWTile->getRoomId()))
						{

						r->addTile(wallTile); // connection between the two diff rooms
						wallTile->setType(DungeonTile::CLEAR); // clear the connection
						roomsToMerge.push_back(checkingWTile->getRoomId());
						expandedThisRoom = true;
						}
					}
				}
			}
		}

		found = false;
		DungeonTile* dtE = r->getEasternTileAtLine(i);
		if(dtE != NULL)
		{
			int checkingEIndex = dtE->getX() + range;
			DungeonTile* checkingETile = mGrid->getTile(i, checkingEIndex);
			if(checkingETile)
			{
				if(checkingETile->getType() != DungeonTile::WALL  && checkingETile->getRoomId() != dtE->getRoomId())
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
						DungeonTile* wallTile = mGrid->getTile(checkingETile->getY()-1,i);
						//if(wallTile->getType() == DungeonTile::WALL || (wallTile->getType() == DungeonTile::CLEAR && wallTile->getRoomId() != checkingETile->getRoomId()))
						{
						r->addTile(wallTile);
						wallTile->setType(DungeonTile::CLEAR);
						roomsToMerge.push_back(checkingETile->getRoomId());
						expandedThisRoom = true;
						}
					}
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

	return expandedThisRoom;

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