#include "common.h"
#include "grammar.h"

#include "lex.h"
#include "rdesc/util.h"
#include "rdesc/grammar.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>


int main(void)
{
	fprintf(stderr, "TESTING: grammar\n"); /* -------------------------- */
	struct rdesc_grammar grammar;

	unwrap(rdesc_grammar_init(&grammar,
			   PRODUCTION_COUNT,
			   MAX_ALTERNATIVE_COUNT,
			   MAX_ALTERNATIVE_SIZE,
			   (const struct rdesc_grammar_symbol *) grammar_def));

	rdesc_dump_bnf(stderr, &grammar, tk_names, nt_names);

	rdesc_grammar_destroy(&grammar);


	fprintf(stderr, "TESTING: lexer\n"); /* ---------------------------- */
	struct lex lex;
	uint16_t lex_out;

	lex_init(&lex, tk_names, 11);

	for (int i = 0; i < 10; i++) {
		assert(lex_pump(&lex, 't', NULL) == LEX_CONTINUE);
		lex_pump(&lex, 'r', NULL);
		lex_pump(&lex, 'u', NULL);
		assert(lex_pump(&lex, 'e', &lex_out) == LEX_READY);
		assert(lex_out == TK_TRUE);

		lex_pump(&lex, '\xC2', NULL);
		assert(lex_pump(&lex, '\xAC', &lex_out) == LEX_READY);
		assert(lex_out == TK_NEG);

		lex_pump(&lex, 'f', NULL);
		assert(lex_pump(&lex, 'o', NULL) == LEX_NOTOKEN);

		assert(lex_pump(&lex, 'x', NULL) == LEX_NOTOKEN);
	}

	lex_destroy(&lex);
}
