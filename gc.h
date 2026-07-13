#ifndef GC_H
#define GC_H
#include <stddef.h>
#include "value.h"
typedef struct Runtime Runtime;
typedef struct Environment Environment;
void* gcAllocateObject(
    Runtime* runtime,
    size_t size,
    GcObjectType type
);
int gcPushRoot(Runtime* runtime, Value value);
void gcPopRoot(Runtime* runtime);
void gcMarkValue(Runtime* runtime, const Value* value);
void gcMarkEnvironment(Runtime* runtime, Environment* env);
void gcMarkRoots(Runtime* runtime);
void gcTraceReferences(Runtime* runtime);
void gcCollect(Runtime* runtime);
void gcSafePoint(Runtime* runtime);
void gcFreeAllObjects(Runtime* runtime);
#endif


