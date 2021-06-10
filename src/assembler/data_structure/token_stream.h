#ifndef TOKENSTREAM
#define TOKENSTREAM

// TODO : complete this
typedef struct token_t {
	int tag;
	char ** oprands;
} token_t;

typedef struct token_node_t {
	token_t data;
	token_node_t * next;
	token_node_t * prev;
} token_node_t;

typedef struct token_stream_t {
	token_node_t * head_holder;
	token_node_t * tail_holder;
} token_stream_t;

typedef token_stream_t *TokenStream;

#endif //TOKENSTREAM

