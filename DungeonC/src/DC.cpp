//
//  DC.cpp
//  DungeonCreator
//
//  Created by Nuno Moreira on 4/22/14.
//  Copyright (c) 2014 NunoMoreira. All rights reserved.
//

#include "DC.h"


DC::DC(int width, int height, int iterations, string path)
{
    Grid mGrid = Grid(width,height,path);
	mGrid.print();
	
	//translate(&mGrid,"Hello! Is it me you're looking for? I can see it in your eyes I can see it in your smile");
	//translate(&mGrid,"Never gonna give you up never gonna let you down never gonna run around and hurt you");
	translate(&mGrid,"Heróis do mar nobre povo nação valente imortal, levantai hoje de novo o esplendor de Portugal");
	
	mGrid.print();
	for(int i = 0; i < iterations; i++)
	{
		mGrid = gameOfLife(&mGrid);
		mGrid.print();
	}

    surroundByWall(&mGrid);
    mGrid.print();
    
	RoomManager* rm = new RoomManager(&mGrid);
	rm->findRooms();
}


void DC::translate(Grid *g,  char* byteArray)
{
	int index = 0;
	int byteArrayIndex = 0;
	char currentByte = byteArray[0];
    int offset = 0;
    while(true)
    {
        if(currentByte == '\0')
        {
            byteArrayIndex = 0;
            currentByte = byteArray[0];
            offset = index;
        }
		int mask = 1;
		for(int i = 0; i < 8; i++)
		{
			index = 8*byteArrayIndex + i + offset;
			int gridX = (int)(index % g->getWidth());
			int gridY = (int)(index / g->getWidth());
			
			if(index >= g->getWidth()*g->getHeight())
				return;
			if(mask & currentByte)
			{
				g->setTileType(gridX, gridY, DungeonTile::WALL);
			}
			mask = mask << 1;
		}
		currentByte = byteArray[++byteArrayIndex];
	}
}


Grid DC::gameOfLife(Grid* g)
{
	//cout << "Game of Life \n";
	Grid newGrid = Grid(*g);
    
	for(int i = 0; i < newGrid.getWidth(); i++)
	{
		for(int j = 0; j < newGrid.getHeight(); j++)
		{
			int livingNeighbours = 0;
			for(int k = -1; k < 2; k++)
			{
				for(int m = -1; m < 2; m++)
				{
					if(i + k < 0 || i + k >= newGrid.getWidth() ||
                       j + m < 0 || j + m >= newGrid.getHeight())
					{
						continue;
					}
                    
					if(g->getTileType(i+k, j+m) == DungeonTile::WALL)
						livingNeighbours++;
				}
			}
            
            if(newGrid.getTileType(i,j) == DungeonTile::WALL)
            {
            
                if(livingNeighbours < 4 || livingNeighbours > 8)
                    newGrid.setTileType(i, j,DungeonTile::CLEAR);
                else
                    newGrid.setTileType(i, j, DungeonTile::WALL);
            }
            else
            {
                if(livingNeighbours >= 5)
                    newGrid.setTileType(i,j, DungeonTile::WALL);
                else
                    newGrid.setTileType(i,j, DungeonTile::CLEAR);
            }
		}
	}
	return newGrid;
}

void DC::surroundByWall(Grid* g)
{
    int width = g->getWidth();
    int height = g->getHeight();
    
    for(int i = 0; i < width; i ++)
    {
        g->setTileType(i, 0, DungeonTile::WALL);
        g->setTileType(i, height-1, DungeonTile::WALL);
    }
    for(int i = 0; i < height; i++)
    {
        g->setTileType(0, i, DungeonTile::WALL);
        g->setTileType(width-1, i, DungeonTile::WALL);
    }
}

