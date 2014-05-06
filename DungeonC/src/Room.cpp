//
//  Room.cpp
//  DungeonCreator
//
//  Created by Nuno Moreira on 4/27/14.
//  Copyright (c) 2014 NunoMoreira. All rights reserved.
//

#include "Room.h"
using namespace std;

Room::Room(int id)
{
	mId = id;
	mTiles = vector<DungeonTile*>();
}

int Room::getId()
{
	return mId;
};

bool  Room::addTile(DungeonTile* dt)
{
	for(vector<DungeonTile*>::iterator it = mTiles.begin(); it != mTiles.end(); it++)
	{
		if(*it == dt)
			return false;
	}
	mTiles.push_back(dt);
	return true;
}
void  Room::removeTile(DungeonTile* dt)
{

}

bool Room::isEmpty()
{
	return mTiles.empty();
}

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
    int westest = 0;
    for(vector<DungeonTile*>::iterator it = mTiles.begin(); it != mTiles.end(); it++)
	{
        if((*it)->getY() == line)
        {
            int xPos =(*it)->getX();
            if(xPos > westest)
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
    int northern = 0;
    for(vector<DungeonTile*>::iterator it = mTiles.begin(); it != mTiles.end(); it++)
	{
        if((*it)->getX() == column)
        {
            int yPos =(*it)->getY();
            if(yPos > northern)
            {
                northern = yPos;
                returnTile = *it;
            }
        }
    }
    
	return returnTile;
}
