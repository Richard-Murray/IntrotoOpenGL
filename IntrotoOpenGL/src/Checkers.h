#ifndef CHECKERS_H
#define CHECKERS_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Entity.h"
#include "AssetManager.h"

class CheckersBoard;
class Checker;
struct Action;

enum BOARD { NONE, BLUEPIECE, REDPIECE, BLUEKING, REDKING, INVALID };
enum PLAYER { PLAYERONE, PLAYERTWO}; //player 1 is blue, player 2 is red

class CheckersManager //exists in the application, only one exists
{
public:
	CheckersManager(AssetManager* assetManager, GLFWwindow* window);
	~CheckersManager();

	void Update(float deltaTime);
	void Draw(BaseCamera* camera);

	BOARD GetPieceValue(int row, int col);

private:
	CheckersBoard* m_checkersBoard;
	CheckersBoard* m_clonedCheckersBoard;
	AssetManager* m_assetManager;
	GLFWwindow* m_window;
	std::vector<Checker> m_checkerModels; // Checker* m_checkerModels;
	std::vector<Entity> m_checkersboardEntities;

	int m_selectedMove;
	bool m_validActionsRetrieved = false;
	//int m_selectedPieceRow, m_selectedPieceColumn;
	bool m_inputLastFrame = false;

	float m_moveTimer = 0;
	int m_testGameNumber = 100;
	//PLAYER m_currentPlayer = PLAYER::PLAYERONE;
};

class CheckersBoard //clone-able board with functionality
{
public:
	CheckersBoard();
	~CheckersBoard();

	void GetValidActions(PLAYER player);
	void GetValidJumpActions(PLAYER player, int row, int col);
	void ApplyAction(Action* action);
	void SetLastAction(Action* action) { m_lastAction = action; };

	BOARD GetPieceValue(int row, int col);
	void SetPieceValue(int row, int col, BOARD checkerType);

	void PromoteKings();

	void Copy(CheckersBoard* checkersBoard);
	float PlayRandomisedGame();
	int PiecesLeftForPlayer(PLAYER player);
	void RemoveNonJumpMoves();
	bool CheckForJumpMoves();

	std::vector<Action*> m_listOfActions;
	PLAYER m_currentPlayer = PLAYER::PLAYERONE;
	int m_board[8][8];

	CheckersBoard* m_parentCheckersBoard;

private:
	Action* m_lastAction;
	
};

class Checker : public Entity //3D representation of individual checkers, not used in calculations
{
public:
	Checker(const char* idName) : Entity(idName){}

	void InitialiseChecker();

	virtual void Update(float deltaTime);
	void SetPieceType(BOARD type);
	void SetPieceLocation(int row, int col);
	int GetPieceRow();
	int GetPieceColumn();
	void Select(bool isSelected);

private:
	int m_row, m_column;
	BOARD m_pieceType;
	bool m_isSelected = false;
};

struct Action
{
	void SetRegularMove(int startRow, int startCol, int endRow, int endCol, PLAYER player, BOARD checkerType);
	void SetJumpMove(int startRow, int startCol, int endRow, int endCol, PLAYER player, BOARD checkerType, int jumpedRow, int jumpedCol);
	Action* Clone();

	int m_startRow, m_startCol;
	int m_endRow, m_endCol;
	int m_jumpedRow, m_jumpedCol;
	bool m_isJump;
	PLAYER m_player;
	BOARD m_checkerType;

	float m_score;
};

#endif