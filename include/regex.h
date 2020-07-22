#ifndef REGEX_H
#define REGEX_H

/// C standard library include(s)
// TODO: Move to custom allocator and user-provided memory pool.
#include <stdlib.h>
// TODO: Replace with inline versions.
#include <string.h>
// TODO: Replace assert() calls with non-fatal calls.
#include <assert.h>

/** DFA typedefs. */

typedef struct transition_s
{
    void* next_state; // NOTE[joe] This will **ALWAYS** be a State.
    char  match;
} transition;

typedef struct transition_table_s
{
    transition*   rows;
    unsigned int  row_count;
    unsigned int  _length; // The number of rows allocated to this table.
} transition_table;

typedef struct state_s
{
    transition_table transitions;
    // TODO[joe] This is a lot of memory for a boolean flag.
    unsigned int     accept;
} state;

typedef struct state_heap_s
{
    state* states;
    unsigned int _next_state_index;
    unsigned int _size;
} state_heap;

typedef struct alphabet_s
{
    char* glyphs;
    unsigned int length;
    unsigned int _size;
} alphabet;


/** BEGIN RegEx code. */

static void
add_transition(transition_table* table, char match, state* next_state)
{
    // Abort if we've already added the given transition to the table.
    for (unsigned int i = 0; i < table->row_count; i++)
    {
        // FIXME[joe] Access violation, trying to read 0xFFFFFFFFFFFFFFFF.
        // I'm pretty sure this is due to being handed a dangling pointer, but
        // we'll need to test that hypothesis in Visual Studio.
        if (table->rows[i].next_state == next_state &&
            table->rows[i].match == match)
        {
            return;
        }
    }

    if (table->_length == 0)
    {
        table->rows = (transition*) malloc(sizeof(transition));
        table->_length = 1;
    }
    else if (table->row_count == table->_length)
    {
        table->_length *= 2;
        table->rows = (transition*)
            realloc(table->rows, sizeof(transition) * table->_length);
    }

    transition* transition = &(table->rows[table->row_count++]);

    transition->match = match;
    transition->next_state = next_state;
}

static void
append_glyph(alphabet *alphabet, char glyph)
{
    // Abort if the glyph is already in the alphabet.
    for (unsigned int i = 0; i < alphabet->length; i++)
    {
        if (glyph == alphabet->glyphs[i])
            return;
    }

    if (alphabet->_size == 0)
    {
        alphabet->_size = 1;
        alphabet->glyphs = (char*)malloc(sizeof(char));
    }
    else if (alphabet->length == alphabet->_size)
    {
        alphabet->_size *= 2;
        alphabet->glyphs = (char*)
            realloc(alphabet->glyphs, sizeof(char) * alphabet->_size);
    }

    alphabet->glyphs[alphabet->length++] = glyph;
}

static inline void
cleanup_transition_tables(state* automata, unsigned int state_count)
{
    for (unsigned int i = 0; i < state_count; i++)
        free(automata[i].transitions.rows);
}

static state*
new_state(state_heap *state_heap)
{
    if (state_heap->_size == 0)
    {
        state_heap->_size = 1;
        state_heap->_next_state_index = 0;
        state_heap->states = malloc(sizeof(state) * state_heap->_size);
    }
    
    else if (state_heap->_next_state_index == state_heap->_size)
    {
        state_heap->_size *= 2;
        state_heap->states =
            realloc(state_heap->states, sizeof(state) * state_heap->_size);
    }

    state *state = &state_heap->states[state_heap->_next_state_index++];

    state->transitions.rows = 0;
    state->transitions.row_count = 0;
    state->transitions._length = 0;
    state->accept = 0;

    return state;
}

/**
 * This function builds our DFA from the given Pattern and stores all of it's
 * states into the pre-allocated StateBuffer. The first state in the
 * StateBuffer is the DFA's initial state.
 */
static state*
build_dfa(const char* pattern, state_heap* state_heap)
{
    state* last_appended_state;
    state* first_state;

    last_appended_state = new_state(state_heap);

    unsigned int state_count = 0;

    alphabet alphabet = { 0 };

    int pattern_length = strlen(pattern);
    for (int i = 0; i < pattern_length; i++)
    {
        switch (pattern[i])
        {
            // todo[joe] implement special character cases.
            case '+':
            {
                // note[joe] cause a crash if we don't have a previous state.
                assert(last_appended_state != 0);

                add_transition(
                    &last_appended_state->transitions,
                    pattern[i - 1],
                    last_appended_state);
            } break;

            default:
            {
                state* state = new_state(state_heap); state_count++;

                add_transition(
                    &last_appended_state->transitions,
                    pattern[i],
                    state);

                last_appended_state = state;

                append_glyph(&alphabet, pattern[i]);
            } break;
        }
    }

    // todo[joe] implement dfa generation algorithm.
    // it needs to traverse the nfa we've created and generate a dfa with
    // equivalent behavior.
    //
    // algorithm outline:
    // 1. create an nfa (done by code above).
    // 2. create a list of glyphs that are in our regex's alphabet.
    // 3. create a table (state-by-glyph), in which each cell represents the
    //    states that you can transition to when you are at a particular state
    //    and see a particular glpyh from the alphabet.
    // 4. for each list of states in a cell that is longer than 1 state,
    //    create a new state and give it all the unique transitions of the
    //    states found in the state list.
    //      4a. for each new state that is added, fill out the table. (this
    //          will have to be a recursive process, as we will surely run into
    //          situations where we have have to create a new state and a new
    //          state has to be handled, which may generate another new state
    //          to handle. and so-on and so-forth.)
    // 5. after no more new states can be created, we begin generation of the
    //    new fa. to do this, we start at the first row, and create a state
    //    from it. for each column, we add a state and a transition from the
    //    current state to the new state. we then visit each new state and
    //    repeat the process.
    
    state* state_table = (state*)
        malloc(sizeof(state) * alphabet.length * state_count);
    memset(state_table, 0, sizeof(state) * alphabet.length * state_count);
    
    // todo[joe] merge outer two loops.
    for (unsigned int i = 0; i < state_count; i++)
    {
        state s = state_heap->states[i];

        for (unsigned int j = 0; j < alphabet.length; j++)
        {
            char match = alphabet.glyphs[j];

            state new_state = state_table[i * alphabet.length + j];

            for (unsigned int k = 0; k < s.transitions.row_count; k++)
            {
                if (s.transitions.rows[k].match == match)
                {
                    state* next_state = s.transitions.rows[k].next_state;

                    for (unsigned int l = 0;
                         l < next_state->transitions.row_count;
                         l++)
                    {
                        add_transition(
                            &new_state.transitions,
                            next_state->transitions.rows[l].match,
                            next_state->transitions.rows[l].next_state);
                    }
                }
            }
        }
    }
    
    return &state_heap->states[0];
}

/**
 * This function traverses a DFA, given to us as a pointer to its initial
 * state. It explores the DFA until it finds states that have no exit, which it
 * considers the accept states of the DFA. Assuming that the DFA is well
 * formed (the DFA is not a forest), then this should be true.
 * 
 * FIXME[joe] This function produces a very crude approximation.
 */
static void
mark_accept_states(state* initial_state)
{
    unsigned int transition_count = initial_state->transitions.row_count;

    state** new_states = (state**) malloc(sizeof(state *) * transition_count);

    char *new_state_memory = (char *) new_states;
    for (unsigned int i = 0; i < sizeof(state *) * transition_count; i++)
        new_state_memory[i] = 0;

    unsigned int state_count = 0;

    // FIXME[joe] The transition count is insanely large for certain cases.
    // I don't know how it got to be so huge, but it's causing access
    // violations inside this loop.
    for (unsigned int i = 0; i < transition_count; i++)
    {
        if (initial_state->transitions.rows[i].next_state != initial_state)
        {
            new_states[state_count++] =
                initial_state->transitions.rows[i].next_state;
        }
    }

    // NOTE[joe] If TransitionCount == 0, we never zeroed NewStates.
    // A TransitionCount of 0 means that NewStates is of size 0, meaning that
    // we get memory, but can't zero it (since there technically _isn't_ any
    // memory we can legally access to do so). This means that
    // NewStates[0] == 0 will almost always fail, since the memory that is
    // there is garbage with no guarantees on it's probability of being all 0s.
    if (transition_count == 0 || new_states[0] == 0)
        initial_state->accept = 1;

    else
    {
        for (unsigned int i = 0; i < state_count; i++)
            mark_accept_states(new_states[i]);
    }

    free(new_states);
}

/**
 * This is the main function that our users will be using. It takes two
 * strings, a Pattern and a String to match. It compiles the Pattern into a
 * regex and then uses the regex it generated to check to see if there is a
 * substring in the given String that matches it.
 */
int match(const char* pattern, const char* string)
{
    state_heap state_heap = { 0 };
    state* automata = build_dfa(pattern, &state_heap);

    mark_accept_states(automata);

    state* current_state = automata;
    int    match_made = 0;

    int string_length = strlen(string);
    for (int i = 0; i < string_length; i++)
    {
        for (int j = 0; j < current_state->transitions.row_count; j++)
        {
            // NOTE[joe] Take the first matching transition.
            // This is because we still end up with non-deterministic
            // transitions in our automata.
            if (string[i] == current_state->transitions.rows[j].match)
            {
                current_state = current_state->transitions.rows[j].next_state;
                break;
            }
        }

        // TODO[joe] We need to be able to unset MatchMade.
        // Sometimes we end up in invalid states and we need to be able to say
        // that we failed to match.
        if (current_state->accept && match_made != 1)
            match_made = 1;
    }

    // FIXME[joe] The change in how we're storing states breaks this.
    // CleanupTransitionTables(Automata, MAX_STATE_BUFFER_SIZE);

    return match_made;
}

#endif
