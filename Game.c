#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "Game.h"

#define HEXAGON_SIDES 6

#define MAX_PLAYERS 3
#define MAX_TILES 19
#define MAX_TILES_PER_EDGE 2
#define MAX_TILES_PER_VERTICE 3
#define MAX_VERTICES 53

//Max and min values for tile co-ordinates
#define X_MIN -2
#define X_MAX 2
#define Y_MIN -2
#define Y_MAX 2

typedef struct _player player;
typedef struct _tile tile;

typedef struct _vertice {
   player *campus;
   tile *tiles[MAX_TILES_PER_VERTICE];
   int campusType;
} vertice;

typedef struct _edge {
   int arcGrant;
   tile *tiles[MAX_TILES_PER_EDGE];
} edge;

struct _tile {
   int x, y;
   int discipline;
   int score;
   edge *edges[HEXAGON_SIDES];
   vertice *vertices[HEXAGON_SIDES];
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
   tile board[MAX_TILES];
   gameStats stats;
} game;

void buildBoard(Game g, int disciplines[], int dice[]);
int getConnectedTilesToEdge(edge *given);
tile *getTile(Game g, int x, int y);
tile *getTileDirection(Game g, tile *given, int direction);
void initialiseBoard(Game g);

//////////////////////
//New Game Functions//
//////////////////////

//Converts our given disciplines and dice numbers into our board
//format. (i.e. our x, y co-ordinate system)
void buildBoard(Game g, int disciplines[], int dice[]) {
   int x = -2;
   int y = 0;
   int i = 0;
   while (i < MAX_TILES) {
      if (i == 0) { }
       else if (i == 3) {
         x = -1;
         y = -1;
      } else if (i == 7) {
         x = 0;
         y = -2;
      } else if (i == 12) {
         x = 1;
         y = -2;
      } else if (i == 16) {
         x = 2;
         y = -2;
      } else {
         y++;
      }
      g->board[i].x = x;
      g->board[i].y = y;
      g->board[i].discipline = disciplines[i];
      g->board[i].score = dice[i];
      i++;
   }
}

int getConnectedTilesToEdge(edge *given) {
   //WORK IN PROGRESS
   return 0;
}

tile *getTile(Game g, int x, int y) {
   int i = 0;
   tile *result = NULL;
   while (i < MAX_TILES) {
      if (g->board[i].x == x && g->board[i].y == y) {
         result = &g->board[i];
      }
      i++;
   }
   return result;
}

//Returns an adjacent tile, given a direction (where 0 means it
//shares its upper side, going around clockwise to 5
tile *getTileDirection(Game g, tile *given, int direction) {
   int x = (*given).x;
   int y = (*given).y;
   if (direction == 0) {
      y--;
   } else if (direction == 1) {
      x++;
      y--;
   } else if (direction == 2) {
      x++;
   } else if (direction == 3) {
      y++;
   } else if (direction == 4) {
      x--;
      y++;
   } else {
      x--;
   }
   return getTile(g, x, y);
}

void initialiseBoard(Game g) {
   int i = 0;
   while (i < MAX_TILES) {
      int i2 = 0;
      //tile *direction;
      while (i2 < HEXAGON_SIDES) {
         //direction = getTileDirection(g, &g->board[i], i2);
         //int cTiles = getConnectedTilesToEdge(g->board[i].edges[i2]);
         //WORK IN PROGRESS
         i2++;
      }
      i++;
   }
}

Game newGame (int disciplines[], int dice[]) {
   Game created = malloc(sizeof(game));
   assert(created != NULL);
   buildBoard(created, disciplines, dice);
   initialiseBoard(created);
   return created;
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

