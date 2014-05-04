//
//  DC.h
//  DungeonCreator
//
//  Created by Nuno Moreira on 4/22/14.
//  Copyright (c) 2014 NunoMoreira. All rights reserved.
//

#ifndef __DungeonCreator__DC__
#define __DungeonCreator__DC__

#include <iostream>
#include "Grid.h"
#include "DungeonTile.h"
#include "RoomManager.h"

using namespace std;

class DC
{
    
public:
    DC(int w, int h, int iterations, string path);
    
private:
    
    Grid gameOfLife(Grid* g);
    void translate(Grid *g,  char* byteArray);
    
};

#endif /* defined(__DungeonCreator__DC__) */
