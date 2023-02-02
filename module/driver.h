#pragma once

#include <linux/mm.h>
#include <linux/list.h>

struct free_pages {
	spinlock_t       lock;
	struct list_head list;
	unsigned long    count;
};

#define FREE_PAGES_INIT(name) {.list = LIST_HEAD_INIT(name.list), .count = 0}
#define FREE_PAGES(name)							\
	struct free_pages name = FREE_PAGES_INIT(name)

static inline void free_pages_init(struct free_pages *fp) {
	spin_lock_init(&fp->lock);
	fp->count = 0;
	INIT_LIST_HEAD(&fp->list);
}

struct exmap_alloc_ctx;

typedef int (*exmap_insert_callback)(struct exmap_alloc_ctx *, unsigned long, struct page *);

int exmap_insert_pages(struct vm_area_struct *vma,
					   unsigned long addr, unsigned long num_pages,
					   struct free_pages *pages,
					   exmap_insert_callback cb, struct exmap_alloc_ctx *data);

int exmap_unmap_pages(struct vm_area_struct *vma,
					  unsigned long addr, unsigned long num_pages,
					  struct free_pages *pages);

pmd_t * exmap_walk_to_pmd(struct vm_area_struct *vma,
						  unsigned long addr);

// #define exmap_debug(...) pr_info("exmap:" __VA_ARGS__)
#define exmap_debug(...) 

