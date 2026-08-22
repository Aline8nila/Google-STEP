%%writefile mymalloc.c

//
// >>>> malloc challenge! <<<<
//
// Your task is to improve utilization and speed of the following malloc
// implementation.
// Initial implementation is the same as the one implemented in simple_malloc.c.
// For the detailed explanation, please refer to simple_malloc.c.

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Interfaces to get memory pages from OS
//

void *mmap_from_system(size_t size);
void munmap_to_system(void *ptr, size_t size);

//
// Struct definitions
//

typedef struct my_metadata_t {
  size_t size;
  struct my_metadata_t *next;
} my_metadata_t;

typedef struct my_heap_t {
  my_metadata_t *free_head;
  my_metadata_t dummy;
} my_heap_t;

//
// Static variables (DO NOT ADD ANOTHER STATIC VARIABLES!)
//
my_heap_t my_heap;

//
// Helper functions
//

void my_add_to_free_list(my_metadata_t *metadata) {
  assert(!metadata->next)
  metadata->next = my_heap.free_head;
  my_heap.free_head = metadata;
}

void my_remove_from_free_list(my_metadata_t *metadata, my_metadata_t *prev) {
  if (prev) {
    prev->next = metadata->next;
  } else {
    my_heap.free_head = metadata->next;
  }

  metadata->next = NULL;
}

//
// Interfaces of malloc (DO NOT RENAME FOLLOWING FUNCTIONS!)
//

// This is called at the beginning of each challenge.
void my_initialize() {
  my_heap.free_head = &my_heap.dummy;
  my_heap.dummy.size = 0;
  my_heap.dummy.next = NULL;
}

// my_malloc() is called every time an object is allocated.
// |size| is guaranteed to be a multiple of 8 bytes and meets 8 <= |size| <=
// 4000. You are not allowed to use any library functions other than
// mmap_from_system() / munmap_to_system().
void *my_malloc(size_t size) {
  my_metadata_t *metadata = my_heap.free_head;
  my_metadata_t *prev = NULL;

  my_metadata_t *best = NULL;
  my_metadata_t *best_prev = NULL;

  // Best-fit: find the smallest free block that fits.
  while (metadata) {
    if (metadata->size >= size) {
      if (!best || metadata->size < best->size) {
        best = metadata;
        best_prev = prev;
      }
    }

    prev = metadata;
    metadata = metadata->next;
  }

  // If no free block fits, request a new 4096-byte page.
  if (!best) {
    size_t buffer_size = 4096;

    my_metadata_t *new_metadata =
        (my_metadata_t *)mmap_from_system(buffer_size);

    new_metadata->size = buffer_size - sizeof(my_metadata_t);
    new_metadata->next = NULL;

    my_add_to_free_list(new_metadata);

    return my_malloc(size);
  }

  void *ptr = best + 1;
  size_t remaining_size = best->size - size;

  my_remove_from_free_list(best, best_prev);

  // Split the free block if enough space remains.
  if (remaining_size > sizeof(my_metadata_t)) {
    best->size = size;

    my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);

    new_metadata->size = remaining_size - sizeof(my_metadata_t);
    new_metadata->next = NULL;
    // Add the remaining free slot to the free list.
    my_add_to_free_list(new_metadata);
  }

  return ptr;
}

void my_free(void *ptr) {
  if (!ptr) {
    return;
  }

  my_metadata_t *metadata = (my_metadata_t *)ptr - 1;
  metadata->next = NULL;

  my_metadata_t *current = my_heap.free_head;
  my_metadata_t *prev = NULL;

  // Try to merge this freed block with neighboring free blocks.
  while (current) {
    char *metadata_end =
        (char *)metadata + sizeof(my_metadata_t) + metadata->size;

    char *current_end =
        (char *)current + sizeof(my_metadata_t) + current->size;

    // Freed block is directly before current block.
    if (metadata_end == (char *)current) {
      metadata->size += sizeof(my_metadata_t) + current->size;
      my_remove_from_free_list(current, prev);

      current = my_heap.free_head;
      prev = NULL;
      continue;
    }

    // Current block is directly before freed block.
    if (current_end == (char *)metadata) {
      current->size += sizeof(my_metadata_t) + metadata->size;
      metadata = current;

      current = my_heap.free_head;
      prev = NULL;
      continue;
    }

    prev = current;
    current = current->next;
  }

  my_add_to_free_list(metadata);
}

void my_finalize() {
  // Nothing needed here.
}

void test() {
  assert(1 == 1);
}
