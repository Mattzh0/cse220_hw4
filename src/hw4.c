#include "hw4.h"

void initialize_game(ChessGame *game) {
    char board[8][8] = {
        {'r','n','b','q','k','b','n','r'}, 
        {'p','p','p','p','p','p','p','p'}, 
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'P','P','P','P','P','P','P','P'},
        {'R','N','B','Q','K','B','N','R'}, 
    };
    int move_cnt = 0;
    int capt_cnt = 0;
    int current_player = 0; //0 = white, 1 = black
    ChessMove moves[MAX_MOVES];
    char capturedPieces[MAX_CAPTURED_PIECES] = {0};

    memcpy(game->chessboard, board, sizeof(game->chessboard));
    memcpy(game->moves, moves, sizeof(game->moves));
    memcpy(game->capturedPieces, capturedPieces, sizeof(game->capturedPieces));
    game->moveCount = move_cnt;
    game->capturedCount = capt_cnt;
    game->currentPlayer = current_player;

}

void chessboard_to_fen(char fen[], ChessGame *game) {
    (void)fen;
    (void)game;
}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (piece == 'p') { //white player
        if (dest_row == src_row + 1) {
            if (game->chessboard[dest_row][dest_col] != '.') {
                if ((dest_col == src_col - 1) || (dest_col == src_col + 1)) {
                    return true;
                }
            }
            else if (dest_col == src_col) {
                return true;
            }
        }
        else if (dest_row == src_row + 2 && src_row == 1 && game->chessboard[dest_row][dest_col] == '.') {
            return true;
        }
    }
    else if (piece == 'P') { //black player
        if (dest_row == src_row - 1) {
            if (game->chessboard[dest_row][dest_col] != '.') {
                if ((dest_col == src_col - 1) || (dest_col == src_col + 1)) {
                    return true;
                }
            }
            else if (dest_col == src_col) {
                return true;
            }
        }
        else if (dest_row == src_row - 2 && src_row == 6 && game->chessboard[dest_row][dest_col] == '.') {
            return true;
        }
    }
    return false;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return false;
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    return false;
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    (void)piece;
    (void)src_row;
    (void)src_col;
    (void)dest_row;
    (void)dest_col;
    (void)game;
    return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    (void)fen;
    (void)game;
}

int parse_move(const char *move, ChessMove *parsed_move) {
    (void)move;
    (void)parsed_move;
    return -999;
}

int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    (void)game;
    (void)move;
    (void)is_client;
    (void)validate_move;
    return -999;
}

int send_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int receive_command(ChessGame *game, const char *message, int socketfd, bool is_client) {
    (void)game;
    (void)message;
    (void)socketfd;
    (void)is_client;
    return -999;
}

int save_game(ChessGame *game, const char *username, const char *db_filename) {
    (void)game;
    (void)username;
    (void)db_filename;
    return -999;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    (void)game;
    (void)username;
    (void)db_filename;
    (void)save_number;
    return -999;
}

void display_chessboard(ChessGame *game) {
    printf("\nChessboard:\n");
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", game->chessboard[i][j]);
        }
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}
