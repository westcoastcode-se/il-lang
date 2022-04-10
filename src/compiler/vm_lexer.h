#ifndef _VML_H_
#define _VML_H_

#include "../vm_string.h"

enum vml_type
{
	// Instructions
	VML_TYPE_INS,

	// Argument index
	VML_TYPE_ARGIDX,

	// Newline
	VML_TYPE_NEWLINE,

	// Comment
	VML_TYPE_COMMENT,

	// Integer value
	VML_TYPE_INT,

	// Decmial value
	VML_TYPE_DECIMAL,

	// Hex value
	VML_TYPE_HEX,

	// String value
	VML_TYPE_STRING,

	// Rune value
	VML_TYPE_RUNE,

	// Minus sign
	VML_TYPE_MINUS,

	// Plus sign
	VML_TYPE_PLUS,

	// Comma character, used to separate multiple arguments to an instruction
	VML_TYPE_COMMA,

	// [. Normally used as a way to tell the compiler to de-reference the address
	VML_TYPE_LSQUARE,

	// ]
	VML_TYPE_RSQUARE,

	// end-of-file
	VML_TYPE_EOF
};
typedef enum vml_type vml_type;

// Various modifiers that a type can have. For example, a string can be a single or a multi-line. An integer can be
// based on an integer, a binary or a hex value
enum vml_modifier
{
	vml_modifier_none
};
typedef enum vml_modifier vml_modifier;

// Represents a specific token
typedef struct
{
	vml_type type;
	vm_string string;
} vml_token;

// Message raised by the lexer, for example if an error has occurred
struct vml_message
{
	int code;

	// The line where the message originates
	int line;

	// The horizontal offset where the message originated
	int line_offset;

	// The offset on in the bytestream where the message originates
	int offset;
};

// Lexer
struct vmc_lexer
{
	const vm_byte* source;
};
typedef struct vmc_lexer vmc_lexer;

// Parse the supplied source code and return byte code as the result.
extern vmc_lexer* vmc_lexer_parse(const vm_byte* source);

// Delete the supplied lexer
extern void vmc_lexer_destroy(vmc_lexer* l);

// Get next token
extern void vmc_lexer_next(vmc_lexer* l, vml_token* token);

// Parse the next token and return TRUE if the supplied token is the supplied type
extern BOOL vmc_lexer_next_type(vmc_lexer* l, vml_type type, vml_token* token);

#endif
