#ifndef VALUE_H
#define VALUE_H

#include "token.h"

typedef struct AstNode AstNode;
typedef struct Environment Environment;
typedef struct Runtime Runtime;

typedef struct Value Value;
typedef struct NativeFunction NativeFunction;
typedef struct FunctionObject FunctionObject;
typedef struct ClassObject ClassObject;
typedef struct InstanceField InstanceField;
typedef struct InstanceObject InstanceObject;

struct FunctionObject {
    char* name;
    Token* params;
    int paramCount;
    AstNode* body;
    Environment* closure;
};

struct ClassObject {
    char* name;
    AstNode* body;
    Environment* closure;
};

typedef enum {
    VAL_NONE,
    VAL_BOOL,
    VAL_NUMBER,
    VAL_STRING,
    VAL_NATIVE_FUNCTION,
    VAL_FUNCTION,
    VAL_CLASS,
    VAL_INSTANCE
} ValueType;

struct Value {
    ValueType type;
    union {
        int boolean;
        double number;
        char* string;
        NativeFunction* nativeFunction;
        FunctionObject* function;
        ClassObject* classObject;
        InstanceObject* instance;
    } as;
};

struct NativeFunction {
    const char* name;
    int arity;
    Value (*fn)(Runtime* runtime, int argCount, Value* args);
};

struct InstanceField {
    char* name;
    Value value;
};

struct InstanceObject {
    ClassObject* classObject;
    InstanceField* fields;
    int fieldCount;
    int fieldCapacity;
};

Value makeNone(void);
Value makeBool(int boolean);
Value makeNumber(double number);
Value makeStringOwned(char* string);
Value makeStringCopy(const char* string);

Value makeNativeFunction(NativeFunction* nativeFunction);
Value makeFunction(FunctionObject* function);
Value makeClass(ClassObject* classObject);
Value makeInstance(InstanceObject* instance);

void freeValue(Value* value);
Value copyValue(const Value* value);

int valueIsTruthy(const Value* value);
int valueEquals(const Value* a, const Value* b);

const char* valueTypeName(const Value* value);
void printValue(const Value* value);

#endif