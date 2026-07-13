#ifndef VALUE_H
#define VALUE_H
#include <stddef.h>
#include "token.h"
typedef struct AstNode AstNode;
typedef struct Environment Environment;
typedef struct Runtime Runtime;
typedef struct Value Value;
typedef struct NativeFunction NativeFunction;
typedef struct GcObject GcObject;
typedef struct FunctionObject FunctionObject;
typedef struct MethodEntry MethodEntry;
typedef struct ClassObject ClassObject;
typedef struct InstanceField InstanceField;
typedef struct InstanceObject InstanceObject;
typedef struct BoundMethodObject BoundMethodObject;
typedef struct ListObject ListObject;
typedef struct DictEntry DictEntry;
typedef struct DictObject DictObject;
typedef enum {
    GC_FUNCTION,
    GC_CLASS,
    GC_INSTANCE,
    GC_BOUND_METHOD,
    GC_LIST,
    GC_DICT,
    GC_ENVIRONMENT
} GcObjectType;
struct GcObject {
    GcObjectType type;
    unsigned char marked;
    GcObject* next;
    size_t size;
};
typedef enum {
    VAL_NONE,
    VAL_BOOL,
    VAL_NUMBER,
    VAL_STRING,
    VAL_NATIVE_FUNCTION,
    VAL_FUNCTION,
    VAL_CLASS,
    VAL_INSTANCE,
    VAL_BOUND_METHOD,
    VAL_LIST,
    VAL_DICT
} ValueType;
struct FunctionObject {
    GcObject gc;
    char* name;
    Token* params;
    int paramCount;
    AstNode* body;
    Environment* closure;
};
struct MethodEntry {
    char* name;
    FunctionObject* function;
};
struct ClassObject {
    GcObject gc;
    char* name;
    AstNode* body;
    Environment* closure;
    MethodEntry* methods;
    int methodCount;
    int methodCapacity;
};
struct BoundMethodObject {
    GcObject gc;
    InstanceObject* receiver;
    FunctionObject* method;
};
struct ListObject {
    GcObject gc;
    Value* items;
    int count;
    int capacity;
};
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
        BoundMethodObject* boundMethod;
        ListObject* list;
        DictObject* dict;
    } as;
};
struct DictEntry {
    Value key;
    Value value;
};
struct DictObject {
    GcObject gc;
    DictEntry* entries;
    int count;
    int capacity;
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
    GcObject gc;
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
Value makeBoundMethod(BoundMethodObject* boundMethod);
Value makeList(ListObject* list);
ListObject* createListObject(Runtime* runtime);
int listAppend(ListObject* list, Value value);
Value makeDict(DictObject* dict);
DictObject* createDictObject(Runtime* runtime);
int dictSet(DictObject* dict, Value key, Value value);
int dictGet(DictObject* dict, const Value* key, Value* outValue);
void freeValue(Value* value);
Value copyValue(const Value* value);
int valueIsTruthy(const Value* value);
int valueEquals(const Value* a, const Value* b);
const char* valueTypeName(const Value* value);
void printValue(const Value* value);
#endif


