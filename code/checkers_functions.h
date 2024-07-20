#ifndef FUNCTIONSH
#define FUNCTIONSH

class Piece
{
public:
	int x;
	int y;
	int isAlive = 1;
	int isKing = 0;
	sf::Color color;
};

class Tile
{
public:
	int x;
	int y;
	int isOccupied = 0;
	sf::Color color;
};

void Setup(Tile tiles[][8], Piece pieces[24]);

void DrawBoard(sf::RenderWindow& window, Tile tiles[8][8]);

void DrawPieces(sf::RenderWindow& window, Piece pieces[24], int active_piece);

int SearchPieces(int x, int y, Piece pieces[24]);

void ChangeTurn(int* turn, int* active_piece);

void KillPiece(int pressed_x, int released_x, int pressed_y, int released_y, Piece pieces[24], Tile tiles[][8], int* alive_reds, int* alive_magentas);

void PossibleMoves(Piece piece, Piece pieces[24], Tile tiles[][8], int possible_moves[][2], int turn, int double_jump);

#endif