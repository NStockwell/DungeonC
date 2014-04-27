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

void  Room::addTile(DungeonTile* dt)
{
	mTiles.push_back(dt);
}
void  Room::removeTile(DungeonTile* dt)
{

}

DungeonTile*  Room::getEasternTileAtLine(int line)
{

}

DungeonTile*  Room::getWesternTileAtLine(int line)
{

}

DungeonTile*  Room::getSoutherTileAtColumn(int column)
{

}

DungeonTile* Room:: getNorthernTileAtColumn(int column)
{

}
