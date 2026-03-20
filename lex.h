#ifndef LEX_H
#define LEX_H


#include <stddef.h>
#include <stdint.h>


struct lex {
	const char **tk_names;
	size_t *tk_lens;
	uint16_t tk_count;

	size_t max_tk_len;

	size_t collected_token_len;
	char *collected_token;
};

enum lex_result {
	LEX_CONTINUE  /**< Continue to pumping new chars. */,
	LEX_READY  /**< A token is ready for consumption. */,
	LEX_NOTOKEN  /**< Input string could not recognized. */,
};


/** @brief Initializes an empty lexer. */
void lex_init(struct lex *l, const char *tk_names[], uint16_t tk_count);

/**
 * @brief Pumps a char into the lexer.
 *
 * Returns token from out variable, if return code is LEX_READY.
 */
enum lex_result lex_pump(struct lex *l, char c, uint16_t *out);

/** @brief Destroy and deallocate memory allocated by the lexer. */
void lex_destroy(struct lex *l);


#endif
