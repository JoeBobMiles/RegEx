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
#include <assert.h>

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

    InitRegEx(&RegEx, Pattern);

    state *LastAppendedState = 0;

    int PatternLength = strlen(RegEx.Pattern);
    for (int i = 0; i < PatternLength; i++) {
        switch (RegEx.Pattern[i]) {
            // @TODO[joe] Implement special character cases.
            case '+':
            {
                if (LastAppendedState == 0) {
                    // @TODO[joe] Error reporting.
                    break;
                }

                else {
                    LastAppendedState->Accept = 1;
                    LastAppendedState->NextState = LastAppendedState;
                }
            } break;

            default:
            {
                // @TODO[joe] Implement a "stack" way of allocating states.
                // Just so that we can perform a bulk deallocate instead of
                // worrying about tracking each individual node in the FA.
                state *State = (state *) malloc(sizeof(state));
                State->Match = RegEx.Pattern[i];
                State->NextState = 0;
                State->Accept = 0;

                if (RegEx.Automata->InitialState == 0)
                    RegEx.Automata->InitialState = State;

                else
                    LastAppendedState->NextState = State;

                LastAppendedState = State;
            } break;
        }
    }

    LastAppendedState->Accept = 1;

    state *CurrentState = 0;
    state *NextState = RegEx.Automata->InitialState;

    int MatchMade = 0;

    int StringLength = strlen(String);
    for (int i = 0; i < StringLength; i++) {
        if (String[i] == NextState->Match) {
            CurrentState = NextState;
            NextState = CurrentState->NextState;
        }

        if (CurrentState != 0 && CurrentState->Accept)
            MatchMade = 1;
    }

    return MatchMade;
}

int main(void)
{
    /*
    Basic '1 or more' cases.
     */
    assert(Match("a+", "a+") == 1);
    assert(Match("a+", "a") == 1);
    assert(Match("a+", "aa") == 1);
    // @NOTE[joe] The empty string is a special case we may have to handle at
    // a later time...
    assert(Match("a+", "") == 0);
    assert(Match("a+", "b") == 0);

    /*
    More complex cases of '1 or more'.
    @NOTE[joe] These just work... I don't trust that.
     */
    assert(Match("a+b", "ab") == 1);
    assert(Match("a+b", "aab") == 1);
    assert(Match("a+b", "") == 0);
    assert(Match("a+b", "b") == 0);
    assert(Match("a+b", "c") == 0);

    return 0;
}
