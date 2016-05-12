#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"

#define HEXAGON_SIDES 6
#define STUDENT_TYPES 6

#define MAX_PLAYERS 3
#define MAX_TILES 19
#define MAX_VERTICES 54
#define MAX_EDGES 72

//CAMPUSES
#define NORMAL_CAMPUS 0
#define GO8_CAMPUS 1

//PATHS
#define BACK 'B'
#define LEFT 'L'
#define RIGHT 'R'

//DIRECTIONS
#define UNDEFINED -1

#define ACROSS 0
#define ACROSS_X 2
#define ACROSS_Y 0

#define ANGLE_LEFT 1
#define ANGLE_LEFT_X -1
#define ANGLE_LEFT_Y 1

#define ANGLE_RIGHT 2
#define ANGLE_RIGHT_X 1
#define ANGLE_RIGHT_Y 1

//INITIAL CONDITONS
#define INITIAL_TURN -1
#define INITIAL_DIR_POS { -2, 6 }

#define CAMPUS_A_START { -1, 5 }
#define CAMPUS_B_START { -8, 2 }
#define CAMPUS_C_START { -7, -3 }

#define INITIAL_BTHINKING 0
#define INITIAL_BPROBLEMSOLVING 3
#define INITIAL_BHOWTHINGSWORK 3
#define INITIAL_BJOBS 1
#define INITIAL_BTVJOBS 1
#define INITIAL_BMONEY 1

typedef struct _player player;
typedef struct _tile tile;

typedef struct _pos {
   double x;
   double y;
} pos;

typedef struct _line {
   pos point1;
   pos point2;
} line;

typedef struct _direction {
   int type;
   int reversed;
} direction;

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
   int bHowThingsWork;
   int bJobs;
   int bTVJobs;
   int bMoney;
} studentTypes;

typedef struct _playerStats {
   int points;
   int arcGrants;
   int campuses;
   int publications;
   int IPs;
} playerStats;

struct _player {
   int playerID;
   vertice *campuses[MAX_VERTICES];
   int students[STUDENT_TYPES];
   playerStats stats;
};

typedef struct _gameStats {
   player *maxPoints;
   player *maxArcGrants;
   player *maxPublications;
   int turnNumber;
   player *whoseTurn;
   int numGO8s;
} gameStats;

typedef struct _game {
   player players[MAX_PLAYERS+1];
   tile tiles[MAX_TILES];
   edge edges[MAX_EDGES];
   vertice vertices[MAX_VERTICES];
   gameStats stats;
} game;

double absolute(double x);
void appendPath(path *given, path *add);
void buildBoard(Game g, int disciplines[], int dice[]);
void buildCampus(Game g, player *p, vertice *v, int campusType);
int buildHexagonEdges(Game g, tile *tile, int startIndex);
int buildHexagonVertices(Game g, tile *tile, int startIndex);
void buildInitialConditions(Game g);
int calculateGO8s(player *p);
void calculateAllPoints(Game g);
void calculatePoints(Game g, player *p);
direction getDirection(pos from, pos to);
edge *getEdge(Game g, pos p);
edge *getEdgeFromPath(Game g, path p);
edge *getHexagonEdge(Game g, tile *tile, int index);
pos getHexagonEdgePosition(Game g, tile *tile, int index);
vertice *getHexagonVertice(Game g, tile *tile, int index);
pos getHexagonVerticePosition(Game g, tile *tile, int index);
pos getNewPosition(pos position, direction dir);
vertice *getVertice(Game g, pos position);
vertice *getVerticeFromPath(Game g, path p);
int edgeExists(Game g, pos p);
pos midpoint(pos p1, pos p2);
pos negative (pos p);
pos relativePos(pos p1, pos p2);
int sameSign(double x, double y);
pos takeTurn(pos from, pos current, char turnType);
pos takeTurns(pos from, pos current, path p);
line takeTurnsDetailed(pos from, pos current, path p);
void testInternals (Game g);
int verticeExists(Game g, pos position);

double absolute(double x) {
   if (x < 0) {
      x *= -1;
   }
   return x;
}

void appendPath(path *given, path *add) {
   int i = 0;
   while ((*given)[i] != 0 && i < PATH_LIMIT) {
      i++;
   }
   int j = 0;
   while ((*given)[j] != 0 && ((i + j - 1) < PATH_LIMIT)) {
      (*given)[i + j] = (*add)[j];
      j++;
   }
   (*given)[i+j] = 0;
}

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
   testInternals(g);

}

void buildCampus(Game g, player *p, vertice *v, int campusType) {
   assert(p != NULL && v != NULL);
   v->campus = p;
   v->campusType = campusType;
   p->campuses[p->stats.campuses] = v;
   p->stats.campuses++;
}

int buildHexagonEdges(Game g, tile *tile, int startIndex) {
    int i = 0;
    pos edgePos;
    while (i < HEXAGON_SIDES) {
      edgePos = getHexagonEdgePosition(g, tile, i);
      if (!edgeExists(g, edgePos)) {
         g->edges[startIndex].position = edgePos;
         g->edges[startIndex].arcGrant = VACANT_ARC;
         startIndex++;

      }
      i++;
    }
    return startIndex;
}

int buildHexagonVertices(Game g, tile *tile, int startIndex) {
   direction dir = { 0, 0 };
   pos newPos;
   while (dir.type < 3) {
      while (dir.reversed < 2) {
         newPos = getNewPosition(tile->position, dir);
         if (!verticeExists(g, newPos)) {
            g->vertices[startIndex].position = newPos;
            g->vertices[startIndex].campusType = VACANT_VERTEX;
            g->vertices[startIndex].campus = NULL;
            startIndex++;
         }
         dir.reversed++;
      }
      dir.reversed = 0;
      dir.type++;
   }
   return startIndex;
}

void buildInitialCampuses(Game g) {
   pos positions[MAX_PLAYERS] = { CAMPUS_A_START, CAMPUS_B_START, CAMPUS_C_START };
   int i = 0;
   while (i < MAX_PLAYERS) {
      buildCampus(g, &g->players[i + 1], getVertice(g, positions[i]), NORMAL_CAMPUS);
      buildCampus(g, &g->players[i + 1], getVertice(g, negative(positions[i])), NORMAL_CAMPUS);
      i++;
   }
}

void buildInitialConditions(Game g) {
   g->stats.turnNumber = INITIAL_TURN;
   int i = 0;
   while (i <= MAX_PLAYERS) {
      g->players[i].playerID = i;
      g->players[i].stats.arcGrants = 0;
      g->players[i].stats.campuses = 0;
      g->players[i].stats.IPs = 0;
      g->players[i].stats.publications = 0;
      g->players[i].students[STUDENT_THD] = INITIAL_BTHINKING;
      g->players[i].students[STUDENT_BPS] = INITIAL_BPROBLEMSOLVING;
      g->players[i].students[STUDENT_BQN] = INITIAL_BHOWTHINGSWORK;
      g->players[i].students[STUDENT_MJ] = INITIAL_BJOBS;
      g->players[i].students[STUDENT_MTV] = INITIAL_BTVJOBS;
      g->players[i].students[STUDENT_MMONEY] = INITIAL_BMONEY;
      i++;
   }
   g->stats.maxArcGrants = &g->players[NO_ONE];
   g->stats.maxPoints = &g->players[NO_ONE];
   g->stats.maxPublications = &g->players[NO_ONE];
   buildInitialCampuses(g);
   calculateAllPoints(g);
}

int calculateGO8s(player *p) {
   int count = 0;
   int i = 0;
   while (i < p->stats.campuses) {
      if (p->campuses[i]->campusType == GO8_CAMPUS) {
         count++;
      }
      i++;
   }
   return count;
}

void calculateAllPoints(Game g) {
   int i = 1;
   while (i <= MAX_PLAYERS) {
      calculatePoints(g, &g->players[i]);
      i++;
   }
}

void calculatePoints(Game g, player *p) {
   int points = 0;
   points += p->stats.campuses * 10;
   points += calculateGO8s(p) * 10;
   points += p->stats.arcGrants * 2;
   points += p->stats.IPs * 10;
   if (g->stats.maxArcGrants == p) {
      points += 10;
   }
   if (g->stats.maxPublications == p) {
      points += 10;
   }
   p->stats.points = points;
}

pos negative (pos p) {
   p.x *= -1;
   p.y *= -1;
   return p;
}

Game newGame (int disciplines[], int dice[]) {
   Game created = malloc(sizeof(struct _game));
   assert(created != NULL);
   buildBoard(created, disciplines, dice);
   buildInitialConditions(created);
   return created;
}

direction getDirection(pos from, pos to) {
   direction result;
   pos relPos;
   relPos.x = to.x - from.x;
   relPos.y = to.y - from.y;
   if (relPos.y == 0) {
      result.type = ACROSS;
      result.reversed = (relPos.x < 0);
   } else if (!sameSign(relPos.x, relPos.y)) {
      result.type = ANGLE_LEFT;
      result.reversed = (relPos.x < 0);
   } else if (sameSign(relPos.x, relPos.y)) {
      result.type = ANGLE_RIGHT;
      result.reversed = (relPos.x > 0);
   } else {
      result.type = UNDEFINED;
   }
   return result;
}

pos getNewPosition(pos position, direction dir) {
   pos relPos = { 0 , 0 };
   if (dir.type == ACROSS) {
      relPos.x += (double)ACROSS_X;
      relPos.y += (double)ACROSS_Y;
   } else if (dir.type == ANGLE_LEFT) {
      relPos.x += (double)ANGLE_LEFT_X;
      relPos.y += (double)ANGLE_LEFT_Y;
   } else {
      relPos.x += (double)ANGLE_RIGHT_X;
      relPos.y += (double)ANGLE_RIGHT_Y;
   }
   if (dir.reversed) {
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

edge *getEdgeFromPath(Game g, path p) {
   pos from = INITIAL_DIR_POS;
   pos current = CAMPUS_A_START;
   line result = takeTurnsDetailed(from, current, p);
   return getEdge(g, midpoint(result.point1, result.point2));
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
   direction dir;
   if (index == 0 || index == 6) {
      dir.type = ANGLE_LEFT;
      dir.reversed = FALSE;
   } else if (index == 1) {
      dir.type = ANGLE_RIGHT;
      dir.reversed = FALSE;
   } else if (index == 2) {
      dir.type = ACROSS;
      dir.reversed = FALSE;
   } else if (index == 3) {
      dir.type = ANGLE_LEFT;
      dir.reversed = TRUE;
   } else if (index == 4) {
      dir.type = ANGLE_RIGHT;
      dir.reversed = TRUE;
   } else {
      dir.type = ACROSS;
      dir.reversed = TRUE;
   }
   return getNewPosition(tile->position,dir);
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

vertice *getVerticeFromPath(Game g, path p) {
   pos from = INITIAL_DIR_POS;
   pos current = CAMPUS_A_START;
   return getVertice(g, takeTurns(from, current, p));
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

pos relativePos(pos p1, pos p2) {
   pos rel;
   rel.x = p2.x - p1.x;
   rel.y = p2.y - p1.y;
   return rel;
}

int sameSign(double x, double y) {
   return (absolute(x * y) == (x * y));
}

pos takeTurn(pos from, pos current, char turnType) {
   direction travelling = getDirection(from, current);
   if (turnType == LEFT) {
      travelling.type--;
      //travelling.reversed = !travelling.reversed;
   } else if (turnType == RIGHT) {
      travelling.type++;
      travelling.reversed = !travelling.reversed;
   } else {
      travelling.reversed = !travelling.reversed;
   }
   if (travelling.type < 0) {
      travelling.type += 3;
   }
   travelling.type = (travelling.type % 3);
   return getNewPosition(current, travelling);
}

pos takeTurns(pos from, pos current, path p) {
   line result = takeTurnsDetailed(from, current, p);
   return result.point2;
}

line takeTurnsDetailed(pos from, pos current, path p) {
   int i = 0;
   line newLine;
   while (i < PATH_LIMIT && p[i] != 0) {
      newLine.point2 = takeTurn(from, current, p[i]);
      from = current;
      current = newLine.point2;
      i++;
   }
   newLine.point1 = current;
   return newLine;
}

//INTERNAL TESTING///

/////////////////////

void testInternals (Game g) {

   //Single tests
   pos i = { -1, 1};
   pos c = { 1, 1};
   pos f = takeTurn(i, c, 'R');
   assert(f.x == 2 && f.y == 0);
   f = takeTurn(i, c, 'L');
   assert(f.x == 2 && f.y == 2);
   f = takeTurn(c, f, 'R');
   assert(f.x == 4 && f.y == 2);

   //Multi-tests

   char given[PATH_LIMIT] =  { RIGHT, RIGHT, RIGHT, RIGHT, RIGHT, RIGHT, 0 };
   pos finalPos = takeTurns(i, c, given);
   assert(finalPos.x == c.x && finalPos.y == c.y);

   printf("All internal tests passed! You are awesome!\n");
}

////////////////////

int verticeExists(Game g, pos position) {
   vertice *test = getVertice(g, position);
   return (test != NULL);
}

///////////////////////
//Interface Functions//
///////////////////////

//COMPLETED
void disposeGame (Game g) {
   free(g);
}

void makeAction (Game g, action a) {
   if (a.actionCode == PASS) {

   } else if (a.actionCode == BUILD_CAMPUS) {

   } else if (a.actionCode == BUILD_GO8) {

   } else if (a.actionCode == OBTAIN_ARC) {

   } else if (a.actionCode == START_SPINOFF) {

   } else if (a.actionCode == OBTAIN_PUBLICATION) {

   } else if (a.actionCode == OBTAIN_IP_PATENT) {

   } else {

   }
}

//COMPLETED
void throwDice (Game g, int diceScore) {
   int i = 0;
   int j = 0;
   vertice *v;
   while (i < MAX_TILES) {
      while (j < HEXAGON_SIDES) {
         v = getHexagonVertice(g, &g->tiles[i], j);
         if (v != NULL && v->campus != NULL) {
            v->campus->students[g->tiles[i].discipline] += 1 + v->campusType;
         }
         j++;
      }
      j = 0;
      i++;
   }
   g->stats.turnNumber++;
}

//COMPLETED
int getDiscipline (Game g, int regionID) {
   return g->tiles[regionID].discipline;
}

//COMPLETED
int getDiceValue (Game g, int regionID) {
   return g->tiles[regionID].score;
}

//COMPLETED
int getMostARCs (Game g) {
   return g->stats.maxArcGrants->playerID;
}

//COMPLETED
int getMostPublications (Game g) {
   return g->stats.maxPublications->playerID;
}

//COMPLETED
int getTurnNumber (Game g) {
   return g->stats.turnNumber;
}

//COMPLETED
int getWhoseTurn (Game g) {
   return g->stats.whoseTurn->playerID;
}

//COMPLETED
int getCampus(Game g, path pathToVertex) {
   vertice *loc = getVerticeFromPath(g, pathToVertex);
   return (loc->campus->playerID + (3 * loc->campusType));
}

//COMPLETED
int getARC(Game g, path pathToEdge) {
   return getEdgeFromPath(g, pathToEdge)->arcGrant;
}

int isLegalAction (Game g, action a) {
   return 0;
}

//COMPLETED
int getKPIpoints (Game g, int player) {
   return g->players[player].stats.points;
}

//COMPLETED
int getARCs (Game g, int player) {
   return g->players[player].stats.arcGrants;
}

//COMPLETED
int getGO8s (Game g, int player) {
   return calculateGO8s(&g->players[player]);
}

//COMPLETED
int getCampuses (Game g, int player) {
   return g->players[player].stats.campuses;
}

//COMPLETED
int getIPs (Game g, int player) {
   return g->players[player].stats.IPs;
}

//COMPLETED
int getPublications (Game g, int player) {
   return g->players[player].stats.publications;
}

//COMPLETED
int getStudents (Game g, int player, int discipline) {
   return g->players[player].students[discipline];
}

int getExchangeRate (Game g, int player,
                     int disciplineFrom, int disciplineTo) {
   return 0;
}

