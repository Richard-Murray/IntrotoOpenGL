#include "Checkers.h"

#include <iostream>
#include <string>

//CheckerManager
CheckersManager::CheckersManager(AssetManager* assetManager)
{
	m_checkersBoard = new CheckersBoard();
	m_clonedCheckersBoard = new CheckersBoard();
	m_clonedCheckersBoard->m_parentCheckersBoard = m_checkersBoard;
	m_assetManager = assetManager;

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			std::string s("Checker");
			s += row;
			s += col;
			//char s[50];
			//sprintf(s, "Checker %d", 5);
			m_checkerModels.emplace_back(s.c_str());
			m_checkerModels.back().Initialise(m_assetManager);
			m_checkerModels.back().InitialiseChecker();
			m_checkerModels.back().SetPieceLocation(row, col);
			glm::mat4 transform = glm::mat4(1);
			transform[3] = glm::vec4(row * 1.1f, 0, col * 1.1f, 1);
			//transform = transform * glm::vec4(glm::vec3(row * 10, col * 10, -5), 1);
			m_checkerModels.back().SetWorldTransform(transform);
		}
	}
}

CheckersManager::~CheckersManager()
{
	delete m_clonedCheckersBoard;
	delete m_checkersBoard;
}

void CheckersManager::Update(float deltaTime)
{
	m_moveTimer += deltaTime;

	for (auto iter = m_checkerModels.begin(); iter != m_checkerModels.end(); iter++)
	{
		iter->SetPieceType(GetPieceValue(iter->GetPieceRow(), iter->GetPieceColumn()));
		iter->Update(deltaTime);
	}

	//m_checkersBoard->GetValidActions(PLAYER::PLAYERONE);
	//if (m_checkersBoard->m_listOfActions.size() > 0 && m_moveTimer >= 0 && m_currentPlayer == PLAYER::PLAYERONE)
	//{
	//	m_checkersBoard->ApplyAction(m_checkersBoard->m_listOfActions[rand() % m_checkersBoard->m_listOfActions.size()]);
	//	m_moveTimer = 0;
	//	m_currentPlayer = PLAYER::PLAYERTWO;
	//}
	//else
	//{
	//	//loses because there's no more moves
	//}
	//m_checkersBoard->GetValidActions(PLAYER::PLAYERTWO);
	//if (m_checkersBoard->m_listOfActions.size() > 0 && m_moveTimer >= 0 && m_currentPlayer == PLAYER::PLAYERTWO)
	//{
	//	m_checkersBoard->ApplyAction(m_checkersBoard->m_listOfActions[rand() % m_checkersBoard->m_listOfActions.size()]);
	//	m_moveTimer = 0;
	//	m_currentPlayer = PLAYER::PLAYERONE;
	//}
	//else
	//{
	//	//loses because there's no more moves
	//}

	m_checkersBoard->GetValidActions(PLAYER::PLAYERONE);
	if (m_checkersBoard->m_listOfActions.size() > 0 && m_moveTimer >= 1 && m_checkersBoard->m_currentPlayer == PLAYER::PLAYERONE)
	{
		m_checkersBoard->RemoveNonJumpMoves();
		m_checkersBoard->ApplyAction(m_checkersBoard->m_listOfActions[rand() % m_checkersBoard->m_listOfActions.size()]);
		m_moveTimer = 0;
		m_checkersBoard->m_currentPlayer = PLAYER::PLAYERTWO;
	}
	else if (m_checkersBoard->m_listOfActions.size() == 0 && m_moveTimer >= 1 && m_checkersBoard->m_currentPlayer == PLAYER::PLAYERONE)
	{
		m_checkersBoard->m_currentPlayer = PLAYER::PLAYERTWO;
		//loses because there's no more moves
	}
	m_checkersBoard->GetValidActions(PLAYER::PLAYERTWO);
	if (m_checkersBoard->m_listOfActions.size() > 0 && m_moveTimer >= 1 && m_checkersBoard->m_currentPlayer == PLAYER::PLAYERTWO)
	{
		m_checkersBoard->RemoveNonJumpMoves();
		for (auto iter = m_checkersBoard->m_listOfActions.begin(); iter < m_checkersBoard->m_listOfActions.end(); iter++)
		{
			m_clonedCheckersBoard->Copy(m_checkersBoard);
			m_clonedCheckersBoard->ApplyAction((*iter));
			m_clonedCheckersBoard->m_currentPlayer = PLAYER::PLAYERONE;
			for (int i = 0; i < m_testGameNumber; i++)
			{
				(*iter)->m_score += m_clonedCheckersBoard->PlayRandomisedGame();
			}
		}

		Action* chosenAction = m_checkersBoard->m_listOfActions[0];
		for (auto iter = m_checkersBoard->m_listOfActions.begin(); iter < m_checkersBoard->m_listOfActions.end(); iter++)
		{
			if ((*iter)->m_score > chosenAction->m_score)
			{
				chosenAction = (*iter);
			}
		}

		m_checkersBoard->ApplyAction(chosenAction);
		m_moveTimer = 0;
		
		//m_checkersBoard->ApplyAction(m_checkersBoard->m_listOfActions[rand() % m_checkersBoard->m_listOfActions.size()]);
		//m_moveTimer = 0;
		m_checkersBoard->m_currentPlayer = PLAYER::PLAYERONE;
	}
	else if (m_checkersBoard->m_listOfActions.size() == 0 && m_moveTimer >= 1 && m_checkersBoard->m_currentPlayer == PLAYER::PLAYERTWO)
	{
		m_checkersBoard->m_currentPlayer = PLAYER::PLAYERONE;
		//loses because there's no more moves
	}


	//if (m_moveTimer < 1)
	//{
	//	m_checkersBoard->PlayRandomisedGame();
	//}

	m_checkersBoard->PromoteKings();
}

void CheckersManager::Draw(BaseCamera* camera)
{
	for (auto iter = m_checkerModels.begin(); iter != m_checkerModels.end(); iter++)
	{
		iter->Draw(camera);
	}
}

BOARD CheckersManager::GetPieceValue(int row, int col)
{
	return m_checkersBoard->GetPieceValue(row, col);
}


//Checkerboard
CheckersBoard::CheckersBoard()
{
	for (int row = 0; row < 8; row++) //row
	{
		for (int col = 0; col < 8; col++) //column
		{
			m_board[row][col] = BOARD::NONE;
		}
	}

	for (int col = 0; col < 8; col++) //column
	{
		if (col % 2)
		{
			m_board[1][col] = BOARD::BLUEPIECE;
		}
		else
		{
			m_board[0][col] = BOARD::BLUEPIECE;
			m_board[2][col] = BOARD::BLUEPIECE;
		}
	}

	for (int col = 0; col < 8; col++)
	{
		if (col % 2)
		{
			m_board[7][col] = BOARD::REDPIECE;
			m_board[5][col] = BOARD::REDPIECE;
		}
		else
		{
			m_board[6][col] = BOARD::REDPIECE;
			
		}
	}
}

CheckersBoard::~CheckersBoard()
{
	if (m_listOfActions.size() > 0)
	{
		for (int i = m_listOfActions.size(); i > 0; i--)
		{
			m_listOfActions.pop_back();
		}
	}
}

void CheckersBoard::GetValidActions(PLAYER player)
{
	m_listOfActions.clear();

	for (int row = 0; row < 8; row++) //row
	{
		for (int col = 0; col < 8; col++) //column
		{
			if (player == PLAYER::PLAYERONE && ((GetPieceValue(row, col) == BOARD::BLUEPIECE) || (GetPieceValue(row, col) == BOARD::BLUEKING)))
			{
				//check top right
				if (row + 1 < 8 && col + 1 < 8)
				{
					if (GetPieceValue(row + 1, col + 1) == BOARD::NONE)
					{
						m_listOfActions.push_back(new Action());
						m_listOfActions.back()->SetRegularMove(row, col, row + 1, col + 1, player, GetPieceValue(row, col));
					}
					else if (GetPieceValue(row + 1, col + 1) == BOARD::REDPIECE || GetPieceValue(row + 1, col + 1) == BOARD::REDKING)
					{
						if (row + 2 < 8 && col + 2 < 8 && GetPieceValue(row + 2, col + 2) == BOARD::NONE)
						{
							m_listOfActions.push_back(new Action());
							m_listOfActions.back()->SetJumpMove(row, col, row + 2, col + 2, player, GetPieceValue(row, col), row + 1, col + 1);
						}
					}
				}

				//check top left
				if (row + 1 < 8 && col - 1 >= 0)
				{
					if (GetPieceValue(row + 1, col - 1) == BOARD::NONE)
					{
						m_listOfActions.push_back(new Action());
						m_listOfActions.back()->SetRegularMove(row, col, row + 1, col - 1, player, GetPieceValue(row, col));
					}
					else if (GetPieceValue(row + 1, col - 1) == BOARD::REDPIECE || GetPieceValue(row + 1, col - 1) == BOARD::REDKING)
					{
						if (row + 2 < 8 && col - 2 >= 0 && GetPieceValue(row + 2, col - 2) == BOARD::NONE)
						{
							m_listOfActions.push_back(new Action());
							m_listOfActions.back()->SetJumpMove(row, col, row + 2, col - 2, player, GetPieceValue(row, col), row + 1, col - 1);
						}
					}
				}

				
				if ((GetPieceValue(row, col) == BOARD::BLUEKING))
				{
					//check bottom right (king)
					if (row - 1 >= 0 && col + 1 < 8)
					{
						if (GetPieceValue(row - 1, col + 1) == BOARD::NONE)
						{
							m_listOfActions.push_back(new Action());
							m_listOfActions.back()->SetRegularMove(row, col, row - 1, col + 1, player, GetPieceValue(row, col));
						}
						else if (GetPieceValue(row - 1, col + 1) == BOARD::REDPIECE || GetPieceValue(row - 1, col + 1) == BOARD::REDKING)
						{
							if (row - 2 >= 0 && col + 2 < 8 && GetPieceValue(row - 2, col + 2) == BOARD::NONE)
							{
								m_listOfActions.push_back(new Action());
								m_listOfActions.back()->SetJumpMove(row, col, row - 2, col + 2, player, GetPieceValue(row, col), row - 1, col + 1);
							}
						}
					}

					//check bottom left (king)
					if (row - 1 >= 0 && col - 1 > 0)
					{
						if (GetPieceValue(row - 1, col - 1) == BOARD::NONE)
						{
							m_listOfActions.push_back(new Action());
							m_listOfActions.back()->SetRegularMove(row, col, row - 1, col - 1, player, GetPieceValue(row, col));
						}
						else if (GetPieceValue(row - 1, col - 1) == BOARD::REDPIECE || GetPieceValue(row - 1, col - 1) == BOARD::REDKING)
						{
							if (row - 2 >= 0 && col - 2 >= 0 && GetPieceValue(row - 2, col - 2) == BOARD::NONE)
							{
								m_listOfActions.push_back(new Action());
								m_listOfActions.back()->SetJumpMove(row, col, row - 2, col - 2, player, GetPieceValue(row, col), row - 1, col - 1);
							}
						}
					}
				}
			}

			if (player == PLAYER::PLAYERTWO && ((GetPieceValue(row, col) == BOARD::REDPIECE) || (GetPieceValue(row, col) == BOARD::REDKING)))
			{
				
				//check bottom right
				if (row - 1 >= 0 && col + 1 < 8)
				{
					if (GetPieceValue(row - 1, col + 1) == BOARD::NONE)
					{
						m_listOfActions.push_back(new Action());
						m_listOfActions.back()->SetRegularMove(row, col, row - 1, col + 1, player, GetPieceValue(row, col));
					}
					else if (GetPieceValue(row - 1, col + 1) == BOARD::BLUEPIECE || GetPieceValue(row - 1, col + 1) == BOARD::BLUEKING)
					{
						if (row - 2 >= 0 && col + 2 < 8 && GetPieceValue(row - 2, col + 2) == BOARD::NONE)
						{
							m_listOfActions.push_back(new Action());
							m_listOfActions.back()->SetJumpMove(row, col, row - 2, col + 2, player, GetPieceValue(row, col), row - 1, col + 1);
						}
					}
				}

				//check bottom left
				if (row - 1 >= 0 && col - 1 >= 0)
				{
					if (GetPieceValue(row - 1, col - 1) == BOARD::NONE)
					{
						m_listOfActions.push_back(new Action());
						m_listOfActions.back()->SetRegularMove(row, col, row - 1, col - 1, player, GetPieceValue(row, col));
					}
					else if (GetPieceValue(row - 1, col - 1) == BOARD::BLUEPIECE || GetPieceValue(row - 1, col - 1) == BOARD::BLUEKING)
					{
						if (row - 2 >= 0 && col - 2 >= 0 && GetPieceValue(row - 2, col - 2) == BOARD::NONE)
						{
							m_listOfActions.push_back(new Action());
							m_listOfActions.back()->SetJumpMove(row, col, row - 2, col - 2, player, GetPieceValue(row, col), row - 1, col - 1);
						}
					}
				}

				if ((GetPieceValue(row, col) == BOARD::REDKING))
				{
					//check top right
					if (row + 1 < 8 && col + 1 < 8)
					{
						if (GetPieceValue(row + 1, col + 1) == BOARD::NONE)
						{
							m_listOfActions.push_back(new Action());
							m_listOfActions.back()->SetRegularMove(row, col, row + 1, col + 1, player, GetPieceValue(row, col));
						}
						else if (GetPieceValue(row + 1, col + 1) == BOARD::BLUEPIECE || GetPieceValue(row + 1, col + 1) == BOARD::BLUEKING)
						{
							if (row + 2 < 8 && col + 2 < 8 && GetPieceValue(row + 2, col + 2) == BOARD::NONE)
							{
								m_listOfActions.push_back(new Action());
								m_listOfActions.back()->SetJumpMove(row, col, row + 2, col + 2, player, GetPieceValue(row, col), row + 1, col + 1);
							}
						}
					}

					//check top left
					if (row + 1 < 8 && col - 1 >= 0)
					{
						if (GetPieceValue(row + 1, col - 1) == BOARD::NONE)
						{
							m_listOfActions.push_back(new Action());
							m_listOfActions.back()->SetRegularMove(row, col, row + 1, col - 1, player, GetPieceValue(row, col));
						}
						else if (GetPieceValue(row + 1, col - 1) == BOARD::BLUEPIECE || GetPieceValue(row + 1, col - 1) == BOARD::BLUEKING)
						{
							if (row + 2 < 8 && col - 2 >= 0 && GetPieceValue(row + 2, col - 2) == BOARD::NONE)
							{
								m_listOfActions.push_back(new Action());
								m_listOfActions.back()->SetJumpMove(row, col, row + 2, col - 2, player, GetPieceValue(row, col), row + 1, col - 1);
							}
						}
					}

					
				}

				////check top right
				//if (row - 1 > 0 || col + 1 < 8)
				//{
				//	if (GetPieceValue(row - 1, col + 1) == BOARD::NONE)
				//	{
				//		m_listOfActions.push_back(new Action());
				//		m_listOfActions.back()->SetRegularMove(row, col, row - 1, col + 1, player, GetPieceValue(row, col));
				//	}
				//	if (GetPieceValue(row - 1, col + 1) == BOARD::REDPIECE || GetPieceValue(row - 1, col + 1) == BOARD::REDKING)
				//	{
				//		if (row - 2 > 0 || col + 2 < 8)
				//		{
				//			m_listOfActions.push_back(new Action());
				//			m_listOfActions.back()->SetJumpMove(row, col, row - 2, col + 2, player, GetPieceValue(row, col), row - 1, col + 1);
				//		}
				//	}
				//}

				////check top left
				//if (row - 1 > 0 || col - 1 > 0)
				//{
				//	if (GetPieceValue(row - 1, col - 1) == BOARD::NONE)
				//	{
				//		m_listOfActions.push_back(new Action());
				//		m_listOfActions.back()->SetRegularMove(row, col, row - 1, col - 1, player, GetPieceValue(row, col));
				//	}
				//	if (GetPieceValue(row - 1, col - 1) == BOARD::REDPIECE || GetPieceValue(row - 1, col + 1) == BOARD::REDKING)
				//	{
				//		if (row - 2 > 0 || col - 2 < 8)
				//		{
				//			m_listOfActions.push_back(new Action());
				//			m_listOfActions.back()->SetJumpMove(row, col, row - 2, col - 2, player, GetPieceValue(row, col), row - 1, col - 1);
				//		}
				//	}
				//}
			}
		}
	}
}

void CheckersBoard::ApplyAction(Action* action)
{
	if (!action->m_isJump)
	{
		SetPieceValue(action->m_startRow, action->m_startCol, BOARD::NONE);
		SetPieceValue(action->m_endRow, action->m_endCol, action->m_checkerType);
	}
	else if (action->m_isJump)
	{
		SetPieceValue(action->m_startRow, action->m_startCol, BOARD::NONE);
		SetPieceValue(action->m_jumpedRow, action->m_jumpedCol, BOARD::NONE);
		SetPieceValue(action->m_endRow, action->m_endCol, action->m_checkerType);
	}
}

BOARD CheckersBoard::GetPieceValue(int row, int col)
{
	return static_cast<BOARD>(m_board[row][col]);
}

void CheckersBoard::SetPieceValue(int row, int col, BOARD checkerType)
{
	m_board[row][col] = checkerType;
}

void CheckersBoard::PromoteKings()
{
	for (int i = 0; i < 8; i++)
	{
		if (m_board[0][i] == BOARD::REDPIECE)
		{
			m_board[0][i] = BOARD::REDKING;
		}
		if (m_board[7][i] == BOARD::BLUEPIECE)
		{
			m_board[7][i] = BOARD::BLUEKING;
		}
	}
}

void CheckersBoard::Copy(CheckersBoard* checkersBoard)
{
	m_currentPlayer = checkersBoard->m_currentPlayer;

	//m_listOfActions.clear();
	if (m_listOfActions.size() > 0)
	{
		for (int i = m_listOfActions.size(); i > 0; i--)
		{
			m_listOfActions.pop_back();
		}
	}

	for (auto iter = checkersBoard->m_listOfActions.begin(); iter < checkersBoard->m_listOfActions.end(); iter++)
	{
		m_listOfActions.push_back((*iter)->Clone());
	}

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			SetPieceValue(row, col, checkersBoard->GetPieceValue(row, col));
		}
	}
}

float CheckersBoard::PlayRandomisedGame()
{
	Copy(m_parentCheckersBoard);
	
	srand(glfwGetTime() + rand());

	float value = 0;

	int bothPlayersStuck = 0;
	int maxNumberOfMoves = 100;
	int numberOfMoves = 0;

	int initialBluePieces = PiecesLeftForPlayer(PLAYER::PLAYERONE);
	int initialRedPieces = PiecesLeftForPlayer(PLAYER::PLAYERTWO);

	while (value == 0)
	{

		bothPlayersStuck = 0;

		GetValidActions(PLAYER::PLAYERONE);
		if (m_listOfActions.size() > 0  && m_currentPlayer == PLAYER::PLAYERONE)
		{
			RemoveNonJumpMoves();
			ApplyAction(m_listOfActions[rand() % m_listOfActions.size()]);
			m_currentPlayer = PLAYER::PLAYERTWO;
		}
		else if (m_currentPlayer == PLAYER::PLAYERONE)
		{
			if (!PiecesLeftForPlayer(PLAYER::PLAYERONE) > 0)
			{
				value++;
				break;
			}
			else
			{
				bothPlayersStuck++;
				m_currentPlayer = PLAYER::PLAYERTWO;
			}
		}
		GetValidActions(PLAYER::PLAYERTWO);
		if (m_listOfActions.size() > 0 && m_currentPlayer == PLAYER::PLAYERTWO)
		{
			RemoveNonJumpMoves();
			ApplyAction(m_listOfActions[rand() % m_listOfActions.size()]);
			m_currentPlayer = PLAYER::PLAYERONE;
		}
		else if (m_currentPlayer == PLAYER::PLAYERTWO)
		{
			if (!PiecesLeftForPlayer(PLAYER::PLAYERTWO) > 0)
			{
				value--;
				break;
			}
			else
			{
				bothPlayersStuck++;
				m_currentPlayer = PLAYER::PLAYERONE;
			}
		}

		if (bothPlayersStuck == 2)
		{
			value--;
			break;
		}

		PromoteKings();

		numberOfMoves++;
		if (numberOfMoves > maxNumberOfMoves)
		{
			if (PiecesLeftForPlayer(PLAYER::PLAYERTWO) - initialRedPieces > PiecesLeftForPlayer(PLAYER::PLAYERONE) - initialBluePieces)
			{
				value++;
			}
			else
			{
				value--;
			}
			return value;
		}

		if (value != 0)
			return value;
	}

	return value;
}

int CheckersBoard::PiecesLeftForPlayer(PLAYER player)
{
	int pieceLeft = 0;

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			if (player == PLAYER::PLAYERONE)
			{
				if (GetPieceValue(row, col) == (BOARD::BLUEPIECE || BOARD::BLUEKING))
				{
					pieceLeft++;
				}
			}
			if (player == PLAYER::PLAYERTWO)
			{
				if (GetPieceValue(row, col) == (BOARD::REDPIECE || BOARD::REDKING))
				{
					pieceLeft++;
				}
			}
		}
	}

	return pieceLeft;
}

void CheckersBoard::RemoveNonJumpMoves()
{
	bool isJump = false;

	for (auto iter = m_listOfActions.begin(); iter < m_listOfActions.end(); iter++)
	{
		if ((*iter)->m_isJump)
		{
			isJump = true;
			break;
		}
	}

	if (isJump == true)
	{
		/*for (auto iter = m_listOfActions.begin(); iter < m_listOfActions.end(); iter++)
		{
			if ((*iter)->m_isJump == false)
			{
				m_listOfActions.erase(iter);
				iter--;
			}
		}*/

		for (int i = 0; i < m_listOfActions.size(); i++)
		{
			if (m_listOfActions[i]->m_isJump == false)
			{
				m_listOfActions.erase(m_listOfActions.begin() + i);
				i--;
			}
		}
		
	}

}


//Checker
void Checker::InitialiseChecker()
{
	AttachModel("Cube1");
	AttachShader("GeometryPass");
}

void Checker::Update(float deltaTime)
{
	if (m_pieceType == BOARD::NONE)
	{
		Hide();
	}
	else
	{
		Show();
		if (m_pieceType == BOARD::BLUEPIECE)
		{
			AttachTexture("Snow");
		}
		if (m_pieceType == BOARD::REDPIECE)
		{
			AttachTexture("Rock");
		}
		if (m_pieceType == BOARD::BLUEKING)
		{
			AttachTexture("Snow");
		}
		if (m_pieceType == BOARD::REDKING)
		{
			AttachTexture("Rock");
		}

	}
	//set piece's model
}

void Checker::SetPieceType(BOARD type)
{
	m_pieceType = type;
}

void Checker::SetPieceLocation(int row, int col)
{
	m_row = row;
	m_column = col;
}

int Checker::GetPieceRow()
{
	return m_row;
}

int Checker::GetPieceColumn()
{
	return m_column;
}

void Action::SetRegularMove(int startRow, int startCol, int endRow, int endCol, PLAYER player, BOARD checkerType)
{
	m_startRow = startRow, m_startCol = startCol;
	m_endRow = endRow, m_endCol = endCol;
	m_jumpedRow = 0, m_jumpedCol = 0;
	m_isJump = false;
	m_player = player;
	m_checkerType = checkerType;
}

void Action::SetJumpMove(int startRow, int startCol, int endRow, int endCol, PLAYER player, BOARD checkerType, int jumpedRow, int jumpedCol)
{
	m_startRow = startRow, m_startCol = startCol;
	m_endRow = endRow, m_endCol = endCol;
	m_jumpedRow = jumpedRow, m_jumpedCol = jumpedCol;
	m_isJump = true;
	m_player = player;
	m_checkerType = checkerType;
}

Action* Action::Clone()
{
	Action* actionClone;
	actionClone = new Action(); //
	
	actionClone->m_startRow = m_startRow;
	actionClone->m_startCol = m_startCol;
	actionClone->m_endRow = m_endRow;
	actionClone->m_endCol = m_endCol;
	actionClone->m_jumpedRow = m_jumpedRow;
	actionClone->m_jumpedCol = m_jumpedCol;
	actionClone->m_isJump = m_isJump;
	actionClone->m_player = m_player;
	actionClone->m_checkerType;

	return actionClone;
}