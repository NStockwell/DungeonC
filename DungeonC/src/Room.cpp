//
//  Room.cpp
//  DungeonCreator
//
//  Created by Nuno Moreira on 4/27/14.
//  Copyright (c) 2014 NunoMoreira. All rights reserved.
//

#include "Room.h"
#include <limits>
using namespace std;

Room::Room(int id)
{
	mId = id;
	mTiles = vector<DungeonTile*>();
	mNorthLimit = INT_MAX;
	mSouthLimit = INT_MIN;
	mEastLimit = INT_MIN;
	mWestLimit = INT_MAX;
}

int Room::getId()
{
	return mId;
}

void Room::setId(int roomId)
{
	if(roomId == mId)
		return;

	mId = roomId;

	for(vector<DungeonTile*>::iterator it = mTiles.begin(); it != mTiles.end(); it++)
	{ 
		(*it)->setRoomId(roomId);
	}
}

void Room::clearTiles()
{
	mTiles.clear();
	mId = mNorthLimit = mSouthLimit = mWestLimit = mEastLimit = -1;
}

vector<DungeonTile*> Room::getTiles()
{
	return mTiles;
}

bool  Room::addTile(DungeonTile* dt)
{
	for(vector<DungeonTile*>::iterator it = mTiles.begin(); it != mTiles.end(); it++)
	{
		if(*it == dt)
			return false;
	}
	mTiles.push_back(dt);

	if(dt->getX() > mEastLimit)
		mEastLimit = dt->getX();
	if(dt->getX() < mWestLimit)
		mWestLimit = dt->getX();

	if(dt->getY() > mSouthLimit)
		mSouthLimit = dt->getY();
	if(dt->getY() < mNorthLimit)
		mNorthLimit = dt->getY();
	
	dt->setRoomId(mId);

	return true;
}
void  Room::removeTile(DungeonTile* dt)
{

}

bool Room::isEmpty()
{
	return mTiles.empty();
}


int Room::getNorthLimit() { return mNorthLimit;}
int Room::getSouthLimit() {return mSouthLimit;}
int Room::getWestLimit() {return mWestLimit;}
int Room::getEastLimit() {return mEastLimit;}

DungeonTile*  Room::getEasternTileAtLine(int line)
{
    DungeonTile* returnTile = NULL;
    int eastest = 0;
    for(vector<DungeonTile*>::iterator it = mTiles.begin(); it != mTiles.end(); it++)
	{
        if((*it)->getY() == line)
        {
            int xPos =(*it)->getX();
            if(xPos > eastest)
            {
                eastest = xPos;
                returnTile = *it;
            }
        }
    }
    
	return returnTile;
}

DungeonTile*  Room::getWesternTileAtLine(int line)
{
    DungeonTile* returnTile = NULL;
    int westest = INT_MAX;
    for(vector<DungeonTile*>::iterator it = mTiles.begin(); it != mTiles.end(); it++)
	{
        if((*it)->getY() == line)
        {
            int xPos =(*it)->getX();
            if(xPos < westest)
            {
                westest = xPos;
                returnTile = *it;
            }
        }
    }
    
	return returnTile;
}

DungeonTile*  Room::getSouthernTileAtColumn(int column)
{
    DungeonTile* returnTile = NULL;
    int southest = 0;
    for(vector<DungeonTile*>::iterator it = mTiles.begin(); it != mTiles.end(); it++)
	{
        if((*it)->getX() == column)
        {
            int yPos =(*it)->getY();
            if(yPos > southest)
            {
                southest = yPos;
                returnTile = *it;
            }
        }
    }
    
	return returnTile;
}

DungeonTile* Room:: getNorthernTileAtColumn(int column)
{
    DungeonTile* returnTile = NULL;
    int northern = INT_MAX;
    for(vector<DungeonTile*>::iterator it = mTiles.begin(); it != mTiles.end(); it++)
	{
        if((*it)->getX() == column)
        {
            int yPos =(*it)->getY();
            if(yPos < northern)
            {
                northern = yPos;
                returnTile = *it;
            }
        }
    }
    
	return returnTile;
}
