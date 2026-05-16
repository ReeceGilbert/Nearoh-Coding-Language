#include "builtins.h"

#include <stdio.h>
#include <string.h>

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
    if (argCount != 1) {
        runtimeError(runtime, "len() expects exactly 1 argument.");
        return makeNone();
    }

    switch (args[0].type) {
        case VAL_LIST:
            return makeNumber(args[0].as.list == NULL ? 0 : (double)args[0].as.list->count);

        case VAL_DICT:
            return makeNumber(args[0].as.dict == NULL ? 0 : (double)args[0].as.dict->count);

        case VAL_STRING:
            return makeNumber(args[0].as.string == NULL ? 0 : (double)strlen(args[0].as.string));

        default:
            runtimeError(runtime, "len() expects a list, dictionary, or string.");
            return makeNone();
    }
}

static Value builtinType(Runtime* runtime, int argCount, Value* args) {
    if (argCount != 1) {
        runtimeError(runtime, "type() expects exactly 1 argument.");
        return makeNone();
    }

    return makeStringCopy(valueTypeName(&args[0]));
}

static Value builtinAppend(Runtime* runtime, int argCount, Value* args) {
    ListObject* result;
    int i;

    if (argCount != 2) {
        runtimeError(runtime, "append() expects exactly 2 arguments.");
        return makeNone();
    }

    if (args[0].type != VAL_LIST) {
        runtimeError(runtime, "append() expects the first argument to be a list.");
        return makeNone();
    }

    if (args[0].as.list == NULL) {
        runtimeError(runtime, "append() cannot append to a null list.");
        return makeNone();
    }

    result = createListObject();
    if (result == NULL) {
        runtimeError(runtime, "Out of memory while creating appended list.");
        return makeNone();
    }

    for (i = 0; i < args[0].as.list->count; i++) {
        if (!listAppend(result, args[0].as.list->items[i])) {
            freeValue(&(Value){ .type = VAL_LIST, .as.list = result });
            runtimeError(runtime, "Out of memory while copying list for append().");
            return makeNone();
        }
    }

    if (!listAppend(result, args[1])) {
        freeValue(&(Value){ .type = VAL_LIST, .as.list = result });
        runtimeError(runtime, "Out of memory while appending list item.");
        return makeNone();
    }

    return makeList(result);
}

static Value builtinRange(Runtime* runtime, int argCount, Value* args) {
    ListObject* result;
    int count;
    int i;

    if (argCount != 1) {
        runtimeError(runtime, "range() expects exactly 1 argument.");
        return makeNone();
    }

    if (args[0].type != VAL_NUMBER) {
        runtimeError(runtime, "range() expects a number.");
        return makeNone();
    }

    count = (int)args[0].as.number;

    if (count < 0) {
        runtimeError(runtime, "range() count cannot be negative.");
        return makeNone();
    }

    result = createListObject();
    if (result == NULL) {
        runtimeError(runtime, "Out of memory while creating range list.");
        return makeNone();
    }

    for (i = 0; i < count; i++) {
        Value number = makeNumber((double)i);

        if (!listAppend(result, number)) {
            freeValue(&(Value){ .type = VAL_LIST, .as.list = result });
            runtimeError(runtime, "Out of memory while building range list.");
            return makeNone();
        }
    }

    return makeList(result);
}

static Value builtinKeys(Runtime* runtime, int argCount, Value* args) {
    ListObject* result;
    int i;

    if (argCount != 1) {
        runtimeError(runtime, "keys() expects exactly 1 argument.");
        return makeNone();
    }

    if (args[0].type != VAL_DICT) {
        runtimeError(runtime, "keys() expects a dictionary.");
        return makeNone();
    }

    if (args[0].as.dict == NULL) {
        runtimeError(runtime, "keys() cannot read a null dictionary.");
        return makeNone();
    }

    result = createListObject();
    if (result == NULL) {
        runtimeError(runtime, "Out of memory while creating keys list.");
        return makeNone();
    }

    for (i = 0; i < args[0].as.dict->count; i++) {
        if (!listAppend(result, args[0].as.dict->entries[i].key)) {
            freeValue(&(Value){ .type = VAL_LIST, .as.list = result });
            runtimeError(runtime, "Out of memory while building keys list.");
            return makeNone();
        }
    }

    return makeList(result);
}

static Value builtinHas(Runtime* runtime, int argCount, Value* args) {
    Value found;

    if (argCount != 2) {
        runtimeError(runtime, "has() expects exactly 2 arguments.");
        return makeNone();
    }

    if (args[0].type != VAL_DICT) {
        runtimeError(runtime, "has() expects the first argument to be a dictionary.");
        return makeNone();
    }

    if (args[0].as.dict == NULL) {
        return makeBool(0);
    }

    if (dictGet(args[0].as.dict, &args[1], &found)) {
        freeValue(&found);
        return makeBool(1);
    }

    return makeBool(0);
}

void registerBuiltins(Environment* globals) {
    static NativeFunction printFunction = {
        "print",
        -1,
        builtinPrint
    };

    static NativeFunction lenFunction = {
        "len",
        1,
        builtinLen
    };

    static NativeFunction typeFunction = {
        "type",
        1,
        builtinType
    };

    static NativeFunction appendFunction = {
        "append",
        2,
        builtinAppend
    };

    static NativeFunction rangeFunction = {
        "range",
        1,
        builtinRange
    };

    static NativeFunction keysFunction = {
        "keys",
        1,
        builtinKeys
    };

    static NativeFunction hasFunction = {
        "has",
        2,
        builtinHas
    };

    envDefine(globals, "true", makeBool(1));
    envDefine(globals, "false", makeBool(0));
    envDefine(globals, "none", makeNone());

    envDefine(globals, "print", makeNativeFunction(&printFunction));
    envDefine(globals, "len", makeNativeFunction(&lenFunction));
    envDefine(globals, "type", makeNativeFunction(&typeFunction));
    envDefine(globals, "append", makeNativeFunction(&appendFunction));
    envDefine(globals, "range", makeNativeFunction(&rangeFunction));
    envDefine(globals, "keys", makeNativeFunction(&keysFunction));
    envDefine(globals, "has", makeNativeFunction(&hasFunction));
}