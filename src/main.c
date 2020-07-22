/// C standard library includes
#include "stdio.h"

/// Third-party includes
#include "utest.h"

/// Internal includes
#include "regex.h"


/// Basic regex cases

UTEST(basic, a_matches_a)
{
    ASSERT_TRUE(match("a", "a"));
}

UTEST(basic, a_matches_aa)
{
    ASSERT_TRUE(match("a", "aa"));
}

UTEST(basic, a_matches_ab)
{
    ASSERT_TRUE(match("a", "ab"));
}

UTEST(basic, a_matches_ba)
{
    ASSERT_TRUE(match("a", "ba"));
}

UTEST(basic, a_matches_bac)
{
    ASSERT_TRUE(match("a", "bac"));
}

UTEST(basic, a_does_not_match_b)
{
    ASSERT_FALSE(match("a", "b"));
}

UTEST(basic, a_does_not_match_empty_string)
{
    ASSERT_FALSE(match("a", ""));
}

/// Basic '1 or more' cases.

UTEST(one_or_more, a_plus_matches_a_plus)
{
    ASSERT_TRUE(match("a+", "a+"));
}

UTEST(one_or_more, a_plus_matches_a)
{
    ASSERT_TRUE(match("a+", "a"));
}

UTEST(one_or_more, a_plus_matches_aa)
{
    ASSERT_TRUE(match("a+", "aa"));
}

UTEST(one_or_more, a_plus_matches_ab)
{
    ASSERT_TRUE(match("a+", "ab"));
}

UTEST(one_or_more, a_plus_matches_aab)
{
    ASSERT_TRUE(match("a+", "aab"));
}

UTEST(one_or_more, a_plus_does_not_match_empty_string)
{
    ASSERT_FALSE(match("a+", ""));
}

UTEST(one_or_more, a_plus_does_not_match_b)
{
    ASSERT_FALSE(match("a+", "b"));
}

/// Complex '1 or more' cases.

UTEST(one_or_more, a_plus_b_matches_ab)
{
    ASSERT_TRUE(match("a+b", "ab"));
}

UTEST(one_or_more, a_plus_b_matches_aab)
{
    ASSERT_TRUE(match("a+b", "aab"));
}

UTEST(one_or_more, a_plus_b_does_not_match_empty_string)
{
    ASSERT_TRUE(match("a+b", ""));
}

UTEST(one_or_more, a_plus_b_does_not_match_b)
{
    ASSERT_TRUE(match("a+b", "b"));
}

UTEST(one_or_more, a_plus_b_does_not_match_c)
{
    ASSERT_TRUE(match("a+b", "c"));
}


UTEST(one_or_more, a_plus_b_plus_matches_ab)
{
    ASSERT_TRUE(match("a+b+", "ab"));
}

UTEST(one_or_more, a_plus_b_plus_matches_aab)
{
    ASSERT_TRUE(match("a+b+", "aab"));
}

UTEST(one_or_more, a_plus_b_plus_matches_abb)
{
    ASSERT_TRUE(match("a+b+", "abb"));
}

UTEST(one_or_more, a_plus_b_plus_matches_aabb)
{
    ASSERT_TRUE(match("a+b+", "aabb"));
}

UTEST(one_or_more, a_plus_b_plus_does_not_match_empty_string)
{
    ASSERT_FALSE(match("a+b+", ""));
}

UTEST(one_or_more, a_plus_b_plus_does_not_match_a)
{
    ASSERT_FALSE(match("a+b+", "a"));
}

UTEST(one_or_more, a_plus_b_plus_does_not_match_b)
{
    ASSERT_FALSE(match("a+b+", "b"));
}

UTEST(one_or_more, a_plus_b_plus_does_not_match_c)
{
    ASSERT_FALSE(match("a+b+", "c"));
}

/// Failing tests
// FIXME: These are edgecases that cannot be handled.
// They can be fixed by creating a deterministic finite automata, but presently
// we are only able to create NFAs.

UTEST(failing, a_plus_a_matches_aa)
{
    ASSERT_TRUE(match("a+a", "aa"));
}

UTEST(failing, a_plus_a_matches_aaa)
{
    ASSERT_TRUE(match("a+a", "aaa"));
}

UTEST(failing, a_plus_a_matches_aab)
{
    ASSERT_TRUE(match("a+a", "aab"))
}

UTEST(failing, a_plus_a_does_not_match_a)
{
    ASSERT_FALSE(match("a+a", "a"));
}


UTEST_MAIN();
