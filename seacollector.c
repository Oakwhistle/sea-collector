#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX 256

//misc code

typedef enum {
    OBJ_INT,
    OBJ_PAIR
} ObjectType;

typedef struct sObject {
    ObjectType type;
    unsigned char marked;

    union {
        /* OBJ_INT */
        int value;

        /* OBJ_PAIR */
        struct {
            struct sObject* head;
            struct sObject* tail;
        };
    };
} Object;

//VM modeling

typedef struct {
    Object* stack[STACK_MAX];
    int stackSize;
}VM;

VM* newVM() {                                             //VM init
    VM* vm = malloc(sizeof(VM));
    vm->stackSize = 0;
    return vm;
}

void push(VM* vm, Object* value) {                        //Stack manipulation
    assert(vm->stackSize < STACK_MAX, "Stack overflow!");
    vm->stack[vm->stackSize++] = value;
}

Object* pop(VM* vm) {                                     //Idem
    assert(vm->stackSize > 0, "Stack underflow!");
    return vm->stack[--vm->stackSize];
}

void mark(Object* object) {
    if (object->marked) return;
    object->marked = 1;

    if (object->type == OBJ_PAIR) {
        mark(object->head);
        mark(object->tail);
    }
}

void markAll(VM* vm)
{
    for (int i = 0; i < vm->stackSize; i++) {
        mark(vm->stack[i]);
    }
}

Object* newObject(VM* vm, ObjectType type) {             //HF to stuff variables, allocate mem and set type tag
  Object* object = malloc(sizeof(Object));
  object->type = type;
  return object;
}

void pushInt(VM* vm, int intValue) {
  Object* object = newObject(vm, OBJ_INT);
  object->value = intValue;
  push(vm, object);
}

Object* pushPair(VM* vm) {
  Object* object = newObject(vm, OBJ_PAIR);
  object->tail = pop(vm);
  object->head = pop(vm);

  push(vm, object);
  return object;
}