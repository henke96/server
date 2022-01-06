// Transposition table.
#define tt_UPPER_BOUND 0x1
#define tt_LOWER_BOUND 0x2
#define tt_EXACT_BOUND (tt_LOWER_BOUND | tt_UPPER_BOUND)

#define tt_NUM_ENTRIES (1 << 28)
#define tt_KEY_MASK (tt_NUM_ENTRIES - 1)

static uint64_t tt[tt_NUM_ENTRIES];
