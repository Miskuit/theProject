#include <stdio.h>
#include <stdlib.h>
#include "Game.h"

#define HEXAGON_SIDES 6

#define MAX_PLAYERS 3
#define MAX_TILES 19
#define MAX_VERTICES 53

typedef struct _player player;
typedef struct _tile tile;

typedef struct _vertice {
   player *campus;
   tile *tile;
   int campusType;
} vertice;

typedef struct _edge {
   int arcGrant;
   tile *tile;
} edge;

struct _tile {
   int x, y, z;
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

Game newGame (int disciplines[], int dice[]) {

}

void disposeGame (Game g) {

}

void makeAction (Game g, action a) {

}

void throwDice (Game g, int diceScore) {

}

int getDiscipline (Game g, int regionID) {

}

int getDiceValue (Game g, int regionID) {

}

int getMostARCs (Game g) {

}

int getMostPublications (Game g) {

}

int getTurnNumber (Game g) {

}

int getWhoseTurn (Game g) {

}

int getCampus(Game g, path pathToVertex) {

}


int getARC(Game g, path pathToEdge) {

}

int isLegalAction (Game g, action a) {

}

int getKPIpoints (Game g, int player) {

}

int getARCs (Game g, int player) {

}

int getGO8s (Game g, int player) {

}

int getCampuses (Game g, int player) {

}

int getIPs (Game g, int player) {

}

int getPublications (Game g, int player) {

}

int getStudents (Game g, int player, int discipline) {

}

int getExchangeRate (Game g, int player,
                     int disciplineFrom, int disciplineTo) {

}

