/**
 * @file regex.h
 * @author Joseph Miles <josephmiles2015@gmail.com>
 * @date 2019-02-18
 *
 * This is the main header file of the RegEx project. Including this header
 * into your code will include the RegEx project into your code.
 */

#ifndef REGEX_H
#define REGEX_H

// C standard library include(s)
#include <stdlib.h>
#include <string.h>

// Internal include(s)
#include "regex.h"

/** DFA typedefs. */
typedef struct state {
    char          Match;
    struct state *NextState;
    int           Accept;
} state;

typedef struct {
    state *InitialState;
} dfa;

/** Regex typedef */
typedef struct {
    char *Pattern;
    dfa  *Automata;
} regex;

void InitRegEx(regex *RegEx, const char *Pattern)
{
    int PatternLength = strlen(Pattern);

    // @TODO[joe] Find some way to not malloc this...
    RegEx->Pattern = (char *) malloc(sizeof(char) * (PatternLength+1));
    strcpy_s(RegEx->Pattern, PatternLength+1, Pattern);
    RegEx->Pattern[PatternLength] = '\0';

    RegEx->Automata = (dfa *) malloc(sizeof(dfa));
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
                    // @TODO[joe] Don't compute accept here, do it later.
                    // This is something that Levi suggested. Traversing the
                    // DFA at a later time to find the accept states will lead
                    // to less hard to track down bugs later on.
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

                else {
                    // @TODO[joe] Don't compute accept here, do it later.
                    LastAppendedState->Accept = 0;
                    LastAppendedState->NextState = State;
                }

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
#endif
