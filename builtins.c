#include "builtins.h"
#include <string.h>
#include <stdio.h>

static Value builtinPrint(Runtime* runtime, int argCount, Value* args) {
    int i;

    (void)runtime;

    for (i = 0; i < argCount; i++) {
        if (i > 0) {
            printf(" ");
        }
        printValue(&args[i]);
    }

    printf("\n");
    return makeNone();
}

static Value builtinLen(Runtime* runtime, int argCount, Value* args) {
    (void)runtime;

    if (argCount != 1) {
        return makeNone();
    }

    if (args[0].type == VAL_LIST) {
        if (args[0].as.list == NULL) {
            return makeNumber(0);
        }

        return makeNumber((double)args[0].as.list->count);
    }

    if (args[0].type == VAL_STRING) {
        if (args[0].as.string == NULL) {
            return makeNumber(0);
        }

        return makeNumber((double)strlen(args[0].as.string));
    }

    return makeNone();
}

void registerBuiltins(Environment* globals) {
    static NativeFunction printFunction = { "print", -1, builtinPrint };
    static NativeFunction lenFunction = { "len", 1, builtinLen };

    envDefine(globals, "true", makeBool(1));
    envDefine(globals, "false", makeBool(0));
    envDefine(globals, "none", makeNone());
    envDefine(globals, "print", makeNativeFunction(&printFunction));
    envDefine(globals, "len", makeNativeFunction(&lenFunction));
}