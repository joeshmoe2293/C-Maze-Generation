#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "heap.h"

#define MAZE_SIZE 27
#define NUM_NODES_ROW ((MAZE_SIZE - 1) / 2)
#define NUM_NODES ((NUM_NODES_ROW) * (NUM_NODES_ROW))
#define BLACK 'B'
#define WHITE ' '

typedef struct {
    int idx;
    int row;
    int col;
    int distance;
    int pred_idx;
} node_data_t;

static inline int sign(int val)
{
    if (val < 0) {
        return -1;
    } else {
        return 1;
    }
}

static inline int maze_idx_from_row_col(int row, int col)
{
    return row * MAZE_SIZE + col;
}

static inline int node_idx_from_row_col(int row, int col)
{
    return row * NUM_NODES_ROW + col;
}

static inline int node_row_from_idx(int idx)
{
    return idx / NUM_NODES_ROW;
}

static inline int node_col_from_idx(int idx)
{
    return idx % NUM_NODES_ROW;
}

static int init_maze(unsigned char *maze_pixels)
{
    if (maze_pixels == NULL) {
        return -1;
    }

    if (MAZE_SIZE % 2 != 1 || MAZE_SIZE < 3) {
        printf("Maze must be an odd size greater than 3\n");
        return -1;
    }

    // Set all grids black
    memset(maze_pixels, BLACK, MAZE_SIZE * MAZE_SIZE);

    // Set every node white
    for (int i = 1; i < MAZE_SIZE - 1; i += 2) {
        for (int j = 1; j < MAZE_SIZE - 1; j += 2) {
            maze_pixels[maze_idx_from_row_col(i, j)] = WHITE;
        }
    }

    return 0;
}

static void print_maze(unsigned char *maze_pixels)
{
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            printf("%c", maze_pixels[maze_idx_from_row_col(i, j)]);
        }

        printf("\n");
    }
}

static int random_number(int max)
{
    static int seeded = 0;
    time_t t;

    if (seeded == 0) {
        srand((unsigned)time(&t));
        seeded = 1;
    }

    return rand() % max;
}

static void mark_empty(unsigned char *maze_pixels, int current_idx,
                       int pred_idx)
{
    int current_row;
    int current_col;
    int pred_row;
    int pred_col;
    int d_row;
    int d_col;

    current_row = 2 * node_row_from_idx(current_idx) + 1;
    current_col = 2 * node_col_from_idx(current_idx) + 1;
    pred_row = 2 * node_row_from_idx(pred_idx) + 1;
    pred_col = 2 * node_col_from_idx(pred_idx) + 1;

    d_row = current_row - pred_row;
    d_col = current_col - pred_col;

    for (int row = 0; abs(row) <= abs(d_row); row += sign(d_row)) {
        for (int col = 0; abs(col) <= abs(d_col); col += sign(d_col)) {
            maze_pixels[maze_idx_from_row_col(pred_row + row, pred_col + col)] = WHITE;
        }
    }
}

static void add_neighbor_to_heap(heap_t *heap, node_data_t *current_node,
                                 char *visited_nodes, int d_row, int d_col)
{
    node_data_t *neighbor_node;
    int neighbor_idx;
    int neighbor_row;
    int neighbor_col;

    neighbor_row = current_node->row + d_row;
    neighbor_col = current_node->col + d_col;

    neighbor_idx = node_idx_from_row_col(neighbor_row, neighbor_col);

    // Make sure the neighbor is not outside the maze boundaries and
    // we haven't previously visited it
    if (neighbor_row >= 0 && neighbor_row < NUM_NODES_ROW &&
        neighbor_col >= 0 && neighbor_col < NUM_NODES_ROW &&
        visited_nodes[neighbor_idx] == 0) {
        // Store the info about this node's location + total distance
        neighbor_node = (node_data_t *)malloc(sizeof(node_data_t));
        neighbor_node->row = neighbor_row;
        neighbor_node->col = neighbor_col;
        neighbor_node->idx = neighbor_idx;
        neighbor_node->distance = random_number(100) + current_node->distance;
        neighbor_node->pred_idx = current_node->idx;

        heap_insert(heap, neighbor_node->distance, (void *)neighbor_node);
        visited_nodes[neighbor_idx] = 1;
    }
}

static void add_neighbors_to_heap(heap_t *heap, node_data_t *current_node,
                                  char *visited_nodes)
{
    // Add north, then west, then east, then south
    add_neighbor_to_heap(heap, current_node, visited_nodes, -1, 0);
    add_neighbor_to_heap(heap, current_node, visited_nodes, 0, -1);
    add_neighbor_to_heap(heap, current_node, visited_nodes, 0, 1);
    add_neighbor_to_heap(heap, current_node, visited_nodes, 1, 0);
}

static int prim(unsigned char *maze_pixels)
{
    int start_node;
    heap_t *edges_heap;
    node_data_t *node_data;
    heap_node_t current_node;
    char *visited_nodes;

    // Heap -> which edge to explore next
    edges_heap = heap_init(NUM_NODES);

    // Processed nodes -> what have we added to the MST
    visited_nodes = (char *)malloc(NUM_NODES);
    memset(visited_nodes, 0, NUM_NODES);

    // Find a random node for starting off
    start_node = random_number(NUM_NODES);

    // Store info about the location of the first node
    node_data = (node_data_t *)malloc(sizeof(node_data_t));
    node_data->idx = start_node;
    node_data->row = node_row_from_idx(start_node);
    node_data->col = node_col_from_idx(start_node);
    node_data->distance = 0;
    node_data->pred_idx = start_node;

    heap_insert(edges_heap, 0, (void *)node_data);
   
    visited_nodes[start_node] = 1;

    while (edges_heap->size > 0) {
        current_node = heap_extract_min(edges_heap);

        /*
        printf("Processing node at row: %d col: %d\n",
               ((node_data_t *)current_node.data)->row,
               ((node_data_t *)current_node.data)->col);
        */

        mark_empty(maze_pixels, ((node_data_t *)current_node.data)->idx,
                   ((node_data_t *)current_node.data)->pred_idx);

        add_neighbors_to_heap(edges_heap, (node_data_t *)current_node.data,
                              visited_nodes);

        free(current_node.data);
    }

    free(visited_nodes);

    heap_free_all(edges_heap);

    return 0;
}

int main(int argc, char *argv[])
{
    unsigned char *maze_pixels;
   
    maze_pixels = (unsigned char *)malloc(MAZE_SIZE * MAZE_SIZE);

    if (maze_pixels == NULL) {
        printf("Malloc error for maze!\n");
        return -1;
    }

    if (0 != init_maze(maze_pixels)) {
        printf("Failed to initialize maze!\n");
        return -1;
    }

    if (0 != prim(maze_pixels)) {
        printf("Failed to run Prim's algorithm!\n");
        return -1;
    }

    print_maze(maze_pixels);

    free(maze_pixels);

    return 0;
}
