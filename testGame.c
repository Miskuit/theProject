#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct _game {

} game;

int main (int argc, char *argv[]) {
   Game g = newGame(DEFAULT_DISCIPLINES, DEFAULT_DICE);
   assert(getTurnNumber(g) == -1);
   assert(getMostArcs(g) == 0);
   assert(getMostPublications(g) == 0);

   int region = 0;
   while (region < 19) { }


   //Test the dice value

   //Test initial conditions for each player
   int player = 0;
   while (player < 3) {
      assert(getARCs(g, player) == 2);
      assert(getCampuses(g, player) == 2);
      assert(getG08s(g, player) == 0);
      assert(getIPs(g, player) == 0);
      assert(getPublications(g, player) == 0);
      assert(getStudents(g, player, STUDENT_THD) == 0);
      assert(getStudents(g, player, STUDENT_BPS) == 3);
      assert(getStudents(g, player, STUDENT_BQN) == 0);
      assert(getStudents(g, player, STUDENT_MJ) == 1);
      assert(getStudents(g, player, STUDENT_MTV) == 1);
      assert(getStudents(g, player, STUDENT_MMONEY) == 1);
      assert(getKPIpoints(g, player) == 24);
      int actionCode = 0;
      path p = {0,0,0};
      action a;
      while (action < 8) {
         a = { actionCode, p, 0, 0 };
         assert(isLegalAction(g, a) == FALSE);
         actionCode++;
      }
      player++;
   }

   testLegalAction(g);
}