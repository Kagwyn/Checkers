#include <SFML/Graphics.hpp>
#include "checkers_functions.h"


void Setup(Tile tiles[][8], Piece pieces[24])
{
	int piece = 0;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			tiles[x][y].x = x;
			tiles[x][y].y = y;
			if ((x + y) % 2)
			{
				if (y < 3)
				{
					tiles[x][y].isOccupied = 1;
					pieces[piece].x = x;
					pieces[piece].y = y;
					pieces[piece].color = sf::Color::Red;
					piece++;
				}
				else if (y > 4)
				{
					tiles[x][y].isOccupied = 1;
					pieces[piece].x = x;
					pieces[piece].y = y;
					pieces[piece].color = sf::Color::Magenta;
					piece++;
				}
				tiles[x][y].color = sf::Color::Black;
			}
			else
			{
				tiles[x][y].color = sf::Color::White;
			}
		}
	}
}

void DrawBoard(sf::RenderWindow& window, Tile tiles[8][8])
{
	sf::RectangleShape square(sf::Vector2f(100.f, 100.f));

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			square.setPosition(tiles[x][y].x * 100, tiles[x][y].y * 100);
			square.setFillColor(tiles[x][y].color);
			window.draw(square);
		}
	}
}

void DrawPieces(sf::RenderWindow& window, Piece pieces[24], int active_piece)
{
	sf::CircleShape circle(50.f);
	circle.setOutlineColor(sf::Color::Cyan);

	sf::CircleShape king_circle(25.f);
	king_circle.setFillColor(sf::Color{ 100, 0, 180 });
	king_circle.setOrigin(-25, -25);

	for (int i = 0; i < 24; i++)
	{
		if (pieces[i].isAlive)
		{
			circle.setPosition(pieces[i].x * 100, pieces[i].y * 100);
			circle.setFillColor(pieces[i].color);
			if (i == active_piece)
			{
				circle.setOutlineThickness(3.f);
			}
			window.draw(circle);

			if (pieces[i].isKing)
			{
				king_circle.setPosition(pieces[i].x * 100, pieces[i].y * 100);
				window.draw(king_circle);
			}

			circle.setOutlineThickness(0.f);
		}
	}
}

int SearchPieces(int x, int y, Piece pieces[24])
{
	for (int i = 0; i < 24; i++)
	{
		if (pieces[i].x == x && pieces[i].y == y)
		{
			return i;
		}
	}
	return -1;
}

void ChangeTurn(int* turn, int* active_piece)
{
	if (*turn == 0)
	{
		*turn = 1;
	}
	else
	{
		*turn = 0;
	}
	*active_piece = -1;
}

void KillPiece(int pressed_x, int released_x, int pressed_y, int released_y, Piece pieces[24], Tile tiles[][8], int* alive_reds, int* alive_magentas)
{
	int minX;
	int minY;
	if (pressed_x < released_x)
	{
		minX = pressed_x;
	}
	else
	{
		minX = released_x;
	}
	if (pressed_y < released_y)
	{
		minY = pressed_y;
	}
	else
	{
		minY = released_y;
	}
	for (int i = 0; i < 24; i++)
	{
		if (pieces[i].x == minX + 1 && pieces[i].y == minY + 1)
		{
			pieces[i].x = -1;
			pieces[i].y = -1;
			pieces[i].isAlive = 0;
			tiles[minX + 1][minY + 1].isOccupied = 0;
			if (pieces[i].color == sf::Color::Red)
			{
				*alive_reds -= 1;
			}
			else
			{
				*alive_magentas -= 1;
			}
			break;
		}
	}
}

void PossibleMoves(Piece piece, Piece pieces[24], Tile tiles[][8], int possible_moves[][2], int turn, int double_jump)
{
	int x = piece.x;
	int y = piece.y;

	for (int i = 0; i < 8; i++)
	{
		if (((piece.color == sf::Color::Red && turn == 0) || (piece.isKing == 1 && piece.color == sf::Color::Magenta && turn == 1)) && (i == 0 && double_jump == 0 && x - 1 >= 0 && y + 1 <= 7 && tiles[x - 1][y + 1].isOccupied == 0))
		{
			possible_moves[i][0] = x - 1;
			possible_moves[i][1] = y + 1;
		}
		else if (((piece.color == sf::Color::Red && turn == 0) || (piece.isKing == 1 && piece.color == sf::Color::Magenta && turn == 1)) && (i == 1 && double_jump == 0 && x + 1 <= 7 && y + 1 <= 7 && tiles[x + 1][y + 1].isOccupied == 0))
		{
			possible_moves[i][0] = x + 1;
			possible_moves[i][1] = y + 1;
		}
		else if (((piece.color == sf::Color::Magenta && turn == 1) || (piece.isKing == 1 && piece.color == sf::Color::Red && turn == 0)) && (i == 2 && double_jump == 0 && x - 1 >= 0 && y - 1 >= 0 && tiles[x - 1][y - 1].isOccupied == 0))
		{
			possible_moves[i][0] = x - 1;
			possible_moves[i][1] = y - 1;
		}
		else if (((piece.color == sf::Color::Magenta && turn == 1) || (piece.isKing == 1 && piece.color == sf::Color::Red && turn == 0)) && (i == 3 && double_jump == 0 && x + 1 <= 7 && y - 1 >= 0 && tiles[x + 1][y - 1].isOccupied == 0))
		{
			possible_moves[i][0] = x + 1;
			possible_moves[i][1] = y - 1;
		}
		else if (((piece.color == sf::Color::Red && turn == 0) || (piece.isKing == 1 && piece.color == sf::Color::Magenta && turn == 1)) && (i == 4 && x - 2 >= 0 && y + 2 <= 7 && tiles[x - 1][y + 1].isOccupied == 1 && tiles[x - 2][y + 2].isOccupied == 0))
		{
			for (int j = 0; j < 24; j++)
			{
				if (((piece.color == sf::Color::Red && pieces[j].color == sf::Color::Magenta) || (piece.color == sf::Color::Magenta && pieces[j].color == sf::Color::Red)) && (pieces[j].x == x - 1 && pieces[j].y == y + 1))
				{
					possible_moves[i][0] = x - 2;
					possible_moves[i][1] = y + 2;
					break;
				}
			}
		}
		else if (((piece.color == sf::Color::Red && turn == 0) || (piece.isKing == 1 && piece.color == sf::Color::Magenta && turn == 1)) && (i == 5 && x + 2 <= 7 && y + 2 <= 7 && tiles[x + 1][y + 1].isOccupied == 1 && tiles[x + 2][y + 2].isOccupied == 0))
		{
			for (int j = 0; j < 24; j++)
			{
				if (((piece.color == sf::Color::Red && pieces[j].color == sf::Color::Magenta) || (piece.color == sf::Color::Magenta && pieces[j].color == sf::Color::Red)) && (pieces[j].x == x + 1 && pieces[j].y == y + 1))
				{
					possible_moves[i][0] = x + 2;
					possible_moves[i][1] = y + 2;
					break;
				}
			}
		}
		else if (((piece.color == sf::Color::Magenta && turn == 1) || (piece.isKing == 1 && piece.color == sf::Color::Red && turn == 0)) && (i == 6 && x - 2 >= 0 && y - 2 >= 0 && tiles[x - 1][y - 1].isOccupied == 1 && tiles[x - 2][y - 2].isOccupied == 0))
		{
			for (int j = 0; j < 24; j++)
			{
				if (((piece.color == sf::Color::Magenta && pieces[j].color == sf::Color::Red) || (piece.color == sf::Color::Red && pieces[j].color == sf::Color::Magenta)) && (pieces[j].x == x - 1 && pieces[j].y == y - 1))
				{
					possible_moves[i][0] = x - 2;
					possible_moves[i][1] = y - 2;
					break;
				}
			}
		}
		else if (((piece.color == sf::Color::Magenta && turn == 1) || (piece.isKing == 1 && piece.color == sf::Color::Red && turn == 0)) && (i == 7 && x + 2 <= 7 && y - 2 >= 0 && tiles[x + 1][y - 1].isOccupied == 1 && tiles[x + 2][y - 2].isOccupied == 0))
		{
			for (int j = 0; j < 24; j++)
			{
				if (((piece.color == sf::Color::Magenta && pieces[j].color == sf::Color::Red) || (piece.color == sf::Color::Red && pieces[j].color == sf::Color::Magenta)) && (pieces[j].x == x + 1 && pieces[j].y == y - 1))
				{
					possible_moves[i][0] = x + 2;
					possible_moves[i][1] = y - 2;
					break;
				}
			}
		}
	}
}