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
// TODO[joe] Get away from using these?
#include <stdlib.h>
#include <string.h>

/** DFA typedefs. */
typedef struct state {
    char          Match;
    struct state *NextState;
    int           Accept;
} state;

/** BEGIN RegEx code. */

static
void BuildDFA(const char* Pattern, state* StateBuffer)
{
    int StateBufferOffset = 0;
    state AutomataInitialState = (state) { 0 };
    state *LastAppendedState = 0;

    int PatternLength = strlen(Pattern);
    for (int i = 0; i < PatternLength; i++)
    {
        switch (Pattern[i])
        {
            // TODO[joe] Implement special character cases.
            case '+':
            {
                if (LastAppendedState == 0)
                {
                    // TODO[joe] Error reporting.
                    break;
                }

                else
                {
                    LastAppendedState->NextState = LastAppendedState;
                }
            } break;

            default:
            {
                // TODO[joe] Allocate this to the stack instead.
                // The algorithm for constructing the DFA needs to be nailed
                // down first before we can decide if we are able to stack
                // allocate.
                state *State = (state *) malloc(sizeof(state));
                State->Match = Pattern[i];
                State->NextState = 0;
                State->Accept = 0;

                if (StateBufferOffset == 0 && StateBuffer->Match == 0)
                    StateBuffer[StateBufferOffset++] = *State;

                else
                    LastAppendedState->NextState = State;

                LastAppendedState = State;
            } break;
        }
    }
}

static
void MarkAcceptStates(state* InitialState)
{
    if (InitialState->NextState == 0)
        InitialState->Accept = 1;

    else
        MarkAcceptStates(InitialState->NextState);
}

/**
 * This is the main function that our users will be using. It takes two
 * strings, a Patter and a String to match. It compiles the Pattern into a
 * regex and then uses the regex it generated to check to see if the String
 * matches it or not.
 */
int Match(const char* Pattern, const char* String)
{
    state Automata[128] = { 0 };
    BuildDFA(Pattern, Automata);

    MarkAcceptStates(Automata);

    state *CurrentState = 0;
    state *NextState = Automata;

    int MatchMade = 0;

    int StringLength = strlen(String);
    for (int i = 0; i < StringLength; i++)
    {
        if (String[i] == NextState->Match)
        {
            CurrentState = NextState;
            NextState = CurrentState->NextState;
        }

        if (CurrentState != 0 && CurrentState->Accept)
            MatchMade = 1;
    }

    return MatchMade;
}

#endif
