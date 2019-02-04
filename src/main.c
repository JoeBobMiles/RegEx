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
    char *Pattern;
    fa   *Automata;
} regex;

void InitRegEx(regex *RegEx, const char *Pattern)
{
    int PatternLength = strlen(Pattern);

    // @TODO[joe] Find some way to not malloc this...
    RegEx->Pattern = (char *) malloc(sizeof(char) * (PatternLength+1));
    strcpy_s(RegEx->Pattern, PatternLength+1, Pattern);
    RegEx->Pattern[PatternLength] = '\0';

    RegEx->Automata = (fa *) malloc(sizeof(fa));
    RegEx->Automata->InitialState = 0;
}

int Match(const char* Pattern, const char* String)
{
    regex RegEx = {};

    InitRegEx(&RegEx, "[a-zA-Z]+");

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
                State->Match = RegEx.Pattern[i];
                State->NextState = 0;

                if (RegEx.Automata->InitialState == 0)
                    RegEx.Automata->InitialState = State;

                else
                    LastAppendedState->NextState = State;

                LastAppendedState = State;
            } break;
        }
    }

    state *NextState = RegEx.Automata->InitialState;

    int MatchMade = 0;

    int StringLength = strlen(String);
    for (int i = 0; i < StringLength; i++) {
        if (String[i] == NextState->Match)
            NextState = NextState->NextState;

        if (NextState == 0) {
            MatchMade = 1;
            break;
        }
    }

    return MatchMade;
}

int main(void)
{

    int MatchMade = Match("[a-zA-Z]+", "[a-zA-Z]+");

    if (MatchMade)
        printf("Match was made!\n");

    else
        printf("Could not find a match!\n");

    return 0;
}
