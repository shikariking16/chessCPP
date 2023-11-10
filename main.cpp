#include <iostream>
#include <string>
using namespace std;

struct piece{
    int type;
    int position;
};

void hide(){
    // int board[64] = {
    //     4, 3, 2, 5, 6, 2, 3, 4,
    //     1, 1, 1, 1, 1, 1, 1, 1,
    //     0, 0, 0, 0, 0, 0, 0, 0,
    //     0, 0, 0, 0, 0, 0, 0, 0,
    //     0, 0, 0, 0, 0, 0, 0, 0,
    //     0, 0, 0, 0, 0, 0, 0, 0,
    //     -1, -1, -1, -1, -1, -1, -1, -1,
    //     -4, -3, -2, -5, -6, -2, -3, -4
    // };

    // enum enumSquare {
    //     a1, b1, c1, d1, e1, f1, g1, h1,
    //     a2, b2, c2, d2, e2, f2, g2, h2,
    //     a3, b3, c3, d3, e3, f3, g3, h3,
    //     a4, b4, c4, d4, e4, f4, g4, h4,
    //     a5, b5, c5, d5, e5, f5, g5, h5,
    //     a6, b6, c6, d6, e6, f6, g6, h6,
    //     a7, b7, c7, d7, e7, f7, g7, h7,
    //     a8, b8, c8, d8, e8, f8, g8, h8
    // };
}

char* intToChessCoordinate(int number) {
    // Turn the ints 0,1,3 etc and translate it to the correct position on the board

    char* coordinate = (char*)malloc(3 * sizeof(char)); // Allocate memory for the coordinate
    if (coordinate != NULL) {
        coordinate[0] = 'a' + (number % 8);   // Calculate the file (a-h)
        coordinate[1] = '1' + (7 - (number / 8));   // Calculate the rank (1-8)
        coordinate[2] = '\0'; // Null-terminate the string
    }
    return coordinate;
}

int chessCoordinateToInt(string coordinate) {
    // Convert the chess coordinate to an integer index

    if (coordinate.length() != 2) {
        // Invalid coordinate format
        return -1;
    }

    int file = coordinate[0] - 'a';
    int rank = '8' - coordinate[1];

    if (file < 0 || file > 7 || rank < 0 || rank > 7) {
        // Invalid file or rank
        return -1;
    }

    return rank * 8 + file;
}

char typeToString(int type){
    // Convert the type (int) into a char which I can read
    char stringType;
    bool isNegative = false;
    if (type < 0){
        isNegative = true;
        type *= -1;
    }

    switch(type){
        case 6:
            stringType = 'k';
            break;
        case 5:
            stringType = 'q';
            break;
        case 4:
            stringType = 'r';
            break;
        case 3:
            stringType = 'n';
            break;
        case 2:
            stringType = 'b';
            break;
        case 1:
            stringType = 'p';
            break;
        case 0:
            stringType = ' ';
            break;
    }
    
    if (isNegative){
        return stringType;
    }else{
        return toupper(stringType);
    }

}

int* fenToBoard(const string fen) {
    //rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR the starting order for FEN

    int* board = (int*)malloc(64 * sizeof(int)); // Allocate memory for the board

    int x = 0;
    for (int i = 0; fen[i] != '\0'; i++) {
        char curDigit = fen[i];
        int tmpValue = 0;
        bool isBlack = false;

        if (islower(curDigit)){
            isBlack = true;
            curDigit = toupper(curDigit);
        }

        if (isdigit(curDigit)) {
            x += (int)(curDigit - '0') - 1; // Convert character digit to integer
        } else {
            switch (curDigit) {
                case 'K':
                    tmpValue = 6;
                    break;
                case 'Q':
                    tmpValue = 5;
                    break;
                case 'R':
                    tmpValue = 4;
                    break;
                case 'N':
                    tmpValue = 3;
                    break;
                case 'B':
                    tmpValue = 2;
                    break;
                case 'P':
                    tmpValue = 1;
                    break;
                case ' ':
                    tmpValue = 0;
                    break;
                case '/':

                    break;
                    
            }
            if (isBlack){
                tmpValue = tmpValue * -1;
            }
            board[x] = tmpValue;
        }
        if (curDigit != '/'){
            x++;
        }
    }

    return board;
}

void printBoard(int* board) {
    int* newBoard = board;

    int row = 8;
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) {
            printf("\n  +---+---+---+---+---+---+---+---+\n");
            printf("%d |", row);
            row--;
        }
        printf(" %c |", typeToString(newBoard[i]));
    }
    printf("\n  +---+---+---+---+---+---+---+---+\n");
    printf("    A   B   C   D   E   F   G   H\n");
}

void movePondFirstMove(int* board, struct piece *piece, string move){

    int newMove;
    int oldPosition;
    newMove = chessCoordinateToInt(move);

    switch (move[1]){
        case '4':
            oldPosition = newMove + 16; // Position back two
            piece->position = piece->position - 16; // updates the piece position
            break;
        case '3':
            oldPosition = newMove + 8; // Poisition new one
            piece->position = piece->position - 8; // updates the piece position
            break;
        // Black Cases
        case '5':
            oldPosition = newMove - 16; // Position back two
            piece->position = piece->position + 16; // updates the piece position
            break;
        case '6':
            oldPosition = newMove - 8; // Position back two
            piece->position = piece->position + 8; // updates the piece position
            break;
    }

    board[newMove] = board[oldPosition];
    board[oldPosition] = 0; // current becomes nothing
}

void initialize(int* board, struct piece *whitePieces, struct piece *blackPieces){
    // Initialize the Pieces array with the positions and types 
    int whitePieceCount = 0;
    int blackPieceCount = 0;    

    for (int i = 0; i < 64; i++) {
        if (board[i] > 0) {
            whitePieces[whitePieceCount].type = board[i];
            whitePieces[whitePieceCount].position = i;
            whitePieceCount++;
        } else if (board[i] < 0) {
            blackPieces[blackPieceCount].type = board[i];
            blackPieces[blackPieceCount].position = i;
            blackPieceCount++;
        }
    }
}

// TODO
bool isValidMove(int* board, struct piece *piece, int destination){
    int chessPieceType = piece->type;
    bool isBlack = false;

    if (chessPieceType < 0){
        isBlack = true;
        chessPieceType *= -1;
    }

    switch(chessPieceType){
        case 6: // King
            
            break;
        case 5: // Queen
            
            break;
        case 4: // Rook

            break;
        case 3: // Knight

            break;
        case 2: // Bishop

            break;
        case 1: // Pond

            break;
    }

    return false;
}

int main(){
    int* board = fenToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    struct piece whitePieces[16];
    struct piece blackPieces[16];

    initialize(board, whitePieces, blackPieces);

    // This just moved a pond one square forward
    struct piece tmp = whitePieces[0];
    // printf("Piece: %c, Position: %s\n", typeToString(tmp.type), intToChessCoordinate(tmp.position));
    printBoard(board);
    for (int i = 0; i < 4; i++){
    cout << "Enter a move: ";
    string coordinate;
    cin >> coordinate;

    movePondFirstMove(board,&tmp,coordinate);
    //printf("\nPiece: %c, Position: %s\n", typeToString(tmp.type), intToChessCoordinate(tmp.position));
    printBoard(board);
    }
    return 0;
}