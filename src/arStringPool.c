#include "arStringPool.h"
#include "arMemory.h"

//
// PRIVATE ///////////////////////////////////////////////////////////////////////////
// 

void _arStringPool_copy(arString* s, const char* src, int len)
{
	char* dest = (char*)arMalloc(len);
	s->start = dest;
	s->end = dest + len;
	for (int i = 0; i < len; ++i) {
		*dest++ = *src++;
	}
}

//
// PUBLIC ///////////////////////////////////////////////////////////////////////////
// 

void arStringPool_init(arStringPool* p)
{
	p->first = p->last = NULL;
}

void arStringPool_destroy(arStringPool* p)
{
	arStringPoolEntry* e = p->first;
	while (e != NULL) {
		arStringPoolEntry* const next = e->next;
		arFree((void*)e->value.start);
		arFree(e);
		e = next;
	}
	p->first = p->last = NULL;
}

const arString* arStringPool_stringsz(arStringPool* p, const char* str, int len)
{
	// Try to find an existing string
	arStringPoolEntry* e = p->first;
	while (e != NULL) {
		if (arString_cmpsz(&e->value, str, len)) {
			return &e->value;
		}
		e = e->next;
	}

	// If not, then add it to the string pool
	e = (arStringPoolEntry*)arMalloc(sizeof(arStringPoolEntry));
	if (e == NULL) {
		return NULL;
	}
	e->next = NULL;
	_arStringPool_copy(&e->value, str, len);
	if (p->last != NULL) {
		p->last->next = e;
		e->index = p->last->index + 1;
		p->last = e;
	}
	else {
		e->index = 0;
		p->first = p->last = e;
	}
	return &e->value;
}

const arString* arStringPool_string(arStringPool* p, const arString* s)
{
	return arStringPool_stringsz(p, s->start, arString_length(s));
}
