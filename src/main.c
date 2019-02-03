/*
    @author Joseph Miles <josephmiles2015@gmail.com>
    @date 2019-02-03
    @summary This is the main driver for the RegEx project.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char Pattern[0];
} regex;

typedef struct state {
    char            Match;
    struct state   *NextState;
} state;

typedef struct {
    state *InitialState;
} fa;

void InitRegEx(regex *RegEx, const char *pattern)
{
    strcpy((*RegEx).Pattern, pattern);
    (*RegEx).Pattern[strlen(pattern)] = '\0';
}

int main(void)
{
    regex RegEx = {};

    InitRegEx(&RegEx, "[a-zA-Z]+");

    /*
    Now we're going to implement a finite automata generator.

    What does a regular expression need?
     -  See PCRE syntax documentation: http://www.pcre.org/current/doc/html/pcre2syntax.html
    */

    fa Automata = {};
    state *LastAppendedState;

    int PatternLength = strlen(RegEx.Pattern);
    for (int i = 0; i < PatternLength; i++) {
        switch (RegEx.Pattern[i]) {
            // @TODO[joe] Implement special character cases.
            default:
            {
                // @TODO[joe] Implement a "stack" way of allocating states.
                state *State = (state *) malloc(sizeof(state));
                (*State).Match = RegEx.Pattern[i];

                if (Automata.InitialState == 0)
                    Automata.InitialState = State;

                else
                    (*LastAppendedState).NextState = State;

                LastAppendedState = State;
            } break;
        }
    }

    for (state *CurrentState = Automata.InitialState;
         CurrentState;
         CurrentState = (*CurrentState).NextState)
    {
        printf("%c", (*CurrentState).Match);
    }

    printf("\n");

    return 0;
}
