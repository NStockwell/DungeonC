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
	return NULL;
}

DungeonTile*  Room::getWesternTileAtLine(int line)
{
	
	return NULL;
}

DungeonTile*  Room::getSoutherTileAtColumn(int column)
{
	
	return NULL;
}

DungeonTile* Room:: getNorthernTileAtColumn(int column)
{
	
	return NULL;
}
