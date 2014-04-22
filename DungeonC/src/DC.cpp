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
	
	translate(&mGrid,"Hello! Is it me you're looking for? I can see it in your eyes I can see it in your smile");
	
	mGrid.print();
	for(int i = 0; i < iterations; i++)
	{
		mGrid = gameOfLife(&mGrid);
		mGrid.print();
	}
}


void DC::translate(Grid *g,  char* byteArray)
{
	int index = 0;
	int byteArrayIndex = 0;
	char currentByte = byteArray[0];
    
	while(currentByte != '\0')
	{
		int mask = 1;
		for(int i = 0; i < 8; i++)
		{
			index = 8*byteArrayIndex + i;
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
			if(livingNeighbours < 3 || livingNeighbours > 5)
				newGrid.setTileType(i, j,DungeonTile::CLEAR);
			else
				newGrid.setTileType(i, j, DungeonTile::WALL);
		}
	}
	return newGrid;
}


