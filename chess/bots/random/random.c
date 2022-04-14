static struct common common;

static int32_t random_makeMove(bool isHost, uint8_t *board, hc_UNUSED int32_t lastMoveFrom, hc_UNUSED int32_t lastMoveTo, int32_t *moveFrom, int32_t *moveTo) {
    common_init(&common, isHost, board);
    common_dumpBoard(&common);
    common_findMoves(&common);

    debug_printNum("Found ", common.numMoves, " moves\n");
    if (common.numMoves == 0) return -1;
    int32_t moveIndex;
    sys_getrandom(&moveIndex, 1, GRND_INSECURE);
    moveIndex %= common.numMoves;
    struct common_move *move = &common.moves[moveIndex];
    *moveFrom = common_CONVERT_INDEX_BACK(move->from);
    *moveTo = common_CONVERT_INDEX_BACK(move->to);
    return 0;
}
