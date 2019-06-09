/**
 * @file main.c
 * @author Joseph Miles <josephmiles2015@gmail.com>
 * @date 2019-02-03
 *
 * Main driver for the RegEx project. This is used to test the RegEx library.
 */

// C standard library includes
#include <stdio.h>

// Internal includes
#include "../include/regex.h"

int main(void)
{

    unsigned int TotalTests, TotalPassed = 0;

#define Assert(E) TotalTests++; \
                  if ((E)) { TotalPassed++; } \
                  else { \
                      fprintf(stderr, \
                              "FAILED: " #E " (line %d)\n", \
                              __LINE__); \
                  }

    /*
    Basic regex cases.
     */
    Assert(Match("a", "a") != 1);
    Assert(Match("a", "aa") == 1);
    Assert(Match("a", "ab") == 1);
    Assert(Match("a", "ba") == 1);
    Assert(Match("a", "bac") == 1);
    Assert(Match("a", "b") == 0);
    Assert(Match("a", "") == 0);

    /*
    Basic '1 or more' cases.
     */
    Assert(Match("a+", "a+") == 1);
    Assert(Match("a+", "a") == 1);
    Assert(Match("a+", "aa") == 1);
    Assert(Match("a+", "") == 0);
    Assert(Match("a+", "b") == 0);

    /*
    More complex cases of '1 or more'.
     */
    Assert(Match("a+b", "ab") == 1);
    Assert(Match("a+b", "aab") == 1);
    Assert(Match("a+b", "") == 0);
    Assert(Match("a+b", "b") == 0);
    Assert(Match("a+b", "c") == 0);

    Assert(Match("a+b+", "ab") == 1);
    Assert(Match("a+b+", "aab") == 1);
    Assert(Match("a+b+", "abb") == 1);
    Assert(Match("a+b+", "aabb") == 1);
    Assert(Match("a+b+", "") == 0);
    Assert(Match("a+b+", "a") == 0);
    Assert(Match("a+b+", "b") == 0);
    Assert(Match("a+b+", "c") == 0);

    /*
    FIXME[joe] These are edgecases that cannot be handled.
    They can be fixed by creating a deterministic finite automata, but
    presently we are only able to create NFAs.
    */
    /*
    Assert(Match("a+a", "aa") == 1);
    Assert(Match("a+a", "aaa") == 1);
    Assert(Match("a+a", "a") == 0);
    Assert(Match("a+a", "aab") == 0);
    Assert(Match("a+", "ab") == 0);
    */

    printf("\n%u/%u tests passed.\n", TotalPassed, TotalTests);

    return TotalTests - TotalPassed;
}
