#pragma once
#include "InputType.h"

class Texture;

class MainMenu final
{
public:
	MainMenu(float windowWidth, float windowHeight);
	~MainMenu();
	
	void AnyButtonPressed();
	void Return();
	void Select(const Point2f& pos);

	bool CreateAGame() const;
	void Draw() const;
	std::pair<int, int> GetGameData() const;
	std::pair<InputType, InputType> GetInputData() const;
private:
	const Point2f m_WindowSize;
	bool m_InTitleScreen{ true };
	Texture* m_pTitleScreen;
	Texture* m_pMainMenu;
	Texture* m_pPlayersHeader;
	Texture* m_pPlayers;
	Texture* m_pGridSizeHeader;
	Texture* m_pGridSize;
	Texture* m_pP1ControlsHeader;
	Texture* m_pP1Controls;
	Texture* m_pP2ControlsHeader;
	Texture* m_pP2Controls;
	Texture* m_pStartButton;
	Texture* m_pMinusButton;
	Texture* m_pPlusButton;

	int m_Players{1};
	int m_GridSize{ 13 };
	InputType m_P1Controls{ InputType::WASD };
	InputType m_P2Controls{ InputType::Arrows };

	bool m_CreateGame{ false };
};

