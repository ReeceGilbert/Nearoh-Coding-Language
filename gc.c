#include "gc.h"
#include "runtime.h"
#include <stdlib.h>
#define GRAY_INITIAL_CAPACITY 8
#define ROOT_INITIAL_CAPACITY 8
static int ensureGrayCapacity(Runtime* runtime);
static int ensureRootCapacity(Runtime* runtime);
static void markObject(Runtime* runtime, GcObject* object);
static void markEnvironmentContents(Runtime* runtime, Environment* env);
static void blackenObject(Runtime* runtime, GcObject* object);
static void freeGcObject(GcObject* object);
static int ensureGrayCapacity(Runtime* runtime) {
    GcObject** newStack;
    int newCapacity;
    if (runtime == NULL) {
        return 0;
    }
    if (runtime->grayCount < runtime->grayCapacity) {
        return 1;
    }
    newCapacity = runtime->grayCapacity < GRAY_INITIAL_CAPACITY
        ? GRAY_INITIAL_CAPACITY
        : runtime->grayCapacity * 2;
    newStack = (GcObject**)realloc(
        runtime->grayStack,
        sizeof(GcObject*) * (size_t)newCapacity
    );
    if (newStack == NULL) {
        return 0;
    }
    runtime->grayStack = newStack;
    runtime->grayCapacity = newCapacity;
    return 1;
}
static int ensureRootCapacity(Runtime* runtime) {
    Value* newRoots;
    int newCapacity;
    if (runtime == NULL) {
        return 0;
    }
    if (runtime->tempRootCount < runtime->tempRootCapacity) {
        return 1;
    }
    newCapacity = runtime->tempRootCapacity < ROOT_INITIAL_CAPACITY
        ? ROOT_INITIAL_CAPACITY
        : runtime->tempRootCapacity * 2;
    newRoots = (Value*)realloc(
        runtime->tempRoots,
        sizeof(Value) * (size_t)newCapacity
    );
    if (newRoots == NULL) {
        return 0;
    }
    runtime->tempRoots = newRoots;
    runtime->tempRootCapacity = newCapacity;
    return 1;
}
void* gcAllocateObject(
    Runtime* runtime,
    size_t size,
    GcObjectType type
) {
    GcObject* object;
    if (runtime == NULL || size < sizeof(GcObject)) {
        return NULL;
    }
    object = (GcObject*)malloc(size);
    if (object == NULL) {
        return NULL;
    }
    object->type = type;
    object->marked = 0;
    object->size = size;
    object->next = runtime->objects;
    runtime->objects = object;
    runtime->bytesAllocated += size;
    return object;
}
int gcPushRoot(Runtime* runtime, Value value) {
    if (runtime == NULL) {
        return 0;
    }
    if (!ensureRootCapacity(runtime)) {
        return 0;
    }
    runtime->tempRoots[runtime->tempRootCount] = value;
    runtime->tempRootCount++;
    return 1;
}
void gcPopRoot(Runtime* runtime) {
    if (runtime == NULL || runtime->tempRootCount <= 0) {
        return;
    }
    runtime->tempRootCount--;
    runtime->tempRoots[runtime->tempRootCount] = makeNone();
}
static void markObject(Runtime* runtime, GcObject* object) {
    if (runtime == NULL || object == NULL || object->marked) {
        return;
    }
    object->marked = 1;
    if (!ensureGrayCapacity(runtime)) {
        runtimeError(runtime, "Out of memory while growing GC gray stack.");
        return;
    }
    runtime->grayStack[runtime->grayCount] = object;
    runtime->grayCount++;
}
void gcMarkValue(Runtime* runtime, const Value* value) {
    if (runtime == NULL || value == NULL) {
        return;
    }
    switch (value->type) {
        case VAL_FUNCTION:
            markObject(runtime, (GcObject*)value->as.function);
            break;
        case VAL_CLASS:
            markObject(runtime, (GcObject*)value->as.classObject);
            break;
        case VAL_INSTANCE:
            markObject(runtime, (GcObject*)value->as.instance);
            break;
        case VAL_BOUND_METHOD:
            markObject(runtime, (GcObject*)value->as.boundMethod);
            break;
        case VAL_LIST:
            markObject(runtime, (GcObject*)value->as.list);
            break;
        case VAL_DICT:
            markObject(runtime, (GcObject*)value->as.dict);
            break;
        default:
            break;
    }
}
static void markEnvironmentContents(Runtime* runtime, Environment* env) {
    int i;
    if (runtime == NULL || env == NULL) {
        return;
    }
    for (i = 0; i < env->count; i++) {
        gcMarkValue(runtime, &env->bindings[i].value);
    }
    gcMarkEnvironment(runtime, env->parent);
}
void gcMarkEnvironment(Runtime* runtime, Environment* env) {
    if (runtime == NULL || env == NULL) {
        return;
    }
    /*
        globals is embedded directly inside Runtime and is not part of
        runtime->objects, so mark its contents without treating it as a
        heap-allocated GcObject.
    */
    if (env == &runtime->globals) {
        markEnvironmentContents(runtime, env);
        return;
    }
    markObject(runtime, (GcObject*)env);
}
void gcMarkRoots(Runtime* runtime) {
    int i;
    if (runtime == NULL) {
        return;
    }
    markEnvironmentContents(runtime, &runtime->globals);
    gcMarkEnvironment(runtime, runtime->current);
    for (i = 0; i < runtime->tempRootCount; i++) {
        gcMarkValue(runtime, &runtime->tempRoots[i]);
    }
}
static void blackenObject(Runtime* runtime, GcObject* object) {
    int i;
    if (runtime == NULL || object == NULL) {
        return;
    }
    switch (object->type) {
        case GC_FUNCTION: {
            FunctionObject* function = (FunctionObject*)object;
            gcMarkEnvironment(runtime, function->closure);
            break;
        }
        case GC_CLASS: {
            ClassObject* classObject = (ClassObject*)object;
            gcMarkEnvironment(runtime, classObject->closure);
            for (i = 0; i < classObject->methodCount; i++) {
                markObject(
                    runtime,
                    (GcObject*)classObject->methods[i].function
                );
            }
            break;
        }
        case GC_INSTANCE: {
            InstanceObject* instance = (InstanceObject*)object;
            markObject(runtime, (GcObject*)instance->classObject);
            for (i = 0; i < instance->fieldCount; i++) {
                gcMarkValue(runtime, &instance->fields[i].value);
            }
            break;
        }
        case GC_BOUND_METHOD: {
            BoundMethodObject* boundMethod = (BoundMethodObject*)object;
            markObject(runtime, (GcObject*)boundMethod->receiver);
            markObject(runtime, (GcObject*)boundMethod->method);
            break;
        }
        case GC_LIST: {
            ListObject* list = (ListObject*)object;
            for (i = 0; i < list->count; i++) {
                gcMarkValue(runtime, &list->items[i]);
            }
            break;
        }
        case GC_DICT: {
            DictObject* dict = (DictObject*)object;
            for (i = 0; i < dict->count; i++) {
                gcMarkValue(runtime, &dict->entries[i].key);
                gcMarkValue(runtime, &dict->entries[i].value);
            }
            break;
        }
        case GC_ENVIRONMENT: {
            Environment* env = (Environment*)object;
            markEnvironmentContents(runtime, env);
            break;
        }
        default:
            break;
    }
}
void gcTraceReferences(Runtime* runtime) {
    GcObject* object;
    if (runtime == NULL) {
        return;
    }
    while (runtime->grayCount > 0) {
        runtime->grayCount--;
        object = runtime->grayStack[runtime->grayCount];
        blackenObject(runtime, object);
        if (runtime->hadError) {
            return;
        }
    }
}
static void sweep(Runtime* runtime) {
    GcObject* previous;
    GcObject* object;
    if (runtime == NULL) {
        return;
    }
    previous = NULL;
    object = runtime->objects;
    while (object != NULL) {
        if (object->marked) {
            object->marked = 0;
            previous = object;
            object = object->next;
        } else {
            GcObject* unreached = object;
            object = object->next;
            if (previous != NULL) {
                previous->next = object;
            } else {
                runtime->objects = object;
            }
            if (runtime->bytesAllocated >= unreached->size) {
                runtime->bytesAllocated -= unreached->size;
            } else {
                runtime->bytesAllocated = 0;
            }
            freeGcObject(unreached);
        }
    }
}
void gcCollect(Runtime* runtime) {
    size_t minimumNextGc;
    if (runtime == NULL) {
        return;
    }
    runtime->grayCount = 0;
    gcMarkRoots(runtime);
    gcTraceReferences(runtime);
    if (runtime->hadError) {
        return;
    }
    sweep(runtime);
    minimumNextGc = 16 * 1024;
    if (runtime->bytesAllocated > minimumNextGc / 2) {
        runtime->nextGc = runtime->bytesAllocated * 2;
    } else {
        runtime->nextGc = minimumNextGc;
    }
}void gcSafePoint(Runtime* runtime) {
    if (runtime == NULL) {
        return;
    }
#ifdef NEAROH_GC_STRESS
    gcCollect(runtime);
#else
    if (runtime->bytesAllocated >= runtime->nextGc) {
        gcCollect(runtime);
    }
#endif
}static void freeGcObject(GcObject* object) {
    int i;
    if (object == NULL) {
        return;
    }
    switch (object->type) {
        case GC_FUNCTION: {
            FunctionObject* function = (FunctionObject*)object;
            free(function->name);
            free(function->params);
            break;
        }
        case GC_CLASS: {
            ClassObject* classObject = (ClassObject*)object;
            free(classObject->name);
            for (i = 0; i < classObject->methodCount; i++) {
                free(classObject->methods[i].name);
            }
            free(classObject->methods);
            break;
        }
        case GC_INSTANCE: {
            InstanceObject* instance = (InstanceObject*)object;
            for (i = 0; i < instance->fieldCount; i++) {
                free(instance->fields[i].name);
                freeValue(&instance->fields[i].value);
            }
            free(instance->fields);
            break;
        }
        case GC_BOUND_METHOD:
            break;
        case GC_LIST: {
            ListObject* list = (ListObject*)object;
            for (i = 0; i < list->count; i++) {
                freeValue(&list->items[i]);
            }
            free(list->items);
            break;
        }
        case GC_DICT: {
            DictObject* dict = (DictObject*)object;
            for (i = 0; i < dict->count; i++) {
                freeValue(&dict->entries[i].key);
                freeValue(&dict->entries[i].value);
            }
            free(dict->entries);
            break;
        }
        case GC_ENVIRONMENT: {
            Environment* env = (Environment*)object;
            for (i = 0; i < env->count; i++) {
                free(env->bindings[i].name);
                freeValue(&env->bindings[i].value);
            }
            free(env->bindings);
            break;
        }
        default:
            break;
    }
    free(object);
}
void gcFreeAllObjects(Runtime* runtime) {
    GcObject* object;
    if (runtime == NULL) {
        return;
    }
    object = runtime->objects;
    while (object != NULL) {
        GcObject* next = object->next;
        freeGcObject(object);
        object = next;
    }
    runtime->objects = NULL;
    runtime->bytesAllocated = 0;
}



