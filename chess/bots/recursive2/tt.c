// Transposition table.
// Entries have the form: upperKey(32), score(14), depth(4)
#define tt_MASK(BITS) (((uint64_t)1 << BITS) - 1)

#define tt_SCORE_TYPE_CUTOFF ((uint64_t)1 << 62)
#define tt_SCORE_TYPE_EXACT ((uint64_t)1 << 63)
#define tt_SCORE_TYPE_MASK (tt_SCORE_TYPE_CUTOFF | tt_SCORE_TYPE_EXACT)

#define tt_NUM_ENTRIES ((uint64_t)1 << 21)
#define tt_SCORE_BITS 14
#define tt_DEPTH_BITS 4

#define tt_INDEX(ZOBRIST) ((ZOBRIST) & (tt_NUM_ENTRIES - 1))
#define tt_GET_SCORE(TT_ENTRY) ((int32_t)((TT_ENTRY) >> tt_SCORE_BITS) >> (32 - tt_SCORE_BITS))
#define tt_SHIFT_SCORE(SCORE) (((uint64_t)SCORE & tt_MASK(tt_SCORE_BITS)) << 32)
#define tt_GET_DEPTH(TT_ENTRY) (((TT_ENTRY) >> (32 + tt_SCORE_BITS)) & tt_MASK(tt_DEPTH_BITS))
#define tt_SHIFT_DEPTH(DEPTH) ((uint64_t)(DEPTH) << (32 + tt_SCORE_BITS))

static uint64_t *tt;

static int32_t tt_init(void) {
    tt = sys_mmap(NULL, (tt_NUM_ENTRIES * sizeof(tt[0])), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, 0, 0);
    if ((int64_t)tt < 0) {
        debug_printNum("Failed to allocate transposition table: ", (int64_t)tt, " (probably need to set vm.nr_hugepages)\n");
        return -1;
    }
    return 0;
}

