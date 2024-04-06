#include "hw4.h"

int captured_index;
int moves_index;
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
    int i, j, num_free;
    for (i = 0; i < 8; i++) {
        num_free = 0;
        for (j = 0; j < 8; j++) {
            if (game->chessboard[i][j] == '.') {
                num_free++;
            } 
            else {
                if (num_free != 0) {
                    *fen++ = num_free + '0'; //need to add '0' for char type
                    num_free = 0;
                }
                *fen++ = game->chessboard[i][j];
            }
        }
        if (num_free != 0) {
            *fen++ = num_free + '0';
        }
        if (i != 7) {
            *fen++ = '/';
        }
    }
    *fen++ = ' ';
    if (game->currentPlayer == WHITE_PLAYER) {
        *fen++ = 'w';
    }
    else if (game->currentPlayer == BLACK_PLAYER) {
        *fen++ = 'b';
    }
    *fen = '\0';
}

bool is_valid_pawn_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (game->chessboard[src_row][src_col] == '.') {
        return false;
    }
    if (piece == 'p') {
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
        else if (dest_row == src_row + 2 && src_row == 1 && game->chessboard[dest_row][dest_col] == '.' && game->chessboard[dest_row-1][dest_col] == '.') {
            return true;
        }
    }
    else if (piece == 'P') {
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
        else if (dest_row == src_row - 2 && src_row == 6 && game->chessboard[dest_row][dest_col] == '.' && game->chessboard[dest_row+1][dest_col] == '.') {
            return true;
        }
    }
    else {
        return false;
    }
    return false;
}

bool is_valid_rook_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (game->chessboard[src_row][src_col] == '.') {
        return false;
    }
    if (dest_row == src_row) { //potential vertical movement
        if (dest_col > src_col) {
            for (int i = src_col+1; i <= dest_col; i++) {
                if ((i != dest_col) && game->chessboard[dest_row][i] != '.') {
                    return false;
                }
            }
            return true;
        }
        else if (dest_col < src_col) {
            for (int i = src_col - 1; i >= dest_col; i--) {
                if ((i != dest_col) && game->chessboard[dest_row][i] != '.') {
                    return false;
                }
            }
            return true;
        }
        else {
            return false;
        }
    }
    else if (dest_col == src_col) { //potential horizontal movement
        if (dest_row > src_row) {
            for (int i = src_row+1; i <= dest_row; i++) {
                if ((i != dest_row) && game->chessboard[i][dest_col] != '.') {
                    return false;
                }
            }
            return true;
        }
        else if (dest_row < src_row) {
            for (int i = src_row - 1; i >= dest_row; i--) {
                if ((i != dest_row) && game->chessboard[i][dest_col] != '.') {
                    return false;
                }
            }
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

bool is_valid_knight_move(int src_row, int src_col, int dest_row, int dest_col) {
    int row_diff = abs(dest_row - src_row);
    int col_diff = abs(dest_col - src_col);
    return (row_diff == 2 && col_diff == 1) || (row_diff == 1 && col_diff == 2);
}

bool is_valid_bishop_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (abs(dest_row - src_row) != abs(dest_col - src_col)) {
        return false;
    }
    int rs, cs;
    if (dest_row > src_row) {
        rs = 1;
    }
    else {
        rs = -1;
    }
    if (dest_col > src_col) {
        cs = 1;
    }
    else {
        cs = -1;
    }
    int row, col;
    for (row = src_row + rs, col = src_col + cs; row != dest_row; row += rs, col += cs) {
        if (game->chessboard[row][col] != '.') {
            return false;
        }
    }
    return true;
}

bool is_valid_queen_move(int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game) || is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
}

bool is_valid_king_move(int src_row, int src_col, int dest_row, int dest_col) {
    return (abs(dest_row - src_row) <= 1) && (abs(dest_col - src_col) <= 1);
}

bool is_valid_move(char piece, int src_row, int src_col, int dest_row, int dest_col, ChessGame *game) {
    if (piece == 'P' || piece == 'p') {
        return is_valid_pawn_move(piece, src_row, src_col, dest_row, dest_col, game);
    }
    else if (piece == 'R' || piece == 'r') {
        return is_valid_rook_move(src_row, src_col, dest_row, dest_col, game);
    }
    else if (piece == 'N' || piece == 'n') {
        return is_valid_knight_move(src_row, src_col, dest_row, dest_col);
    }
    else if (piece == 'B' || piece == 'b') {
        return is_valid_bishop_move(src_row, src_col, dest_row, dest_col, game);
    }
    else if (piece == 'Q' || piece == 'q') {
        return is_valid_queen_move(src_row, src_col, dest_row, dest_col, game);
    }
    else if (piece == 'K' || piece == 'k') {
        return is_valid_king_move(src_row, src_col, dest_row, dest_col);
    }
    return false;
}

void fen_to_chessboard(const char *fen, ChessGame *game) {
    int i = 0, j = 0, k = 0;
    for (k = 0; fen[k] != ' '; k++) {
        if (fen[k] == '/') {
            i++;
            j = 0;
        } 
        else if (isdigit(fen[k])) {
            int num_empty = fen[k] - '0';
            while (num_empty--) {
                game->chessboard[i][j] = '.';
                j++;
            }
        } 
        else {
            game->chessboard[i][j] = fen[k];
            j++;
        }
    }
    if (fen[k+1] == 'w') {
        game->currentPlayer = 0;
    }
    else if (fen[k+1] == 'b') {
        game->currentPlayer = 1;
    }
    game->moveCount = 0;
    game->capturedCount = 0;
}

int parse_move(const char *move, ChessMove *parsed_move) {
    if (strlen(move) < 4 || strlen(move) > 5 || (move[0] < 'a' || move[0] > 'h' || move[2] < 'a' || move[2] > 'h')) {
        return PARSE_MOVE_INVALID_FORMAT;
    }
    if (move[1] < '1' || move[1] > '8' || move[3] < '1' || move[3] > '8') {
        return PARSE_MOVE_OUT_OF_BOUNDS;
    }
    if (strlen(move) == 5) {
        if ((move[1] == '7' && move[3] == '8') || (move[1] == '2' && move[3] == '1')) {
            if (move[4] != 'q' && move[4] != 'r' && move[4] != 'b' && move[4] != 'n') {
                return PARSE_MOVE_INVALID_PROMOTION;
            }
        } else {
            return PARSE_MOVE_INVALID_DESTINATION;
        }
    }
    strncpy(parsed_move->startSquare, move, 2);
    parsed_move->startSquare[2] = '\0';
    if (strlen(move) == 4) {
        strncpy(parsed_move->endSquare, move + 2, 2);
        parsed_move->endSquare[2] = '\0';
    }
    else if (strlen(move) == 5) {
        strncpy(parsed_move->endSquare, move + 2, 3);
        parsed_move->endSquare[3] = '\0';
    }
    return 0;
}

//client = white, server = black
int make_move(ChessGame *game, ChessMove *move, bool is_client, bool validate_move) {
    int start_row = '8' - move->startSquare[1];
    int start_col = move->startSquare[0] - 'a';
    int end_row = '8' - move->endSquare[1];
    int end_col = move->endSquare[0] - 'a';
    char start_piece = game->chessboard[start_row][start_col];
    char dest_piece = game->chessboard[end_row][end_col];
    int promo_flag;
    if ((strlen(move->startSquare) + strlen(move->endSquare)) == 5) {
        promo_flag = 1;
    }
    else if ((strlen(move->startSquare) + strlen(move->endSquare)) == 4) {
        promo_flag = 0;
    }
    if (validate_move) {
        int start_piece_flag;
        int dest_piece_flag;

        if (start_piece == 'P' || start_piece == 'R' || start_piece == 'N' || start_piece == 'B' || start_piece == 'Q' || start_piece == 'K') {
            start_piece_flag = 0; //0 indicates the piece to be moved is white
        }
        else if (start_piece == 'p' || start_piece == 'r' || start_piece == 'n' || start_piece == 'b' || start_piece == 'q' || start_piece == 'k'){
            start_piece_flag = 1; //1 indicates the piece to be moved is black
        }
        else if (start_piece == '.'){
            start_piece_flag = 2; //2 indicates there is no piece to be moved
        }
        if (dest_piece == 'P' || dest_piece == 'R' || dest_piece == 'N' || dest_piece == 'B' || dest_piece == 'Q' || dest_piece == 'K') {
            dest_piece_flag = 0; //0 indicates the piece to be captured is white
        }
        else if (dest_piece == 'p' || dest_piece == 'r' || dest_piece == 'n' || dest_piece == 'b' || dest_piece == 'q' || dest_piece == 'k'){
            dest_piece_flag = 1; //1 indicates the piece to be captured is black
        }
        else if (dest_piece == '.'){
            dest_piece_flag = 2; //2 indicates there is no piece at final location to be captured
        }

        if ((game->currentPlayer == 0 && !is_client) || (game->currentPlayer == 1 && is_client)) {
            return MOVE_OUT_OF_TURN;
        }
        if (start_piece_flag == 2) {
            return MOVE_NOTHING;
        }
        if ((game->currentPlayer == 0 && start_piece_flag == 1) || (game->currentPlayer == 1 && start_piece_flag == 0)) {
            return MOVE_WRONG_COLOR;
        }
        if (start_piece_flag == dest_piece_flag) {
            return MOVE_SUS;
        }   
        if (promo_flag==1 && ((start_piece != 'P') && (start_piece != 'p'))) {
            return MOVE_NOT_A_PAWN;
        }
        if (promo_flag==0) {
            if (start_piece == 'P' && end_row == 7) {
                return MOVE_MISSING_PROMOTION;
            }
            else if (start_piece == 'p' && end_row == 0) {
                return MOVE_MISSING_PROMOTION;
            }
        }
        if (is_valid_move(start_piece, start_row, start_col, end_row, end_col, game) != 1) {
            printf("%d", is_valid_move(start_piece, start_row, start_col, end_row, end_col, game));
            return MOVE_WRONG;
        }
    }

    if (promo_flag==1) {
        game->chessboard[start_row][start_col] = '.';
        if (game->currentPlayer == WHITE_PLAYER) {
            game->chessboard[end_row][end_col] = toupper(move->endSquare[2]);
        }
        else if (game->currentPlayer == BLACK_PLAYER) {
            game->chessboard[end_row][end_col] = move->endSquare[2];
        }
    }
    else if (dest_piece == '.') {
        game->chessboard[start_row][start_col] = '.';
        game->chessboard[end_row][end_col] = start_piece;
    }
    else {
        game->capturedCount++;
        game->capturedPieces[captured_index++] = dest_piece;
        game->chessboard[start_row][start_col] = '.';
        game->chessboard[end_row][end_col] = start_piece;
    }
    game->moveCount++;
    game->moves[moves_index++] = *move;
    game->currentPlayer = 1 - game->currentPlayer;
    return 0;
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
    for (int i = 0; username[i]; i++) {
        if (username[i] == ' ') {
            return -1;
        }
    }
    FILE *file = fopen(db_filename, "a");
    char fen[256];
    chessboard_to_fen(fen, game);
    fprintf(file, "%s:%s\n", username, fen);
    fclose(file);
    return 0;
}

int load_game(ChessGame *game, const char *username, const char *db_filename, int save_number) {
    FILE *file = fopen(db_filename, "r");
    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file)) { //fgets will read until the newline
        char *colon = strchr(line, ':'); //first possible occurrence of the colon
        if (colon != NULL) {
            *colon = '\0';
            if (strcmp(line, username) == 0) {
                count++;
                if (count == save_number) {
                    fen_to_chessboard(colon + 1, game);
                    fclose(file);
                    return 0;
                }
            }
        }
    }
    fclose(file);
    return -1;
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

/* int main() {
    ChessGame game;
    initialize_game(&game);
    ChessMove move;
    parse_move("e2e4", &move);
    int result = make_move(&game, &move, 1, 1);
    display_chessboard(&game);


    return 0;
} */