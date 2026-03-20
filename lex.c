#include "lex.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


void lex_init(struct lex *l, const char *tk_names[], uint16_t tk_count)
{
	l->tk_names = tk_names;
	l->tk_count = tk_count;

	l->max_tk_len = 0;

	assert(l->tk_lens = malloc(sizeof(size_t) * l->tk_count));

	for (uint16_t i = 0; i < tk_count; i++) {
		size_t tk_len = strlen(tk_names[i]);
		l->tk_lens[i] = tk_len;

		if (tk_len > l->max_tk_len)
			l->max_tk_len = tk_len;
	}

	assert(l->collected_token = malloc(l->max_tk_len + 1));

	l->collected_token[l->max_tk_len] = '\0';
	l->collected_token_len = 0;
}

enum lex_result lex_pump(struct lex *l, char c, uint16_t *out)
{
	uint16_t failed_count = 0;

	if (isspace(c))
		return LEX_CONTINUE;

	l->collected_token[l->collected_token_len++] = c;
	l->collected_token[l->collected_token_len] = '\0';

	for (uint16_t i = 0; i < l->tk_count; i++) {
		if (l->collected_token_len > l->tk_lens[i]) {
			failed_count++;

			continue;
		}

		bool failed = false;
		for (size_t j = 0; j < l->collected_token_len; j++) {
			if (l->tk_names[i][j] != l->collected_token[j]) {
				failed = true;

				break;
			}
		}

		if (failed) {
			failed_count++;
		} else if (l->tk_lens[i] == l->collected_token_len) {
			*out = i;

			l->collected_token_len = 0;

			return LEX_READY;
		}
	}

	if (l->collected_token_len == l->max_tk_len ||
	    failed_count == l->tk_count) {
		l->collected_token_len = 0;

		return LEX_NOTOKEN;
	}

	return LEX_CONTINUE;
}

void lex_destroy(struct lex *l)
{
	free(l->collected_token);
}
