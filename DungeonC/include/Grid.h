#ifndef GRID_H
#define GRID_H

#include <vector>
#include "DungeonTile.h"
#include <iostream>

using namespace std;



class Grid
{
public:
	Grid(int width, int height, string path);
	~Grid();
	void print();
	int getWidth();
	int getHeight();
	void setTileType(int x, int y, DungeonTile::TileType newType);
	void setTileTypeMod(int x, int y, DungeonTile::TileType newType);
	DungeonTile::TileType getTileType(int x, int y);
	DungeonTile* getTile(int x, int y);

private:
	
	FILE *mFile;
	int mWidth;
	int mHeight;
    string mPath;
	vector<vector<DungeonTile*> > mTiles;
};

#endif