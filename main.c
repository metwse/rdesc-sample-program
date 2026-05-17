#include "common.h"
#include "grammar.h"
#include "lex.h"

#include "rdesc/rdesc.h"
#include "rdesc/cst_macros.h"
#include "rdesc/grammar.h"
#include "rdesc/util.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


bool interpret(struct rdesc_node n)
{
	switch (rid(n)) {
	case NT_EXPR:
		rdesc_flip_left(n, 0);  /** flip iff expr */

		return interpret(rchild(n, 0));

	case NT_ATOM:
		switch (ralt_idx(n)) {
		case 0:
			rdesc_flip_left(n, 1);  /** flip iff expr */

			return interpret(rchild(n, 1));
		case 1:
			return true;
		default:
			return false;
		}

	case NT_NEG_EXPR:
		switch (ralt_idx(n)) {
		case 0:
			return !interpret(rchild(n, 1));
		default:
			return interpret(rchild(n, 0));
		}

	case NT_AND_EXPR:
		switch (ralt_idx(n)) {
		case 0:
			return interpret(rchild(n, 0)) && interpret(rchild(n, 2));
		default:
			return interpret(rchild(n, 0));
		}

	case NT_OR_EXPR:
		switch (ralt_idx(n)) {
		case 0:
			rdesc_flip_left(n, 2);  /** flip and expr */

			return interpret(rchild(n, 0)) || interpret(rchild(n, 2));
		default:
			rdesc_flip_left(n, 0);  /** flip and expr */

			return interpret(rchild(n, 0));
		}

	case NT_IF_EXPR:
		rdesc_flip_left(n, 0);  /** flip or expr */

		return ralt_idx(rchild(n, 1)) == 0 ?
			!interpret(rchild(n, 0)) || interpret(rchild(n, 1)) :
			interpret(rchild(n, 0));

	case NT_IF_EXPR_REST:
		switch (ralt_idx(n)) {
		case 0:
			rdesc_flip_left(n, 1);  /** flip or expr */

			return ralt_idx(rchild(n, 2)) == 0 ?
				!interpret(rchild(n, 1)) || interpret(rchild(n, 2)) :
				interpret(rchild(n, 1));
		default:
			assert(0 && "unreachable");
		}

	case NT_IFF_EXPR:
		switch (ralt_idx(n)) {
		case 0: {
			bool a = interpret(rchild(n, 0));
			bool b = interpret(rchild(n, 2));

			return (a && b) || (!a && !b);
		} default:
			return interpret(rchild(n, 0));
		}
	}

	abort();  // GCOVR_EXCL_LINE: unreachable
}

void program(struct rdesc *p)
{
	struct lex l;

	lex_init(&l, tk_names, TOKEN_COUNT);

	char c;

	unwrap(rdesc_start(p, NT_EXPR));

	while ((c = getchar()) != EOF) {
		uint16_t tk_id;
		enum lex_result lex_res = lex_pump(&l, c, &tk_id);

		switch (lex_res) {
		case LEX_CONTINUE:
			continue;

		case LEX_NOTOKEN:
			fprintf(stderr, "Read invalid token!\n");

			continue;

		default: break;
		}

		enum rdesc_result res = rdesc_pump(p, tk_id, NULL);

		assert(res != RDESC_ENOMEM && "Memory error");

		switch (res) {
		case RDESC_CONTINUE:
			continue;

		case RDESC_NOMATCH:
			fprintf(stderr, "Syntax error!\n");

			rdesc_reset(p);
			unwrap(rdesc_start(p, NT_EXPR));

			continue;

		case RDESC_READY:
			printf("(%s)\n", interpret(rdesc_get_root(p))
					  ? "true" : "false");
			unwrap(rdesc_start(p, NT_EXPR));

			break;

		default: abort();  // GCOVR_EXCL_LINE: unreachable
		}
	}
}


int main(void)
{
	struct rdesc_grammar grammar;
	struct rdesc p;

	unwrap(rdesc_grammar_init(&grammar,
			   NONTERMINAL_COUNT,
			   MAX_ALTERNATIVE_COUNT,
			   MAX_ALTERNATIVE_SIZE,
			   (const struct rdesc_grammar_symbol *) grammar_def));

	unwrap(rdesc_init(&p, &grammar, 0, NULL));

	program(&p);

	rdesc_destroy(&p);

	rdesc_grammar_destroy(&grammar);
}
