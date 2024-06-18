/**
 * @file

 * @author  Adrian O
 * @brief
 * @version 1
 * @date 2024-06-18
 *
 * @copyright Copyright (c) 2024
 */
#ifndef VECTOR_HEADER_H
// \cond
#define VECTOR_HEADER_H
// \endcond

#include <stdlib.h>

// \cond Ignore for Doxygen
#define VECTOR_IMPL(word) VECTOR_COMB1(VECTOR_PREFIX,word)
#define VECTOR_COMB1(pre, word) VECTOR_COMB2(pre, word)
#define VECTOR_COMB2(pre, word) pre##word
// \endcond

#endif

// \cond
#ifndef VECTOR_T
#error "VECTOR_T must be defined"
#endif

#ifndef VECTOR_NAME
#define VECTOR_NAME VECTOR_COMB1(VECTOR_COMB1(vector,_), VECTOR_T)
#endif

#ifndef VECTOR_PREFIX
#define VECTOR_PREFIX VECTOR_COMB1(VECTOR_NAME, _)
#endif
// \endcond

// \cond
typedef struct VECTOR_NAME VECTOR_NAME;
struct VECTOR_NAME {
    VECTOR_T* data;
    size_t index;
    size_t amount;
};

size_t VECTOR_IMPL(size)(VECTOR_NAME* v);
void VECTOR_IMPL(delete)(VECTOR_NAME* v);


void VECTOR_IMPL(push_back)(VECTOR_NAME* v, VECTOR_T data);
void VECTOR_IMPL(insert)(VECTOR_NAME* v, VECTOR_T data, size_t index);
VECTOR_T VECTOR_IMPL(pop_back)(VECTOR_NAME* v);
VECTOR_T VECTOR_IMPL(remove)(VECTOR_NAME* v, size_t index);

#ifdef IMPLEMENT

size_t VECTOR_IMPL(size)(VECTOR_NAME* v) {
    return v->index;
}

void VECTOR_IMPL(delete)(VECTOR_NAME* v) {
    free(v->data);
    v->amount = 0;
    v->index = 0;
}

void VECTOR_IMPL(push_back)(VECTOR_NAME* v, VECTOR_T data) {
    if (v->index >= v->amount) {
        v->amount = v->amount ? v->amount * 2 : 1;
        v->data = realloc(v->data, v->amount * sizeof(VECTOR_T));
    }

    v->data[v->index++] = data;
}

void VECTOR_IMPL(insert)(VECTOR_NAME* v, VECTOR_T data, size_t index) {
    if (v->index >= v->amount) {
        v->amount = v->amount ? v->amount * 2 : 1;
        v->data = realloc(v->data, v->amount * sizeof(VECTOR_T));
    }

    if (index > v->index || index < 0) {
        return;
    }

    for (size_t i = index;i < v->index;i++) {
        v->data[i + 1] = v->data[i];
    }

    v->data[index] = data;
    v->index++;
}

VECTOR_T VECTOR_IMPL(pop_back)(VECTOR_NAME* v) {
    if (v->index == 0) {
        return (VECTOR_T) { 0 };
    }

    return v->data[--v->index];
}

VECTOR_T VECTOR_IMPL(remove)(VECTOR_NAME* v, size_t index) {
    if (index < 0 || index >= v->index) {
        return (VECTOR_T) { 0 };
    }

    VECTOR_T store = v->data[index];
    for (size_t i = index;i < v->index - 1;i++) {
        v->data[i] = v->data[i + 1];
    }
    v->index--;

    return store;
}

#endif

#undef VECTOR_T
#undef VECTOR_PREFIX
#undef VECTOR_NAME
#undef VECTOR_push_back
#undef VECTOR_insert
#undef VECTOR_pop_back
#undef VECTOR_remove
#undef IMPLEMENT
#undef SAFE

// \endcond