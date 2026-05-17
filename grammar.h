#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "rdesc/grammar.h"


#define NONTERMINAL_COUNT 11
#define MAX_ALTERNATIVE_COUNT 3
#define MAX_ALTERNATIVE_SIZE 3

#define TOKEN_COUNT 10


enum nt {
	NT_EXPR,

	NT_ATOM,
	NT_NEG_EXPR,
	NT_AND_EXPR, NT_AND_EXPR_REST,
	NT_OR_EXPR, NT_OR_EXPR_REST,
	NT_IF_EXPR, NT_IF_EXPR_REST,
	NT_IFF_EXPR, NT_IFF_EXPR_REST,
};

enum tk {
	TK_TRUE, TK_FALSE,
	TK_NEG, TK_AND, TK_OR, TK_IF, TK_IFF,

	TK_LPAREN, TK_RPAREN,

	TK_ENDSYM
};

extern const char *tk_names[];
extern const char *nt_names[];

/* +1s in alternative count and body length are for sentinels */
extern struct rdesc_grammar_symbol grammar_def[NONTERMINAL_COUNT]
					      [MAX_ALTERNATIVE_COUNT + 1]
					      [MAX_ALTERNATIVE_SIZE + 1];


#endif
