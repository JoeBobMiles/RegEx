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
#include <string.h>

#define MAX_STATE_BUFFER_SIZE 128

/** DFA typedefs. */
typedef struct state {
    // TODO[joe] Figure out how to handle multiple valid matches.
    char          Match;
    // TODO[joe] Make this an array of pointers? How?
    struct state *NextState;
    int           Accept;
} state;

/** BEGIN RegEx code. */

/**
 * This function builds our DFA from the given Pattern and stores all of it's
 * states into the pre-allocated StateBuffer. The first state in the
 * StateBuffer is the DFA's initial state.
 */
static
void BuildDFA(const char* Pattern, state* StateBuffer)
{
    int    StateBufferOffset = 0;
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
                // NOTE[joe] We currently don't have guarantees on state count.
                // So we're just gonna crash if we exceed max state buffer
                // size.
                assert(StateBufferOffset < MAX_STATE_BUFFER_SIZE - 1)

                state *State = &StateBuffer[StateBufferOffset++];
                State->Match = Pattern[i];

                if (LastAppendedState != 0)
                    LastAppendedState->NextState = State;

                LastAppendedState = State;
            } break;
        }
    }
}

/**
 * This function traverses a DFA, given to us as a pointer to its initial
 * state. It explores the DFA until it finds states that have no exit, which it
 * considers the accept states of the DFA. Assuming that the DFA is well
 * formed (the DFA is not a forest), then this should be true.
 */
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
    // TODO[joe] Allocate reasonable amounts of space for the DFA.
    // I believe we can use the length of the pattern string to determine how
    // much space we'll need, but I can't be sure until the DFA construction
    // algorithm has been nailed down.
    state Automata[MAX_STATE_BUFFER_SIZE] = { 0 };
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
