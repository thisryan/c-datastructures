#ifndef VECTOR_HEADER_H

#include <stdlib.h>

#define RINGBUFFER_IMPL(word) RINGBUFFER_COMB1(RINGBUFFER_PREFIX, word)
#define RINGBUFFER_COMB1(pre, word) RINGBUFFER_COMB2(pre, word)
#define RINGBUFFER_COMB2(pre, word) pre##word
#define RINGBUFFER_NULL_VALUE (RINGBUFFER_T) { 0 };

#endif

#ifndef RINGBUFFER_T
#error "RINGBUFFER_T must be defined"
#endif

#ifndef RINGBUFFER_NAME
#define RINBUFFER_NAME RINGBUFFER_COMB1(RINGBUFFER_COMB1(ringbuffer,_), RINGBUFFER_T)
#endif

#ifndef RINGBUFFER_PREFIX
#define RINGBUFFER_PREFIX RINGBUFFER_COMB1(RINGBUFFER_NAME, _)
#endif

typedef struct RINGBUFFER_NAME RINGBUFFER_NAME;
struct RINGBUFFER_NAME {
    RINGBUFFER_T* data;
    size_t amount;
    size_t head;
    size_t tail;
    size_t size;
};  

size_t RINGBUFFER_IMPL(size)(RINGBUFFER_NAME* rb);
size_t RINGBUFFER_IMPL(delete)(RINGBUFFER_NAME* rb);

void RINGBUFFER_IMPL(push_front)(RINGBUFFER_NAME* rb, RINGBUFFER_T data);
void RINGBUFFER_IMPL(push_back)(RINGBUFFER_NAME* rb, RINGBUFFER_T data);
RINGBUFFER_T RINGBUFFER_IMPL(pop_front)(RINGBUFFER_NAME* rb);
RINGBUFFER_T RINGBUFFER_IMPL(pop_back)(RINGBUFFER_NAME* rb);

RINGBUFFER_T RINGBUFFER_IMPL(front)(RINGBUFFER_NAME* rb);
RINGBUFFER_T RINGBUFFER_IMPL(back)(RINGBUFFER_NAME* rb);

#ifdef IMPLEMENT

size_t RINGBUFFER_IMPL(size)(RINGBUFFER_NAME* rb) {
    return rb->size;
}

size_t RINGBUFFER_IMPL(delete)(RINGBUFFER_NAME* rb) {
    free(rb->data);
}

size_t _ringbuffer_left(RINGBUFFER_NAME *rb, size_t index) {
    if(index == 0) {
        return rb->amount-1;
    }
    return index - 1;
}

size_t _ringbuffer_right(RINGBUFFER_NAME *rb, size_t index) {
    if(index == rb->amount-1) {
        return 0;
    }
    return index + 1;
}

void RINGBUFFER_IMPL(push_front)(RINGBUFFER_NAME* rb, RINGBUFFER_T data) {
    if(rb->size == rb->amount) {
        size_t middle_index = rb->amount;
        rb->amount *= rb->amount == 0 ? 1 : 2;
        rb->data = realloc(rb->data, rb->amount * sizeof(RINGBUFFER_T));
        if(rb->tail < rb->head){
            for(int i = 0;i < rb->tail;i++){
                rb->data[middle_index + i] = rb->data[i];
            }
            rb->tail = middle_index + rb->tail;
        }
    }

    rb->head = _ringbuffer_left(rb, rb->head);
    rb->data[rb->head] = data;
    rb->size++;
}

void RINGBUFFER_IMPL(push_back)(RINGBUFFER_NAME* rb, RINGBUFFER_T data) {
    if(rb->size == rb->amount) {
        size_t middle_index = rb->amount;
        rb->amount *= rb->amount == 0 ? 1 : 2;
        rb->data = realloc(rb->data, rb->amount * sizeof(RINGBUFFER_T));
        if(rb->tail < rb->head){
            for(int i = 0;i < rb->tail;i++){
                rb->data[middle_index + i] = rb->data[i];
            }
            rb->tail = middle_index + rb->tail;
        }
    }

    rb->data[rb->tail] = data;
    rb->tail = _ringbuffer_right(rb, rb->tail);
    rb->size++;
}

// TODO: Shrinking
RINGBUFFER_T RINGBUFFER_IMPL(pop_front)(RINGBUFFER_NAME* rb) {
    size_t prev_index = _ringbuffer_right(rb, rb->head);

    if(rb->head == rb->tail ){
        return RINGBUFFER_NULL_VALUE;
    }

    rb->size--;
    RINGBUFFER_T save = rb->data[rb->head];
    rb->head = prev_index;
    return save;
}

RINGBUFFER_T RINGBUFFER_IMPL(pop_back)(RINGBUFFER_NAME* rb) {
    size_t prev_index = _ringbuffer_left(rb, rb->tail);

    if(rb->tail == rb->head) {
        return RINGBUFFER_NULL_VALUE;
    }

    rb->size--;
    rb->tail = prev_index;
    return rb->data[rb->tail];
}

RINGBUFFER_T RINGBUFFER_IMPL(front)(RINGBUFFER_NAME* rb) {
    if(rb->head == rb->tail){
        return RINGBUFFER_NULL_VALUE;
    }

    return rb->data[rb->head];
}

RINGBUFFER_T RINGBUFFER_IMPL(back)(RINGBUFFER_NAME* rb) {
    if(rb->head == rb->tail){
        return RINGBUFFER_NULL_VALUE;
    }

    return rb->data[_ringbuffer_left(rb, rb->tail)];
}

#endif

#undef RINGBUFFER_T
#undef RINGBUFFER_NAME
#undef RINGBUFFER_PREFIX
#undef IMPLEMENT