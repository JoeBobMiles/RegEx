/*
    @file       fa.h
    @author     Joseph Miles <josephmiles2015@gmail.com>
    @date       2019-02-03
    @summary    Declares all finite automata specific functions and data
                structures.
 */

#ifndef __FA_H__
#define __FA_H__

typedef struct state {
    char            Match;
    struct state   *NextState;
    int             Accept;
} state;

typedef struct {
    state *InitialState;
} fa;

#endif
