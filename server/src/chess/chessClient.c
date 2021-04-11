#include "chess/chessClient.h"
#include "chess/chess.h"
#include "chess/protocol.h"
#include "chess/chessRoom.h"

#include <stdbool.h>
#include <string.h>
#include <assert.h>

static void chessClient_create(struct chessClient *self, struct server_client *client) {
    self->client = client;
    self->room = NULL;
}

static inline void chessClient_setRoom(struct chessClient *self, struct chessRoom *room) {
    self->room = room;
}

static inline bool chessClient_inRoom(struct chessClient *self) {
    return self->room;
}

static inline bool chessClient_isHost(struct chessClient *self) {
    assert(self->room);
    return self->room->host == self;
}

static int32_t chessClient_writeState(struct chessClient *self, struct chess *chess, uint8_t *buffer) {
    if (!chessClient_inRoom(self)) {
        buffer[0] = protocol_HOME;
        return 1;
    }
    if (chessRoom_isFull(self->room)) {
        buffer[0] = protocol_CHESS;
        buffer[1] = chessRoom_isHostsTurn(self->room) ? 1 : 0;
        buffer[2] = chessRoom_getWinner(self->room);
        if (chessClient_isHost(self)) {
            memcpy(&buffer[3], &self->room->board[0], 64);
        } else {
            // Flip the board for black.
            for (int i = 0; i < 64; ++i) {
                buffer[66 - i] = self->room->board[i];
            }
        }
        return chessClient_writeState_MAX;
    }
    buffer[0] = protocol_ROOM;
    memcpy(&buffer[1], &self->room->roomId, 4);
    return 5;
}