#include <stdlib.h>
#include "heap.h"

static inline int heap_parent(int idx)
{
    return (idx - 1) / 2;
}

static inline int heap_left(int idx)
{
    return idx * 2 + 1;
}

static inline int heap_right(int idx)
{
    return idx * 2 + 2;
}

static int heap_valid(heap_t *heap, int idx)
{
    int is_valid = 1;

    if (heap_left(idx) < heap->size) {
        is_valid &= \
            (heap->data[idx].value <= heap->data[heap_left(idx)].value);
    }

    if (heap_right(idx) < heap->size) {
        is_valid &= \
            (heap->data[idx].value <= heap->data[heap_right(idx)].value);
    }

    return is_valid;
}

static void heap_swap(heap_t *heap, int idx1, int idx2)
{
    heap_node_t temp = heap->data[idx1];
    heap->data[idx1] = heap->data[idx2];
    heap->data[idx2] = temp;
}

static void heapify(heap_t *heap)
{
    int idx = 0;

    while (0 == heap_valid(heap, idx) &&
           heap_left(idx) < heap->size) {
        if (heap->data[heap_left(idx)].value <
            heap->data[heap_right(idx)].value) {
            heap_swap(heap, idx, heap_left(idx));
            idx = heap_left(idx);
        } else {
            heap_swap(heap, idx, heap_right(idx));
            idx = heap_right(idx);
        }
    }
}

static void heapify_up(heap_t *heap, int idx)
{
    while (idx > 0 && heap->data[idx].value <
                      heap->data[heap_parent(idx)].value) {
        heap_swap(heap, idx, heap_parent(idx));
        idx = heap_parent(idx);
    }
}

heap_t * heap_init(int size)
{
    heap_t *heap = (heap_t *)malloc(sizeof(heap_t));

    if (heap == NULL) {
        return NULL;
    }

    heap->size = 0;
    heap->capacity = size;
    heap->data = (heap_node_t *)malloc(sizeof(heap_node_t) * size);

    if (heap->data == NULL) {
        return NULL;
    }

    return heap;
}

heap_node_t heap_min(heap_t *heap)
{
    return heap->data[0];
}

heap_node_t heap_extract_min(heap_t *heap)
{
    heap_node_t min = heap->data[0];

    heap->data[0] = heap->data[--heap->size];

    heapify(heap);

    return min;
}

void heap_decrease_key(heap_t *heap, int idx, int new_val)
{
    heap->data[idx].value = new_val;

    heapify_up(heap, idx);
}

void heap_insert(heap_t *heap, int value, void *data)
{
    if (heap->size == heap->capacity) {
        return;
    }

    heap->data[heap->size].value = value;
    heap->data[heap->size].data = data;
    heap->size++;

    heapify_up(heap, heap->size - 1);
}

void heap_delete(heap_t *heap, int idx)
{
    heap_decrease_key(heap, idx, -1);
    heap_extract_min(heap);
}

void heap_free_all(heap_t *heap)
{
    if (heap == NULL) {
        return;
    }

    for (int i = 0; i < heap->size; i++) {
        free(heap->data[i].data);
    }

    free(heap->data);
    heap->size = 0;
    heap->capacity = 0;
    free(heap);
}
