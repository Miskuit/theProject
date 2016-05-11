#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"

#define HEXAGON_SIDES 6

#define MAX_PLAYERS 3
#define MAX_TILES 19
#define MAX_VERTICES 54
#define MAX_EDGES 72

#define FALSE 0
#define TRUE 1

//DIRECTIONS
#define ACROSS 0
#define ACROSS_X 2
#define ACROSS_Y 0

#define ANGLE_LEFT 1
#define ANGLE_LEFT_X -1
#define ANGLE_LEFT_Y 1

#define ANGLE_RIGHT 2
#define ANGLE_RIGHT_X 1
#define ANGLE_RIGHT_Y 1

typedef struct _player player;
typedef struct _tile tile;

typedef struct _pos {
   double x;
   double y;
} pos;

typedef struct _vertice {
   player *campus;
   pos position;
   int campusType;
} vertice;

typedef struct _edge {
   int arcGrant;
   pos position;
} edge;

struct _tile {
   pos position;
   int discipline;
   int score;
};

typedef struct _studentTypes {
   int bThinking;
   int bProblemSolving;
   int bHowThinksWorks;
   int bJobs;
   int bTVJobs;
   int bMoney;
} studentTypes;

typedef struct _playerStats {
   int points;
   int arcGrants;
   int publications;
   int IPs;
} playerStats;

struct _player {
   int playerID;
   vertice *campuses[MAX_VERTICES];
   studentTypes students;
   playerStats stats;
};

typedef struct _gameStats {
   player *maxPoints;
   player *maxArcGrants;
   player *maxPublications;
   int turnNumber;
   int numGO8s;
} gameStats;

typedef struct _game {
   player players[MAX_PLAYERS];
   tile tiles[MAX_TILES];
   edge edges[MAX_EDGES];
   vertice vertices[MAX_VERTICES];
   gameStats stats;
} game;

void buildBoard(Game g, int disciplines[], int dice[]);
int buildHexagonEdges(Game g, tile *tile, int startIndex);
int buildHexagonVertices(Game g, tile *tile, int startIndex);
edge *getEdge(Game g, pos p);
edge *getHexagonEdge(Game g, tile *tile, int index);
pos getHexagonEdgePosition(Game g, tile *tile, int index);
vertice *getHexagonVertice(Game g, tile *tile, int index);
pos getHexagonVerticePosition(Game g, tile *tile, int index);
pos getNewPosition(pos position, int direction, int isNegative);
vertice *getVertice(Game g, pos position);
int edgeExists(Game g, pos p);
pos midpoint(pos p1, pos p2);
int verticeExists(Game g, pos position);

//////////////////////
//New Game Functions//
//////////////////////

//Converts our given disciplines and dice numbers into our board
//format. (i.e. our x, y co-ordinate system)
void buildBoard(Game g, int disciplines[], int dice[]) {
   double x = -6;
   double y = 2;
   int i = 0;
   int verticeIndex = 0;
   int edgeIndex = 0;
   while (i < MAX_TILES) {
      if (i == 0) { }
       else if (i == 3) {
         x = -3;
         y = 3;
      } else if (i == 7) {
         x = 0;
         y = 4;
      } else if (i == 12) {
         x = 3;
         y = 3;
      } else if (i == 16) {
         x = 6;
         y = 2;
      } else {
         y = y - 2;
      }
      g->tiles[i].position.x = x;
      g->tiles[i].position.y = y;
      g->tiles[i].discipline = disciplines[i];
      g->tiles[i].score = dice[i];
      verticeIndex = buildHexagonVertices(g, &g->tiles[i], verticeIndex);
      edgeIndex = buildHexagonEdges(g, &g->tiles[i], edgeIndex);
      i++;
   }
   printf("Built %d hexagons, %d vertices and %d edges.\n", i, verticeIndex, edgeIndex);

}

int buildHexagonEdges(Game g, tile *tile, int startIndex) {
    int i = 0;
    pos edgePos;
    while (i < HEXAGON_SIDES) {
      edgePos = getHexagonEdgePosition(g, tile, i);
      if (!edgeExists(g, edgePos)) {
         g->edges[startIndex].position = edgePos;
         startIndex++;

      }
      i++;
    }
    return startIndex;
}

int buildHexagonVertices(Game g, tile *tile, int startIndex) {
   int i = 0;
   int j = 0;
   pos newPos;
   while (i < 3) {
      while (j < 2) {
         newPos = getNewPosition(tile->position, i, j);
         if (!verticeExists(g, newPos)) {
            g->vertices[startIndex].position = newPos;
            startIndex++;
         }
         j++;
      }
      j = 0;
      i++;
   }
   return startIndex;
}

Game newGame (int disciplines[], int dice[]) {
   Game created = malloc(sizeof(struct _game));
   assert(created != NULL);
   buildBoard(created, disciplines, dice);
   return created;
}

pos getNewPosition(pos position, int direction, int isNegative) {
   pos relPos = { 0 , 0 };
   if (direction == ACROSS) {
      relPos.x += (double)ACROSS_X;
      relPos.y += (double)ACROSS_Y;
   } else if (direction == ANGLE_LEFT) {
      relPos.x += (double)ANGLE_LEFT_X;
      relPos.y += (double)ANGLE_LEFT_Y;
   } else {
      relPos.x += (double)ANGLE_RIGHT_X;
      relPos.y += (double)ANGLE_RIGHT_Y;
   }
   if (isNegative) {
      relPos.x *= -1.0;
      relPos.y *= -1.0;
   }
   position.x += relPos.x;
   position.y += relPos.y;
   return position;
}

edge *getEdge(Game g, pos p) {
   int i = 0;
   edge *select = NULL;
   while (i < MAX_EDGES) {
      select = &g->edges[i];
      if (select != NULL && select->position.x == p.x && select->position.y == p.y) {
         i = MAX_EDGES;
      } else {
         select = NULL;
      }
      i++;
   }
   return select;
}

edge *getHexagonEdge(Game g, tile *tile, int index) {
   return getEdge(g, getHexagonEdgePosition(g, tile, index));
}

pos getHexagonEdgePosition(Game g, tile *tile, int index) {
   assert(index <= HEXAGON_SIDES);
   return midpoint(getHexagonVerticePosition(g, tile, index), getHexagonVerticePosition(g, tile, (index+1)));
}

vertice *getHexagonVertice(Game g, tile *tile, int index) {
   return getVertice(g, getHexagonVerticePosition(g, tile, index));
}

pos getHexagonVerticePosition(Game g, tile *tile, int index) {
   int dir, isNegative;
   if (index == 0 || index == 6) {
      dir = ANGLE_LEFT;
      isNegative = FALSE;
   } else if (index == 1) {
      dir = ANGLE_RIGHT;
      isNegative = FALSE;
   } else if (index == 2) {
      dir = ACROSS;
      isNegative = FALSE;
   } else if (index == 3) {
      dir = ANGLE_LEFT;
      isNegative = TRUE;
   } else if (index == 4) {
      dir = ANGLE_RIGHT;
      isNegative = TRUE;
   } else {
      dir = ACROSS;
      isNegative = TRUE;
   }
   return getNewPosition(tile->position,dir,isNegative);
}

vertice *getVertice(Game g, pos position) {
   int i = 0;
   vertice *select = NULL;
   while (i < MAX_VERTICES) {
      select = &g->vertices[i];
      if (select != NULL && select->position.x == position.x && select->position.y == position.y) {
         i = MAX_VERTICES;
      } else {
         select = NULL;
      }
      i++;
   }
   return select;
}

int edgeExists(Game g, pos p) {
   edge *test = getEdge(g, p);
   return (test != NULL);
}

pos midpoint(pos p1, pos p2) {
   pos mid;
   mid.x = (p1.x + p2.x)/2;
   mid.y = (p1.y + p2.y)/2;
   return mid;
}

int verticeExists(Game g, pos position) {
   vertice *test = getVertice(g, position);
   return (test != NULL);
}

/////////////////////
/////////////////////
/////////////////////

void disposeGame (Game g) {

}

void makeAction (Game g, action a) {

}

void throwDice (Game g, int diceScore) {

}

int getDiscipline (Game g, int regionID) {
   return 0;
}

int getDiceValue (Game g, int regionID) {
   return 0;
}

int getMostARCs (Game g) {
   return 0;
}

int getMostPublications (Game g) {
   return 0;
}

int getTurnNumber (Game g) {
   return 0;
}

int getWhoseTurn (Game g) {
   return 0;
}

int getCampus(Game g, path pathToVertex) {
   return 0;
}


int getARC(Game g, path pathToEdge) {
   return 0;
}

int isLegalAction (Game g, action a) {
   return 0;
}

int getKPIpoints (Game g, int player) {
   return 0;
}

int getARCs (Game g, int player) {
   return 0;
}

int getGO8s (Game g, int player) {
   return 0;
}

int getCampuses (Game g, int player) {
   return 0;
}

int getIPs (Game g, int player) {
   return 0;
}

int getPublications (Game g, int player) {
   return 0;
}

int getStudents (Game g, int player, int discipline) {
   return 0;
}

int getExchangeRate (Game g, int player,
                     int disciplineFrom, int disciplineTo) {
   return 0;
}

