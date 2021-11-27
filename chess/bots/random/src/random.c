static int32_t random_makeMove(bool isHost, uint8_t *board, hc_UNUSED int32_t lastMoveFrom, hc_UNUSED int32_t lastMoveTo, int32_t *moveFrom, int32_t *moveTo) {
    common_transformBoard(isHost, board);
    common_dumpBoard();
    common_findMoves();

    debug_printNum("Found ", common_numMoves, " moves\n");
    if (common_numMoves == 0) return -1;
    int32_t moveIndex;
    hc_getrandom(&moveIndex, 1, GRND_INSECURE);
    moveIndex %= common_numMoves;
    struct common_move *move = &common_moves[moveIndex];
    *moveFrom = common_CONVERT_INDEX_BACK(move->from);
    *moveTo = common_CONVERT_INDEX_BACK(move->to);
    return 0;
}
