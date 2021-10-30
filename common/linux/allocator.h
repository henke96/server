struct allocator {
    void *mem;
    int64_t size;
};

#define allocator_MEM(TYPE, SELF) ((TYPE *)(SELF)->mem)

static inline void allocator_create(struct allocator *self) {
    self->size = 0;
}

static int allocator_resize(struct allocator *self, int64_t newSize) {
    newSize = nolibc_ALIGN_FORWARD(newSize, 4096);
    if (self->size == newSize) return 0;

    if (newSize == 0) {
        nolibc_munmap(self->mem, self->size);
    } else {
        void *newMem;
        if (self->size == 0) {
            newMem = nolibc_mmap(NULL, newSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
            if ((int64_t)newMem < 0) return -1;
        } else {
            newMem = nolibc_mremap(self->mem, self->size, newSize, MREMAP_MAYMOVE);
            if ((int64_t)newMem < 0) return -1;
        }
        self->mem = newMem;
    }
    self->size = newSize;
    return 0;
}
