#include "grammar.h"

#include "rdesc/grammar.h"
#include "rdesc/rule_macros.h"


struct rdesc_grammar_symbol grammar_def[PRODUCTION_COUNT]
				       [MAX_ALTERNATIVE_COUNT + 1]
				       [MAX_ALTERNATIVE_SIZE + 1] = {
	/* <expr> ::= */ r(
		NT(IFF_EXPR), TK(ENDSYM)
	),

	/* <atom> ::= */ r(
		TK(LPAREN), NT(IFF_EXPR), TK(RPAREN)
	alt	TK(TRUE)
	alt	TK(FALSE)
	),

	/* <neg_expr> ::= */ r(
		TK(NEG), NT(ATOM)
	alt	NT(ATOM)
	),

	/* <and_expr> ::= */
		rrr(AND_EXPR, (NT(NEG_EXPR)), (TK(AND), NT(NEG_EXPR))),

	/* <or_expr> ::= */
		rrr(OR_EXPR, (NT(AND_EXPR)), (TK(OR), NT(AND_EXPR))),

	/* <if_expr> ::= */
		rrr(IF_EXPR, (NT(OR_EXPR)), (TK(IF), NT(OR_EXPR))),

	/* <iff_expr> ::= */
		rrr(IFF_EXPR, (NT(IF_EXPR)), (TK(IFF), NT(IF_EXPR)))
};

const char *tk_names[] = {
	"\0",
	"true", "false",
	"¬", "∧", "∨", "→", "↔",
	"(", ")",
	";"
};

const char *nt_names[] = {
	"expr",

	"atom",
	"neg_expr",
	"and_expr", "and_expr_rest",
	"or_expr", "or_expr_rest",
	"if_expr", "if_expr_rest",
	"iff_expr", "iff_expr_rest",
};
