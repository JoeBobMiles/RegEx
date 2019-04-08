/**
 * @file main.c
 * @author Joseph Miles <josephmiles2015@gmail.com>
 * @date 2019-02-03
 *
 * Main driver for the RegEx project. This is used to test the RegEx library.
 */

// C standard library includes
#include <assert.h>

// Internal includes
#include "../include/regex.h"

int main(void)
{
    /*
    Basic '1 or more' cases.
     */
    assert(Match("a+", "a+") == 1);
    assert(Match("a+", "a") == 1);
    assert(Match("a+", "aa") == 1);
    assert(Match("a+", "") == 0);
    assert(Match("a+", "b") == 0);

    /*
    More complex cases of '1 or more'.
     */
    assert(Match("a+b", "ab") == 1);
    assert(Match("a+b", "aab") == 1);
    assert(Match("a+b", "") == 0);
    assert(Match("a+b", "b") == 0);
    assert(Match("a+b", "c") == 0);

    assert(Match("a+b+", "ab") == 1);
    assert(Match("a+b+", "aab") == 1);
    assert(Match("a+b+", "abb") == 1);
    assert(Match("a+b+", "aabb") == 1);
    assert(Match("a+b+", "") == 0);
    assert(Match("a+b+", "a") == 0);
    assert(Match("a+b+", "b") == 0);
    assert(Match("a+b+", "c") == 0);

    return 0;
}
