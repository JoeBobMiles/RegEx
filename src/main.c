/*
    @file       main.c
    @author     Joseph Miles <josephmiles2015@gmail.com>
    @date       2019-02-03
    @summary    Main driver for the RegEx project.
*/

// C standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Internal includes
#include "fa.h"

typedef struct {
    char Pattern[0];
} regex;

void InitRegEx(regex *RegEx, const char *Pattern)
{
    int PatternLength = strlen(Pattern);

    strcpy_s((*RegEx).Pattern, PatternLength+1, Pattern);
    (*RegEx).Pattern[PatternLength] = '\0';
}

int main(void)
{
    regex RegEx = {};

    InitRegEx(&RegEx, "[a-zA-Z]+");

    fa Automata = {};
    state *LastAppendedState;

    int PatternLength = strlen(RegEx.Pattern);
    for (int i = 0; i < PatternLength; i++) {
        switch (RegEx.Pattern[i]) {
            // @TODO[joe] Implement special character cases.
            default:
            {
                // @TODO[joe] Implement a "stack" way of allocating states.
                // Just so that we can perform a bulk deallocate instead of
                // worrying about tracking each individual node in the FA.
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
         CurrentState != 0;
         CurrentState = (*CurrentState).NextState)
    {
        printf("%c", (*CurrentState).Match);
    }

    printf("\n");

    return 0;
}
