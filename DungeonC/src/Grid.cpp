#include "Grid.h"


Grid::Grid(int width, int height, string path)
{
	
	mWidth = width;
	mHeight = height;
    mPath = path;
	for(int i = 0; i < height; i++)
	{
		vector<DungeonTile> vDT;
		for(int j = 0; j < width; j++)
		{
			DungeonTile dTile = DungeonTile(i,j,DungeonTile::TileType::CLEAR);
			vDT.push_back(dTile);
		}
		mTiles.push_back(vDT);
	}
}

Grid::~Grid()
{
}

void Grid::print()
{
	mFile = fopen(mPath.c_str(), "a");
	fprintf(mFile, "Printing current state of Grid:");
	fprintf(mFile,"\n   ");
	for(int i = 0; i < mWidth; i++)
	{
		char c[10];
        sprintf(c, "%d",i);
//		itoa(i,c,10);
		fprintf(mFile,c);
		if(i < 10)
			fprintf(mFile, " ");
	}
	fprintf(mFile,"\n");
	for(int i = 0; i < mHeight; i++)
	{
		char c[10];
		sprintf(c, "%d",i);
        //		itoa(i,c,10);
		fprintf(mFile,c);
		if(i < 10)
			fprintf(mFile, " ");
		
			fprintf(mFile," ");
		vector<DungeonTile> vDT = mTiles.at(i);
		for(int j = 0; j < mWidth; j++)
		{
			DungeonTile dTile = vDT.at(j);
			dTile.print(mFile);
			fprintf(mFile," ");
		}
		fprintf(mFile,"\n");
	}
	fprintf(mFile,"\n\n\n");
	fflush(mFile);
	fclose(mFile);
}


int Grid::getWidth(){return mWidth;}
int Grid::getHeight(){return mHeight;}
void Grid::setTileType(int x, int y, DungeonTile::TileType newType)
	{
		mTiles.at(y).at(x).setType(newType);
	}

	
DungeonTile::TileType Grid::getTileType(int x, int y)
	{
		return mTiles.at(y).at(x).getType();
	}