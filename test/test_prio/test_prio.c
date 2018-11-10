#include "../test.h"
#include "../helpers.h"


static int test_prio(void)
{
    test_start("prio");

    cleri_grammar_t * grammar;
    cleri_t * k_hi, * k_bye, * k_and, * k_or, * t_open, * t_close, * prio;

    k_hi = cleri_keyword(0, "hi", false);
    k_bye = cleri_keyword(0, "bye", false);
    k_and = cleri_keyword(0, "and", false);
    k_or = cleri_keyword(0, "or", false);
    t_open = cleri_token(0, "(");
    t_close = cleri_token(0, ")");

    prio = cleri_prio(
        0,
        5,
        k_hi,
        k_bye,
        cleri_sequence(0, 3, t_open, CLERI_THIS, t_close),
        cleri_sequence(0, 3, CLERI_THIS, k_and, CLERI_THIS),
        cleri_sequence(0, 3, CLERI_THIS, k_or, CLERI_THIS));

    grammar = cleri_grammar(prio, NULL);

    // assert statements
    _assert_is_valid(grammar, "hi");
    _assert_is_valid(grammar, "(bye)");
    _assert_is_valid(grammar, "(hi and bye)");
    _assert_is_valid(grammar, "(hi or hi) and (hi or hi)");
    _assert_is_valid(grammar, "(hi or (hi and bye))");
    _assert_is_not_valid(grammar, "");
    _assert_is_not_valid(grammar, "(hi");
    _assert_is_not_valid(grammar, "()");
    _assert_is_not_valid(grammar, "(hi or hi) and");
    {
        cleri_parse_t * pr = cleri_parse(
            grammar,
            "(((((((((((((((((((((((((((((((((((((((((((((((((((hi"
            ")))))))))))))))))))))))))))))))))))))))))))))))))))"
        );
        _assert (pr == NULL);  /* max recursion depth */

    }

    _assert_parse_str (
        grammar,
        "(((((((((((((((((((((((((((((((((((((((((((((((((((hi"
        ")))))))))))))))))))))))))))))))))))))))))))))))))))",
        "no parse result, a possible reason might be that the maximum "
        "recursion depth of 50 has been reached",
        NULL);
    _assert_parse_str2 (
        grammar,
        "(((((((((((((((((((((((((((((((((((((((((((((((((((hi"
        ")))))))))))))))))))))))))))))))))))))))))))))))))))",
        "no parse result, a possible reason might be that the maximum "
        "recursion depth of 50 has been reached",
        NULL);
    cleri_grammar_free(grammar);

    return test_end();
}

int main()
{
    return (
        test_prio() ||
        0
    );
}
