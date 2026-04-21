#include "builtins.h"

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

void registerBuiltins(Environment* globals) {
    static NativeFunction printFunction = { "print", -1, builtinPrint };

    envDefine(globals, "true", makeBool(1));
    envDefine(globals, "false", makeBool(0));
    envDefine(globals, "none", makeNone());
    envDefine(globals, "print", makeNativeFunction(&printFunction));
}