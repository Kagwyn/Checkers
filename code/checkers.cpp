#include <SFML/Graphics.hpp>
#include "checkers_functions.h"


int main(void)
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(800, 900), "Checkers", sf::Style::Titlebar | sf::Style::Close);

	sf::CircleShape possible_circle(50.f);
	possible_circle.setFillColor(sf::Color{ 0, 150, 255 });

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	sf::RectangleShape text_background(sf::Vector2f(800.f, 100.f));
	text_background.setPosition(0, 800);
	text_background.setFillColor(sf::Color{ 0, 0, 20 });

	sf::Text p1_text;
	p1_text.setString("It is red's turn");
	p1_text.setFont(font);
	p1_text.setPosition(25, 825);
	p1_text.setFillColor(sf::Color::Red);

	sf::Text p2_text;
	p2_text.setString("It is magenta's turn");
	p2_text.setFont(font);
	p2_text.setPosition(25, 825);
	p2_text.setFillColor(sf::Color::Magenta);

	sf::Text p1_win_text;
	p1_win_text.setString("The game is over. Red wins!");
	p1_win_text.setFont(font);
	p1_win_text.setPosition(25, 825);
	p1_win_text.setFillColor(sf::Color::White);

	sf::Text p2_win_text;
	p2_win_text.setString("The game is over. Magenta wins!");
	p2_win_text.setFont(font);
	p2_win_text.setPosition(25, 825);
	p2_win_text.setFillColor(sf::Color::White);

	Tile tiles[8][8];
	Piece pieces[24];

	int possible_moves[8][2] = { {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1},{-1, -1}, {-1, -1} };
	int next_moves[8][2];
	int selected_piece = -1;
	int active_piece = -1;
	int turn = 0;
	int double_jump = 0;
	int alive_reds = 12;
	int alive_magentas = 12;
	int pressed_x, pressed_y, released_x, released_y;
	
	Setup(tiles, pieces);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{		
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				pressed_x = sf::Mouse::getPosition(window).x / 100;
				pressed_y = sf::Mouse::getPosition(window).y / 100;
				selected_piece = SearchPieces(pressed_x, pressed_y, pieces);
				if (selected_piece != -1 && (active_piece == -1 || active_piece == selected_piece))
				{
					PossibleMoves(pieces[selected_piece], pieces, tiles, possible_moves, turn, double_jump);
					for (int i = 0; i < 8; i++)
					{
						if (possible_moves[i][0] != -1)
						{
							active_piece = selected_piece;
							break;
						}
					}
				}
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				released_x = sf::Mouse::getPosition(window).x / 100;
				released_y = sf::Mouse::getPosition(window).y / 100;

				for (int i = 0; i < 8; i++)
				{
					if (released_x == possible_moves[i][0] && released_y == possible_moves[i][1])
					{
						pieces[selected_piece].x = released_x;
						pieces[selected_piece].y = released_y;
						tiles[pressed_x][pressed_y].isOccupied = 0;
						tiles[released_x][released_y].isOccupied = 1;
						if (i >= 4)
						{
							KillPiece(pressed_x, released_x, pressed_y, released_y, pieces, tiles, &alive_reds, &alive_magentas);
							PossibleMoves(pieces[selected_piece], pieces, tiles, next_moves, turn, double_jump);

							int negative_count = 0;
							for (int j = 4; j < 8; j++)
							{
								if (next_moves[j][0] == -1)
								{
									negative_count++;
								}
							}
							if (negative_count == 4)
							{
								double_jump = 0;
								ChangeTurn(&turn, &active_piece);
							}
							else
							{
								double_jump = 1;
							}
							break;
						}
						else
						{
							ChangeTurn(&turn, &active_piece);
						}
					}
				}
				for (int i = 0; i < 8; i++)
				{
					possible_moves[i][0] = -1;
					possible_moves[i][1] = -1;
					next_moves[i][0] = -1;
					next_moves[i][1] = -1;
				}
			}	

			if (selected_piece != -1 && pieces[selected_piece].isKing == 0 && ((pieces[selected_piece].color == sf::Color::Magenta && pieces[selected_piece].y == 0) || (pieces[selected_piece].color == sf::Color::Red && pieces[selected_piece].y == 7)))
			{
				pieces[selected_piece].isKing = 1;
			}
			
			
			DrawBoard(window, tiles);
			DrawPieces(window, pieces, active_piece);
			for (int i = 0; i < 8; i++)
			{
				if (possible_moves[i][0] != -1)
				{
					possible_circle.setPosition(possible_moves[i][0]*100, possible_moves[i][1]*100);
					window.draw(possible_circle);
				}
			}

			window.draw(text_background);

			if (alive_magentas == 0)
			{
				window.draw(p1_win_text);
				turn = -1;
			}
			else if (alive_reds == 0)
			{
				window.draw(p2_win_text);
				turn = -1;
			}
			
			if (turn == 0)
			{
				window.draw(p1_text);
			}
			else if (turn == 1)
			{
				window.draw(p2_text);
			}


			window.display();
			window.clear();
		}	
	}

	return 0;
}