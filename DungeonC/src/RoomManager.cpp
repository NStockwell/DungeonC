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
    mMergedRooms = vector<MergedRooms>();
	
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


bool RoomManager::expandRooms(int range)
{
    
    mMergedRooms.clear();
	bool expandedAtLeastOneRoom = false;
    for(int i = 0; i < mRooms.size(); i++)
    {
        Room* r = mRooms.at(i);
        bool expandedThisRoom = expandRoom(r, range);
        if(expandedThisRoom)
        {
            mGrid->print();
        }
		expandedAtLeastOneRoom = expandedThisRoom | expandedAtLeastOneRoom;
	}
    
	if(expandedAtLeastOneRoom)
	{
        for(int i = 0; i < mRooms.size(); i++) // merge rooms by finding 2 rooms with same room id but different pointers
        {
            Room* r = mRooms.at(i);
            for(int i = 0; i < mRooms.size(); i++)
            {
                Room* r2 = mRooms.at(i);
				if(r2->getId() == r->getId() && r != r2 && r2->getId() != -1 && r->getId()!= -1)
				{
                    //cout << "\nJoining Rooms " << r->getId() << " with room " << r2->getId();
					vector<DungeonTile*> addTilesToOriginalRoom = r2->getTiles();
					for(int i = 0; i < addTilesToOriginalRoom.size(); i++)
					{
						r->addTile(addTilesToOriginalRoom.at(i));
					}
					r2->clearTiles();
				}
			}
		}
        
        // create a new list removing the empty rooms
		vector<Room*> newRooms = vector<Room*>();
        for(int i = 0; i < mRooms.size(); i++)
        {
            Room* r = mRooms.at(i);
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
            vector<DungeonTile*> vecDT;
            
			int checkingNorthIndex = dtN->getY() - range;
            vecDT.push_back(mGrid->getTile(i, checkingNorthIndex));
            vecDT.push_back(mGrid->getTile(i+1, checkingNorthIndex));
            vecDT.push_back(mGrid->getTile(i-1, checkingNorthIndex));
            expandedThisRoom |= checkThisTile(dtN, vecDT, &roomsToMerge, r);
            /*
			int checkingNorthIndex = dtN->getY() - range;
			DungeonTile* checkingNorthernTile = mGrid->getTile(i, checkingNorthIndex);
			if(checkingNorthernTile)
			{
				if(checkingNorthernTile->getType() != DungeonTile::WALL  && checkingNorthernTile->getRoomId() != dtN->getRoomId())
				{
                    for(int i = 0; i < mMergedRooms.size(); i++) // check the merged rooms so we don't merge room 1 to room 2 after we have added room 2 into room 1
                    {
                        MergedRooms mr = mMergedRooms.at(i);
                        if((mr.roomA == r->getId() && mr.roomB == checkingNorthernTile->getRoomId()) || 
							(mr.roomB == r->getId()  && checkingNorthernTile->getRoomId() == mr.roomA))
                        {
                            found = true;
                            break;
                        }
                    }
                    if(!found)
                    {
                        for(int i = 0; i < roomsToMerge.size(); i++)
                        {
                            int k = roomsToMerge.at(i);
                            if(k == checkingNorthernTile->getRoomId())
                            {
                                found = true;
                                break;
                            }
                        }
                    }
					if(!found)
					{
						DungeonTile* wallTile = mGrid->getTile(i,checkingNorthernTile->getY()+1);
						if(wallTile->getType() != DungeonTile::WALL)
						{
							cout << "\nN_UPS " << wallTile->getRoomId() << " x:" << wallTile->getX() << " y:" << wallTile->getY();
                            cout << "\nchecking "<< checkingNorthernTile->getRoomId() << " x:" << checkingNorthernTile->getX() << " y:" << checkingNorthernTile->getY();
                            cout << "\n";
						}
						//if(wallTile->getType() == DungeonTile::WALL || (wallTile->getType() == DungeonTile::CLEAR && wallTile->getRoomId() != checkingNorthernTile->getRoomId()))
						{
                            cout << "\n MERGING N ROOMS:" << r->getId() << " and room:" << checkingNorthernTile->getRoomId();
                            cout.flush();
							roomsToMerge.push_back(checkingNorthernTile->getRoomId());
							r->addTile(wallTile);
							wallTile->setType(DungeonTile::CLEAR);
                            
							expandedThisRoom = true;
                            
						}
					}
				}
			}*/
		}
        
		found = false;
		DungeonTile* dtS = r->getSouthernTileAtColumn(i);
		if(dtS != NULL)
		{
            
            vector<DungeonTile*> vecDT;
            
			int checkingSIndex = dtS->getY() + range;
            vecDT.push_back(mGrid->getTile(i, checkingSIndex));
            vecDT.push_back(mGrid->getTile(i+1, checkingSIndex));
            vecDT.push_back(mGrid->getTile(i-1, checkingSIndex));
            expandedThisRoom |= checkThisTile(dtS, vecDT, &roomsToMerge, r);
            /*
			int checkingSIndex = dtS->getY() + range;
			DungeonTile* checkingSTile = mGrid->getTile(i, checkingSIndex);
			if(checkingSTile)
			{
				if(checkingSTile->getType() != DungeonTile::WALL  && checkingSTile->getRoomId() != dtS->getRoomId())
				{
                    for(int i = 0; i < mMergedRooms.size(); i++) // check the merged rooms so we don't merge room 1 to room 2 after we have added room 2 into room 1
                    {
                        MergedRooms mr = mMergedRooms.at(i);
                        if((mr.roomA == r->getId() && mr.roomB == checkingSTile->getRoomId()) || (mr.roomB == r->getId()
                                                                                                  && checkingSTile->getRoomId() == mr.roomA))
                        {
                            found = true;
                            break;
                        }
                    }
                    if(!found)
                    {
                        for(int i = 0; i < roomsToMerge.size(); i++)
                        {
                            int k = roomsToMerge.at(i);
                            if(k == checkingSTile->getRoomId())
                            {
                                found = true;
                                break;
                            }
                        }
                    }
                    
					if(!found)
					{
						DungeonTile* wallTile = mGrid->getTile(i,checkingSTile->getY()-1);
						if(wallTile->getType() != DungeonTile::WALL)
						{
							cout << "\nS_UPS " << wallTile->getRoomId() << " x:" << wallTile->getX() << " y:" << wallTile->getY();
                            cout << "\nchecking "<< checkingSTile->getRoomId() << " x:" << checkingSTile->getX() << " y:" << checkingSTile->getY();;
                            cout << "\n";
						}
						//if(wallTile->getType() == DungeonTile::WALL || (wallTile->getType() == DungeonTile::CLEAR && wallTile->getRoomId() != checkingSTile->getRoomId()))
						{
                            
                            cout << "\n MERGING S ROOMS:" << r->getId() << " and room:" << checkingSTile->getRoomId();
                            cout.flush();
                            roomsToMerge.push_back(checkingSTile->getRoomId());
                            r->addTile(wallTile);
                            wallTile->setType(DungeonTile::CLEAR);
                            expandedThisRoom = true;
                            
						}
					}
				}
			}*/
		}
	}
    
    
    
	for(int i = northLimit;  i <= southLimit; i++)
	{
        
		bool found = false;
		DungeonTile* dtW = r->getWesternTileAtLine(i);
		if(dtW != NULL)
		{
            
            vector<DungeonTile*> vecDT;

			int checkingWIndex = dtW->getX() - range;
            vecDT.push_back(mGrid->getTile(checkingWIndex,i));
            vecDT.push_back(mGrid->getTile(checkingWIndex,i+1));
            vecDT.push_back(mGrid->getTile(checkingWIndex,i-1));
            expandedThisRoom |= checkThisTile(dtW, vecDT, &roomsToMerge, r);
			/*
             if(checkingWTile)
			{
				if(checkingWTile->getType() != DungeonTile::WALL && checkingWTile->getRoomId() != dtW->getRoomId())
				{
                    for(int i = 0; i < mMergedRooms.size(); i++) // check the merged rooms so we don't merge room 1 to room 2 after we have added room 2 into room 1
                    {
                        MergedRooms mr = mMergedRooms.at(i);
                        if((mr.roomA == r->getId() && mr.roomB == checkingWTile->getRoomId()) || (mr.roomB == r->getId()
                                                                                                  && checkingWTile->getRoomId() == mr.roomA))
                        {
                            found = true;
                            break;
                        }
                    }
                    if(!found)
                    {
                        for(int i = 0; i < roomsToMerge.size(); i++)
                        {
                            int k = roomsToMerge.at(i);
                            if(k == checkingWTile->getRoomId())
                            {
                                found = true;
                                break;
                            }
                        }
                    }
                    
					if(!found)
					{
						DungeonTile* wallTile = mGrid->getTile(checkingWTile->getX()+1,i);
						if(wallTile->getType() != DungeonTile::WALL)
						{
							cout << "\nW_UPS " << wallTile->getRoomId() << " x:" << wallTile->getX() << " y:" << wallTile->getY();
                            cout << "\nchecking "<< checkingWTile->getRoomId() << " x:" << checkingWTile->getX() << " y:" << checkingWTile->getY();
                            cout << "\n";
						}
						//if(wallTile->getType() == DungeonTile::WALL || (wallTile->getType() == DungeonTile::CLEAR && wallTile->getRoomId() != checkingWTile->getRoomId()))
						{
                            
                            cout << "\n MERGING W ROOMS:" << r->getId() << " and room:" << checkingWTile->getRoomId();
                            cout.flush();
                            roomsToMerge.push_back(checkingWTile->getRoomId());
                            r->addTile(wallTile); // connection between the two diff rooms
                            wallTile->setType(DungeonTile::CLEAR); // clear the connection
                            expandedThisRoom = true;
                            
						}
					}
				}
			}*/
		}
        
		found = false;
		DungeonTile* dtE = r->getEasternTileAtLine(i);
		if(dtE != NULL)
		{
            vector<DungeonTile*> vecDT;
            
			int checkingEIndex = dtE->getX() + range;
            vecDT.push_back(mGrid->getTile(checkingEIndex,i));
            vecDT.push_back(mGrid->getTile(checkingEIndex,i+1));
            vecDT.push_back(mGrid->getTile(checkingEIndex,i-1));
            
            expandedThisRoom |= checkThisTile(dtE, vecDT, &roomsToMerge, r);
            /*
			int checkingEIndex = dtE->getX() + range;
			DungeonTile* checkingETile = mGrid->getTile(checkingEIndex,i);
			if(checkingETile)
			{
				if(checkingETile->getType() != DungeonTile::WALL  && checkingETile->getRoomId() != dtE->getRoomId())
				{
                    for(int i = 0; i < mMergedRooms.size(); i++) // check the merged rooms so we don't merge room 1 to room 2 after we have added room 2 into room 1
                    {
                        MergedRooms mr = mMergedRooms.at(i);
                        if((mr.roomA == r->getId() && mr.roomB == checkingETile->getRoomId()) || (mr.roomB == r->getId()
                                                                                                  && checkingETile->getRoomId() == mr.roomA))
                        {
                            found = true;
                            break;
                        }
                    }
                    
                    if(!found)
                    {
                        for(int i = 0; i < roomsToMerge.size(); i++)
                        {
                            int k = roomsToMerge.at(i);
                            if(k == checkingETile->getRoomId())
                            {
                                found = true;
                                break;
                            }
                        }
                    }
                    
					if(!found)
					{
						DungeonTile* wallTile = mGrid->getTile(checkingETile->getX()-1,i);
						if(wallTile->getType() != DungeonTile::WALL)
						{
							cout << "\nE_UPS " << wallTile->getRoomId() << " x:" << wallTile->getX() << " y:" << wallTile->getY();
                            cout << "\nchecking "<< checkingETile->getRoomId() << " x:" << checkingETile->getX() << " y:" << checkingETile->getY();
                            cout << "\n";
						}
						//if(wallTile->getType() == DungeonTile::WALL || (wallTile->getType() == DungeonTile::CLEAR && wallTile->getRoomId() != checkingETile->getRoomId()))
						{
                            cout << "\n MERGING E ROOMS:" << r->getId() << " and room:" << checkingETile->getRoomId();
                            cout.flush();
                            roomsToMerge.push_back(checkingETile->getRoomId());
                            r->addTile(wallTile);
                            wallTile->setType(DungeonTile::CLEAR);
                            expandedThisRoom = true;
                            
						}
					}
				}
			}
            */
		}
	}
    
    for(int i = 0; i < roomsToMerge.size(); i++)
    {
        int roomId = roomsToMerge.at(i);
        
        MergedRooms mr;
        mr.roomA = r->getId();
        mr.roomB = roomId;
        
        mMergedRooms.push_back(mr);
		for(int j = 0; j < mRooms.size(); j++)
        {
            Room* room = mRooms.at(j);
           
			if(room->getId() == r->getId())
				continue;
			
			if(room->getId() == roomId)
			{
                //cout<< "Setting room " << room->getId() << " to new id " << r->getId();
				room->setId(r->getId());
			}
		}
	}
    
	return expandedThisRoom;
    
}


bool RoomManager::checkThisTile(DungeonTile* dT, vector<DungeonTile*>tilesToCheck, vector<int>*roomsToMerge, Room* r)
{
    bool found = false;
    bool expandedThisRoom = false;

    if(dT != NULL)
    {
        for(int i= 0; i < tilesToCheck.size(); i++)
        {
            DungeonTile* checkingTile = tilesToCheck.at(i);
            if(checkingTile)
            {
                if(checkingTile->getType() != DungeonTile::WALL  && checkingTile->getRoomId() != dT->getRoomId())
                {
                    for(int i = 0; i < mMergedRooms.size(); i++) // check the merged rooms so we don't merge room 1 to room 2 after we have added room 2 into room 1
                    {
                        MergedRooms mr = mMergedRooms.at(i);
                        if((mr.roomA == r->getId() && mr.roomB == checkingTile->getRoomId()) ||
                           (mr.roomB == r->getId() && checkingTile->getRoomId() == mr.roomA))
                        {
                            found = true;
                            break;
                        }
                    }
                    if(!found)
                    {
                        for(int i = 0; i < roomsToMerge->size(); i++)
                        {
                            int k = roomsToMerge->at(i);
                            if(k == checkingTile->getRoomId())
                            {
                                found = true;
                                break;
                            }
                        }
                    }
                    
                    if(!found) // connect the rooms
					
					{
                        
                        /*
                        DungeonTile* wallTile = mGrid->getTile(i,checkingTile->getY()-1);
                        if(wallTile->getType() != DungeonTile::WALL)
                        {
                            cout << "\nS_UPS " << wallTile->getRoomId() << " x:" << wallTile->getX() << " y:" << wallTile->getY();
                            cout << "\nchecking "<< checkingTile->getRoomId() << " x:" << checkingTile->getX() << " y:" << checkingTile->getY();;
                            cout << "\n";
						}
                         */
                        
                        //cout << "\n MERGING S ROOMS:" << r->getId() << " and room:" << checkingTile->getRoomId();
                        //cout.flush();
                        
                        roomsToMerge->push_back(checkingTile->getRoomId());
                        connectTheseTiles(dT, checkingTile, r);
                        
                        /*
                        r->addTile(wallTile);
                        wallTile->setType(DungeonTile::CLEAR);
                        */
                        expandedThisRoom = true;
                        
                    }
                }
            }
        }
    }
    return expandedThisRoom;
}


void RoomManager::connectTheseTiles(DungeonTile* startingTile, DungeonTile* endTile, Room* r)
{
	int startingRoomId = startingTile->getRoomId();
    
	int x1 = startingTile->getX();
	int y1 = startingTile->getY();
    
	int x2 = endTile->getX();
	int y2 = endTile->getY();
    
	int diffX = x2 - x1;
	int diffY = y2 - y1;
    
    int yOffset = 0;
    int xOffset = 0;
    while( (abs(yOffset) != abs(diffY)) || (abs(xOffset) != abs(diffX)))
    {
        while (abs(xOffset) != abs(diffX))
        {
            xOffset++;
            DungeonTile* newTile = mGrid->getTile((diffX < 0 ? x1 - xOffset : x1 + xOffset),
                                                  y1);
            //newTile->setRoomId(startingRoomId);
            r->addTile(newTile);
            newTile->setType(DungeonTile::CLEAR);
            
        }
        if(abs(yOffset) != abs(diffY) )
        {
            yOffset++;
            DungeonTile* newTile = mGrid->getTile(x2,
                                                  (diffY < 0 ? y1 - yOffset : y1 + yOffset));
            //newTile->setRoomId(startingRoomId);
            r->addTile(newTile);
            newTile->setType(DungeonTile::CLEAR);
		}
        
    }
    
    
}
