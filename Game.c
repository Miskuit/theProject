#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "Game.h"

#define HEXAGON_SIDES 6

#define MAX_PLAYERS 3
#define MAX_TILES 19
#define MAX_VERTICES 54
#define MAX_EDGES 72

//Max and min values for tile co-ordinates
#define X_MIN -2
#define X_MAX 2
#define Y_MIN -2
#define Y_MAX 2

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
int buildHexagonVertices(Game g, tile *tile, int startIndex);
pos getNewPosition(pos position, int direction, int isNegative);
vertice *getVertice(Game g, pos position);
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
   int rValue = 0;
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
      rValue = buildHexagonVertices(g, &g->tiles[i], verticeIndex);
      verticeIndex = rValue;
      i++;
   }
   printf("Built %d hexagons and %d vertices.\n", i, verticeIndex);

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
   //printf("Built hexagon at (%lf, %lf).\n", tile->position.x, tile->position.y);
   return startIndex;
}

Game newGame (int disciplines[], int dice[]) {
   Game created = malloc(sizeof(game));
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

