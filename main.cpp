#include <iostream>
#include <string>
using namespace std;

struct piece{
    int type;
    int position;
    bool alive;
};

void hide(){

    // enum enumSquare {
    // h8, g8, f8, e8, d8, c8, b8, a8,
    // h7, g7, f7, e7, d7, c7, b7, a7,
    // h6, g6, f6, e6, d6, c6, b6, a6,
    // h5, g5, f5, e5, d5, c5, b5, a5,
    // h4, g4, f4, e4, d4, c4, b4, a4,
    // h3, g3, f3, e3, d3, c3, b3, a3,
    // h2, g2, f2, e2, d2, c2, b2, a2,
    // h1, g1, f1, e1, d1, c1, b1, a1
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

void printPieceInfo(struct piece *piece){
    printf("Type: %c\n", typeToString(piece->type));
    printf("Coordinate: %s\n", intToChessCoordinate(piece->position));
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

void movePond(int* board, struct piece *piece, string move){

    int newMove = chessCoordinateToInt(move);
    int oldPosition;
    int diff = piece->position - newMove;
    
    // checks if it is a valid move
    if (diff == 8 || diff == 16){
        oldPosition = newMove + diff; // Position back two
        piece->position = piece->position - diff; // updates the piece position
        board[newMove] = board[oldPosition];
        board[oldPosition] = 0; // current becomes nothing
    }else{
        printf("Invalid Move\n");
    }


}

void movePiece(int* board, struct piece *piece, string move){
    // Move any piece to the move
    int newMove = chessCoordinateToInt(move);
    int oldPosition;
    int diff = piece->position - newMove;
    printf("%d\n", diff);
    
    oldPosition = newMove + diff; // Position back two
    piece->position = piece->position - diff; // updates the piece position
  
    board[newMove] = board[oldPosition];
    board[oldPosition] = 0; // current becomes nothing
}

void moveKnight(int* board, struct piece *piece, string move){
    // Reverse all the nums
    // knight possible moves
    // knight two up (-16) right -15 or left -17
    // knight two down (+16) and then left +17 or right +15
    // knight one up (+8) and then left two (+2) left 10 or right 6
    // First test Nf3 -> Knight to f3

    int newMove = chessCoordinateToInt(move);
    int oldPosition;
    int diff = piece->position - newMove;
    
    // checks if it is a valid move
    if ((diff == -15 || diff == -17 || diff == 17 || diff == 15 || diff == 10 || diff == 6) && newMove > 0 && newMove < 64){
        oldPosition = newMove + diff; // Position back two
        piece->position = piece->position - diff; // updates the piece position
        board[newMove] = board[oldPosition];
        board[oldPosition] = 0; // current becomes nothing
    }else{
        printf("Invalid Move\n");
    }
  
}

void moveRook(int* board, struct piece *piece, string move){
    // move = f3, a2 etc
    int newMove = chessCoordinateToInt(move);
    int oldPosition;
    int diff = piece->position - newMove;
    string curPositionInt = intToChessCoordinate(piece->position);
    
    // checks if it is a valid move
    // Just check if the file and rank are the same etc
    if ((move[0] == curPositionInt[0]) || (move[1] == curPositionInt[1])){
        oldPosition = piece->position; // find the old position
        piece->position = chessCoordinateToInt(move); // updates the piece position
        board[newMove] = board[oldPosition];
        board[oldPosition] = 0; // current becomes nothing
    }else{
        printf("Invalid Move\n");
    }
}

void moveBishop(int* board, struct piece *piece, string move){
    int newMove = chessCoordinateToInt(move);
    int oldPosition;
    int diff = piece->position - newMove;
    
    printf("%d\n", diff);
    // checks if it is a valid move
    if (diff % 9 == 0 || diff % 7 == 0){
        oldPosition = newMove + diff; // Position back two
        piece->position = piece->position - diff; // updates the piece position
        board[newMove] = board[oldPosition];
        board[oldPosition] = 0; // current becomes nothing
    }else{
        printf("Invalid Move\n");
    }
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
    int positionDifference = piece->position - destination;


    if (chessPieceType < 0){
        isBlack = true;
        chessPieceType *= -1;
    }

    switch(chessPieceType){
        case 6: // King
            if (positionDifference >= -9 && positionDifference <= 9) {
                return true;
            }
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

    struct piece tmp = whitePieces[10]; 
    struct piece tmp2 = blackPieces[2];
    printPieceInfo(&tmp2);
    moveBishop(board, &tmp2, "f5");
    printBoard(board);


    return 0;
}