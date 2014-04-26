// DungeonCreator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>

#include "Grid.h"

using namespace std;

void print();

void translate(Grid *g,  char* byteArray);
Grid gameOfLife(Grid* g);


int _tmain(int argc, char* argv[])
{
	Grid mGrid = Grid(50,50);
	mGrid.print();
	
	translate(&mGrid,"Hello! Is it me you're looking for? I can see it in your eyes I can see it in your smile");
	
	mGrid.print();
	for(int i = 0; i < 8; i++)
	{
		mGrid = gameOfLife(&mGrid);
		mGrid.print();
	}
	//while(1)
	{
		Sleep(5000);
	//	print();
	}
	return 0;
}


void print()
{
	cout << "Hello World";
}


void translate(Grid *g,  char* byteArray)
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
				g->setTileType(gridX, gridY, TileType::WALL);
			}
			mask = mask << 1;
		}
		currentByte = byteArray[++byteArrayIndex];
	}
}


Grid gameOfLife(Grid* g)
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

					if(g->getTileType(i+k, j+m) == TileType::WALL)
						livingNeighbours++;
				}
			}
			if(livingNeighbours < 3 || livingNeighbours > 5)
				newGrid.setTileType(i, j,TileType::CLEAR);
			else
				newGrid.setTileType(i, j, TileType::WALL);
		}
	}
	return newGrid;
}