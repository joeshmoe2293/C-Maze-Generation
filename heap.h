#ifndef __HEAP_H__
#define __HEAP_H__

typedef struct {
    int value;
    void *data;
} heap_node_t;

typedef struct {
    heap_node_t *data;
    int size;
    int capacity;
} heap_t;

heap_t * heap_init(int size);
heap_node_t heap_min(heap_t *heap);
heap_node_t heap_extract_min(heap_t *heap);
void heap_decrease_key(heap_t *heap, int idx, int new_val);
void heap_insert(heap_t *heap, int value, void *data);
void heap_delete(heap_t *heap, int idx);
void heap_free_all(heap_t *heap);

#endif
