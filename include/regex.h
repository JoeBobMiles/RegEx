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
#include <assert.h>

#define MAX_STATE_BUFFER_SIZE 128

/** DFA typedefs. */

typedef struct transition {
    void *NextState; // NOTE[joe] This will **ALWAYS** be a State.
    char  Match;
} transition;

typedef struct transition_table {
    transition   *Rows;
    unsigned int  RowCount;
    unsigned int  Length; // The number of rows allocated to this table.
} transition_table;

typedef struct state {
    transition_table Transitions;
    // TODO[joe] This is a lot of memory for a boolean flag.
    unsigned int     Accept;
} state;

/** BEGIN RegEx code. */

static inline
void AddTransition(transition_table *Table, char Match, state *NextState)
{
    if (Table->Length == 0)
    {
        Table->Rows = (transition *) malloc(sizeof(transition));
        Table->Length = 1;
    }
    else if (Table->RowCount == Table->Length)
    {
        Table->Length *= 2;
        Table->Rows = (transition *)
                      realloc(Table->Rows, sizeof(transition) * Table->Length);
    }

    transition *Transition = &(Table->Rows[Table->RowCount++]);

    Transition->Match = Match;
    Transition->NextState = NextState;
}

static inline
void CleanupTransitionTables(state *Automata, unsigned int StateCount)
{
    for (unsigned int i = 0; i < StateCount; i++)
        free(Automata[i].Transitions.Rows);
}

/**
 * This function builds our DFA from the given Pattern and stores all of it's
 * states into the pre-allocated StateBuffer. The first state in the
 * StateBuffer is the DFA's initial state.
 */
static
void BuildDFA(const char* Pattern, state* StateBuffer)
{
    int    StateBufferOffset = 0;
    state *LastAppendedState = &StateBuffer[StateBufferOffset++];

    int PatternLength = strlen(Pattern);
    for (int i = 0; i < PatternLength; i++)
    {
        switch (Pattern[i])
        {
            // TODO[joe] Implement special character cases.
            case '+':
            {
                // NOTE[joe] Cause a crash if we don't have a previous state.
                assert(LastAppendedState != 0);

                AddTransition(&LastAppendedState->Transitions,
                              Pattern[i - 1],
                              LastAppendedState);
            } break;

            default:
            {
                // NOTE[joe] We currently don't have guarantees on state count.
                // So we're just gonna crash if we exceed max state buffer
                // size.
                assert(StateBufferOffset < MAX_STATE_BUFFER_SIZE - 1);

                state *State = &StateBuffer[StateBufferOffset++];

                AddTransition(&LastAppendedState->Transitions,
                              Pattern[i],
                              State);

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
 * FIXME[joe] This function produces a very crude approximation.
 */
static
void MarkAcceptStates(state* InitialState)
{
    unsigned int TransitionCount = InitialState->Transitions.RowCount;

    state **NewStates = (state **) malloc(sizeof(state *) * TransitionCount);

    char *NewStateMemory = (char *) NewStates;
    for (unsigned int i = 0; i < sizeof(state *) * TransitionCount; i++)
        NewStateMemory[i] = 0;

    unsigned int StateCount = 0;

    for (unsigned int i = 0; i < TransitionCount; i++)
    {
        if (InitialState->Transitions.Rows[i].NextState != InitialState)
        {
            NewStates[StateCount++] =
                InitialState->Transitions.Rows[i].NextState;
        }
    }

    // NOTE[joe] If TransitionCount == 0, we never zeroed NewStates.
    if (TransitionCount == 0 || NewStates[0] == 0)
        InitialState->Accept = 1;

    else {
        for (unsigned int i = 0; i < StateCount; i++)
            MarkAcceptStates(NewStates[i]);
    }

    free(NewStates);
}

/**
 * This is the main function that our users will be using. It takes two
 * strings, a Pattern and a String to match. It compiles the Pattern into a
 * regex and then uses the regex it generated to check to see if there is a
 * substring in the given String that matches it.
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

    state *CurrentState = Automata;
    int    MatchMade = 0;

    int StringLength = strlen(String);
    for (int i = 0; i < StringLength; i++)
    {
        for (int j = 0; j < CurrentState->Transitions.RowCount; j++)
        {
            // NOTE[joe] Take the first matching transition.
            // This is because we still end up with non-deterministic
            // transitions in our automata.
            if (String[i] == CurrentState->Transitions.Rows[j].Match)
            {
                CurrentState = CurrentState->Transitions.Rows[j].NextState;
                break;
            }
        }

        // TODO[joe] We need to be able to unset MatchMade.
        // Sometimes we end up in invalid states and we need to be able to say
        // that we failed to match.
        if (CurrentState->Accept && MatchMade != 1)
            MatchMade = 1;
    }

    CleanupTransitionTables(Automata, MAX_STATE_BUFFER_SIZE);

    return MatchMade;
}

#endif
