#ifndef _VMC_PIPELINE_H_
#define _VMC_PIPELINE_H_

#include "arconf.h"
#include "arStringPool.h"
#include "arMemory.h"
#include "arMessage.h"
#include "arByteStream.h"
#include "arList.h"
#include "builder/types.h"
#include "builder/definition.h"
#include "builder/message_codes.h"

DECLARE_LIST_TYPE(vmp_list_packages, arB_package);
DECLARE_LIST_TYPE_FIND(vmp_list_packages, arB_package);

typedef struct arBuilder
{
	// All packages
	vmp_list_packages packages;

	// The total size, in bytes, that the header will take in the bytecode
	arInt32 total_header_size;

	// The total size in bytes that the bytecode for all functions will take
	arInt32 total_body_size;

	// A string pool that can be used to create strings
	arStringPool string_pool;

	// Messages raised during the build process
	arMessages messages;

	// If a panic error has occurred, such as if the computer is out of memory
	arMessage panic_error_message;

	// Stream where the actual data will be put
	arByteStream bytestream;
} arBuilder;

// Create a new virtual machine pipeline
ARLANG_API arBuilder* arBuilder_new();

// Destroy the supplied pipeline
ARLANG_API void arBuilder_destroy(arBuilder* p);

// Add the supplied package
ARLANG_API BOOL arBuilder_add_package(arBuilder* p, arB_package* pkg);

// Resolve all information, sizes and offsets for types, functions etc.
ARLANG_API BOOL arBuilder_resolve(arBuilder* p);

// Get a string
ARLANG_API const arString* arBuilder_get_string(arBuilder* p, const char* str, arInt32 len);

// Compile the builder content into bytecode that the virtual machine understands
ARLANG_API BOOL arBuilder_compile_package(arBuilder* b, const struct arB_package* p);

// Compile the builder content into bytecode that the virtual machine understands
ARLANG_API BOOL arBuilder_compile(arBuilder* b);

// Write bytecode to the builder
ARLANG_API BOOL arBuilder_write(arBuilder* builder, const void* ptr, arInt32 size);

// Reserve memory to the builder
ARLANG_API BOOL arBuilder_reserve(arBuilder* builder, arInt32 size);

// Fetch the bytecode generated by the builder. You are responsible for freeing the bytecode when you are done
ARLANG_API arByte* arBuilder_get_bytecode(arBuilder* b);

// Check to see if the builder has compiled successfully
inline static BOOL arBuilder_success(arBuilder* b)
{
	return arMessages_has_messages(&b->messages) == FALSE &&
		b->panic_error_message.code == 0;
}

#endif
