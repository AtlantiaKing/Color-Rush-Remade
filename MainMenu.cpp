#include "pch.h"
#include "MainMenu.h"
#include "Texture.h"
#include "utils.h"

MainMenu::MainMenu(float windowWidth, float windowHeight)
	: m_pTitleScreen{ new Texture{ "Resources/TitleScreen.png" } }
	, m_pMainMenu{ new Texture{ "Resources/MainMenu.png" } }
	, m_pMinusButton{ new Texture{ "Resources/minus.png" } }
	, m_pPlusButton{ new Texture{ "Resources/plus.png" } }
	, m_pStartButton{ new Texture{ "Resources/startbutton.png" } }
	, m_WindowSize{ windowWidth, windowHeight }
{
	const float m{ 6.0f / 840.0f };
	const float q{ 48.0f - 7680.0f / 840.0f };

	const int fontSize{ int(m * windowWidth + q) };
	const Color4f textColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	m_pPlayersHeader = new Texture{ "PLAYERS", "Resources/Font.ttf", fontSize, textColor };
	m_pGridSizeHeader = new Texture{ "GRIDSIZE", "Resources/Font.ttf", fontSize, textColor };
	m_pP1ControlsHeader = new Texture{ "P1 CONTROLS", "Resources/Font.ttf", fontSize, textColor };
	m_pP2ControlsHeader = new Texture{ "P2 CONTROLS", "Resources/Font.ttf", fontSize, textColor };
	m_pPlayers = new Texture{ "1", "Resources/Font.ttf", fontSize, textColor };
	m_pGridSize = new Texture{ "13", "Resources/Font.ttf", fontSize, textColor };
	m_pP1Controls = new Texture{ "WASD", "Resources/Font.ttf", fontSize, textColor };
	m_pP2Controls = new Texture{ "ARROWS", "Resources/Font.ttf", fontSize, textColor };
}

MainMenu::~MainMenu()
{
	delete m_pTitleScreen;
	delete m_pMainMenu;
	delete m_pPlayersHeader;
	delete m_pPlayers;
	delete m_pGridSizeHeader;
	delete m_pGridSize;
	delete m_pP1ControlsHeader;
	delete m_pP1Controls;
	delete m_pP2ControlsHeader;
	delete m_pP2Controls;
	delete m_pStartButton;
	delete m_pMinusButton;
	delete m_pPlusButton;
}

void MainMenu::AnyButtonPressed()
{
	m_InTitleScreen = false;
}

void MainMenu::Return()
{
	m_CreateGame = false;
	m_InTitleScreen = true;
}

void MainMenu::Select(const Point2f& pos)
{
	if (m_InTitleScreen)
	{
		m_InTitleScreen = false;
		return;
	}

	const float leftTextMiddle{ m_WindowSize.x / 4.0f };
	const float rightTextMiddle{ m_WindowSize.x / 4.0f * 3.0f };

	const float layerOneTextY{ 432.0f / 720.0f * m_WindowSize.y };
	const float layerTwoTextY{ 278.0f / 720.0f * m_WindowSize.y };
	const float startButtonY{ 81.0f / 720.0f * m_WindowSize.y };

	const float headerDistance{ 15.0f / 720.0f * m_WindowSize.y };

	const float scale{ m_WindowSize.x / 1280.0f };

	const float m{ 6.0f / 840.0f };
	const float q{ 48.0f - 7680.0f / 840.0f };
	const int fontSize{ int(m * m_WindowSize.x + q) };
	const Color4f textColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	if (pos.x > leftTextMiddle - m_pPlayersHeader->GetWidth() / 2.0f * scale &&
		pos.y > layerOneTextY - headerDistance - m_pMinusButton->GetHeight() * scale &&
		pos.x < leftTextMiddle - m_pPlayersHeader->GetWidth() / 2.0f * scale + m_pMinusButton->GetWidth() * scale &&
		pos.y < layerOneTextY - headerDistance - m_pMinusButton->GetHeight() * scale + m_pMinusButton->GetHeight() * scale)
	{
		--m_Players;
		if (m_Players < 0)
		{
			m_Players = 2;
		}
		delete m_pPlayers;
		m_pPlayers = new Texture{ std::to_string(m_Players), "Resources/Font.ttf", fontSize, textColor};
	}
	else if (pos.x > leftTextMiddle + (m_pPlayersHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale &&
		pos.y > layerOneTextY - headerDistance - m_pMinusButton->GetHeight() * scale &&
		pos.x < leftTextMiddle + (m_pPlayersHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale + m_pPlusButton->GetWidth() * scale &&
		pos.y < layerOneTextY - headerDistance - m_pMinusButton->GetHeight() * scale + m_pMinusButton->GetHeight() * scale)
	{
		++m_Players;
		if (m_Players > 2)
		{
			m_Players = 0;
		}
		delete m_pPlayers;
		m_pPlayers = new Texture{ std::to_string(m_Players), "Resources/Font.ttf", fontSize, textColor };
	}

	else if (pos.x > rightTextMiddle - m_pGridSizeHeader->GetWidth() / 2.0f * scale &&
		pos.y > layerOneTextY - headerDistance - m_pMinusButton->GetHeight() * scale &&
		pos.x < rightTextMiddle - m_pGridSizeHeader->GetWidth() / 2.0f * scale + m_pMinusButton->GetWidth() * scale &&
		pos.y < layerOneTextY - headerDistance - m_pMinusButton->GetHeight() * scale + m_pMinusButton->GetHeight() * scale)
	{
		--m_GridSize;
		if (m_GridSize < 5)
		{
			m_GridSize = 30;
		}
		delete m_pGridSize;
		m_pGridSize = new Texture{ std::to_string(m_GridSize), "Resources/Font.ttf", fontSize, textColor };
	}
	else if (pos.x > rightTextMiddle + (m_pGridSizeHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale &&
		pos.y > layerOneTextY - headerDistance - m_pMinusButton->GetHeight() * scale &&
		pos.x < rightTextMiddle + (m_pGridSizeHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale + m_pPlusButton->GetWidth() * scale &&
		pos.y < layerOneTextY - headerDistance - m_pMinusButton->GetHeight() * scale + m_pMinusButton->GetHeight() * scale)
	{
		++m_GridSize;
		if (m_GridSize > 30)
		{
			m_GridSize = 5;
		}
		delete m_pGridSize;
		m_pGridSize = new Texture{ std::to_string(m_GridSize), "Resources/Font.ttf", fontSize, textColor };
	}

	if (m_Players > 1)
	{
		if (pos.x > leftTextMiddle - m_pP1ControlsHeader->GetWidth() / 2.0f * scale &&
			pos.y > layerTwoTextY - headerDistance - m_pMinusButton->GetHeight() * scale &&
			pos.x < leftTextMiddle - m_pP1ControlsHeader->GetWidth() / 2.0f * scale + m_pMinusButton->GetWidth() * scale &&
			pos.y < layerTwoTextY - headerDistance - m_pMinusButton->GetHeight() * scale + m_pMinusButton->GetHeight() * scale)
		{
			m_P1Controls = InputType(int(m_P1Controls) - 1);
			if (int(m_P1Controls) < 0)
			{
				m_P1Controls = InputType::Both;
			}
			delete m_pP1Controls;
			switch (m_P1Controls)
			{
			case InputType::WASD:
				m_pP1Controls = new Texture{ "WASD", "Resources/Font.ttf", fontSize, textColor };
				break;
			case InputType::Arrows:
				m_pP1Controls = new Texture{ "ARROWS", "Resources/Font.ttf", fontSize, textColor };
				break;
			case InputType::Both:
				m_pP1Controls = new Texture{ "BOTH", "Resources/Font.ttf", fontSize, textColor };
				break;
			}
		}
		else if (pos.x > leftTextMiddle + (m_pP1ControlsHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale &&
			pos.y > layerTwoTextY - headerDistance - m_pPlusButton->GetHeight() * scale &&
			pos.x < leftTextMiddle + (m_pP1ControlsHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale + m_pPlusButton->GetWidth() * scale &&
			pos.y < layerTwoTextY - headerDistance - m_pPlusButton->GetHeight() * scale + m_pPlusButton->GetHeight() * scale)
		{
			m_P1Controls = InputType(int(m_P1Controls) + 1);
			if (int(m_P1Controls) > int(InputType::Both))
			{
				m_P1Controls = InputType::WASD;
			}
			delete m_pP1Controls;
			switch (m_P1Controls)
			{
			case InputType::WASD:
				m_pP1Controls = new Texture{ "WASD", "Resources/Font.ttf", fontSize, textColor };
				break;
			case InputType::Arrows:
				m_pP1Controls = new Texture{ "ARROWS", "Resources/Font.ttf", fontSize, textColor };
				break;
			case InputType::Both:
				m_pP1Controls = new Texture{ "BOTH", "Resources/Font.ttf", fontSize, textColor };
				break;
			}
		}
	}

	if (m_Players > 0)
	{
		if (pos.x > rightTextMiddle - m_pP2ControlsHeader->GetWidth() / 2.0f * scale &&
			pos.y > layerTwoTextY - headerDistance - m_pMinusButton->GetHeight() * scale &&
			pos.x < rightTextMiddle - m_pP2ControlsHeader->GetWidth() / 2.0f * scale + m_pMinusButton->GetWidth() * scale &&
			pos.y < layerTwoTextY - headerDistance - m_pMinusButton->GetHeight() * scale + m_pMinusButton->GetHeight() * scale)
		{
			m_P2Controls = InputType(int(m_P2Controls) - 1);
			if (int(m_P2Controls) < 0)
			{
				m_P2Controls = InputType::Both;
			}
			delete m_pP2Controls;
			switch (m_P2Controls)
			{
			case InputType::WASD:
				m_pP2Controls = new Texture{ "WASD", "Resources/Font.ttf", fontSize, textColor };
				break;
			case InputType::Arrows:
				m_pP2Controls = new Texture{ "ARROWS", "Resources/Font.ttf", fontSize, textColor };
				break;
			case InputType::Both:
				m_pP2Controls = new Texture{ "BOTH", "Resources/Font.ttf", fontSize, textColor };
				break;
			}
		}
		else if (pos.x > rightTextMiddle + (m_pP2ControlsHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale &&
			pos.y > layerTwoTextY - headerDistance - m_pPlusButton->GetHeight() * scale &&
			pos.x < rightTextMiddle + (m_pP2ControlsHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale + m_pPlusButton->GetWidth() * scale &&
			pos.y < layerTwoTextY - headerDistance - m_pPlusButton->GetHeight() * scale + m_pPlusButton->GetHeight() * scale)
		{
			m_P2Controls = InputType(int(m_P2Controls) + 1);
			if (int(m_P2Controls) > int(InputType::Both))
			{
				m_P2Controls = InputType::WASD;
			}
			delete m_pP2Controls;
			switch (m_P2Controls)
			{
			case InputType::WASD:
				m_pP2Controls = new Texture{ "WASD", "Resources/Font.ttf", fontSize, textColor };
				break;
			case InputType::Arrows:
				m_pP2Controls = new Texture{ "ARROWS", "Resources/Font.ttf", fontSize, textColor };
				break;
			case InputType::Both:
				m_pP2Controls = new Texture{ "BOTH", "Resources/Font.ttf", fontSize, textColor };
				break;
			}
		}
	}

	if (pos.x > m_WindowSize.x / 2.0f - m_pStartButton->GetWidth() / 2.0f * scale &&
		pos.y > startButtonY &&
		pos.x < m_WindowSize.x / 2.0f - m_pStartButton->GetWidth() / 2.0f * scale + m_pStartButton->GetWidth() * scale &&
		pos.y < startButtonY + m_pStartButton->GetHeight() * scale)
	{
		m_CreateGame = true;
	}
}

bool MainMenu::CreateAGame() const
{
	return m_CreateGame;
}

void MainMenu::Draw() const
{
	const float halfWidth{ m_WindowSize.x / 2.0f };

	utils::SetColor(Color::red);
	utils::FillRect(0.0f, 0.0f, halfWidth, m_WindowSize.y);
	utils::SetColor(Color::blue);
	utils::FillRect(halfWidth, 0.0f, halfWidth, m_WindowSize.y);

	const Rectf dstRect
	{
		0.0f,
		0.0f, 
		m_WindowSize.x,
		m_WindowSize.y
	};

	if (m_InTitleScreen)
	{
		m_pTitleScreen->Draw(dstRect);
	}
	else
	{
		m_pMainMenu->Draw(dstRect);

		const float leftTextMiddle{ m_WindowSize.x / 4.0f };
		const float rightTextMiddle{ m_WindowSize.x / 4.0f * 3.0f };

		const float layerOneTextY{ 432.0f / 720.0f * m_WindowSize.y };
		const float layerTwoTextY{ 278.0f / 720.0f * m_WindowSize.y };
		const float startButtonY{ 81.0f / 720.0f * m_WindowSize.y };

		const float headerDistance{ 15.0f / 720.0f * m_WindowSize.y };

		const float scale{ m_WindowSize.x / 1280.0f };

		m_pPlayersHeader->Draw(Rectf
			{
				leftTextMiddle - m_pPlayersHeader->GetWidth() / 2.0f * scale,
				layerOneTextY,
				m_pPlayersHeader->GetWidth() * scale,
				m_pPlayersHeader->GetHeight() * scale
			});
		m_pMinusButton->Draw(Rectf
			{
				leftTextMiddle - m_pPlayersHeader->GetWidth() / 2.0f * scale,
				layerOneTextY - headerDistance - m_pMinusButton->GetHeight() * scale,
				m_pMinusButton->GetWidth() * scale,
				m_pMinusButton->GetHeight() * scale
			});
		m_pPlusButton->Draw(Rectf
			{
				leftTextMiddle + (m_pPlayersHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale,
				layerOneTextY - headerDistance - m_pPlusButton->GetHeight() * scale,
				m_pPlusButton->GetWidth() * scale,
				m_pPlusButton->GetHeight() * scale
			});
		m_pPlayers->Draw(Rectf
			{
				leftTextMiddle - m_pPlayers->GetWidth() / 2.0f * scale,
				layerOneTextY - headerDistance - m_pPlayers->GetHeight() * scale,
				m_pPlayers->GetWidth() * scale,
				m_pPlayers->GetHeight() * scale
			});

		m_pGridSizeHeader->Draw(Rectf
			{
				rightTextMiddle - m_pGridSizeHeader->GetWidth() / 2.0f * scale,
				layerOneTextY,
				m_pGridSizeHeader->GetWidth() * scale,
				m_pGridSizeHeader->GetHeight() * scale
			});
		m_pMinusButton->Draw(Rectf
			{
				rightTextMiddle - m_pGridSizeHeader->GetWidth() / 2.0f * scale,
				layerOneTextY - headerDistance - m_pMinusButton->GetHeight() * scale,
				m_pMinusButton->GetWidth() * scale,
				m_pMinusButton->GetHeight() * scale
			});
		m_pPlusButton->Draw(Rectf
			{
				rightTextMiddle + (m_pGridSizeHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale,
				layerOneTextY - headerDistance - m_pPlusButton->GetHeight() * scale,
				m_pPlusButton->GetWidth() * scale,
				m_pPlusButton->GetHeight() * scale
			});
		m_pGridSize->Draw(Rectf
			{
				rightTextMiddle - m_pGridSize->GetWidth() / 2.0f * scale,
				layerOneTextY - headerDistance - m_pGridSize->GetHeight() * scale,
				m_pGridSize->GetWidth() * scale,
				m_pGridSize->GetHeight() * scale
			});

		if (m_Players > 1)
		{
			m_pP1ControlsHeader->Draw(Rectf
				{
					leftTextMiddle - m_pP1ControlsHeader->GetWidth() / 2.0f * scale,
					layerTwoTextY,
					m_pP1ControlsHeader->GetWidth() * scale,
					m_pP1ControlsHeader->GetHeight() * scale
				});
			m_pMinusButton->Draw(Rectf
				{
					leftTextMiddle - m_pP1ControlsHeader->GetWidth() / 2.0f * scale,
					layerTwoTextY - headerDistance - m_pMinusButton->GetHeight() * scale,
					m_pMinusButton->GetWidth() * scale,
					m_pMinusButton->GetHeight() * scale
				});
			m_pPlusButton->Draw(Rectf
				{
					leftTextMiddle + (m_pP1ControlsHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale,
					layerTwoTextY - headerDistance - m_pPlusButton->GetHeight() * scale,
					m_pPlusButton->GetWidth() * scale,
					m_pPlusButton->GetHeight() * scale
				});
			m_pP1Controls->Draw(Rectf
				{
					leftTextMiddle - m_pP1Controls->GetWidth() / 2.0f * scale,
					layerTwoTextY - headerDistance - m_pP1Controls->GetHeight() * scale,
					m_pP1Controls->GetWidth() * scale,
					m_pP1Controls->GetHeight() * scale
				});
		}

		if (m_Players > 0)
		{
			m_pP2ControlsHeader->Draw(Rectf
				{
					rightTextMiddle - m_pP2ControlsHeader->GetWidth() / 2.0f * scale,
					layerTwoTextY,
					m_pP2ControlsHeader->GetWidth() * scale,
					m_pP2ControlsHeader->GetHeight() * scale
				});
			m_pMinusButton->Draw(Rectf
				{
					rightTextMiddle - m_pP2ControlsHeader->GetWidth() / 2.0f * scale,
					layerTwoTextY - headerDistance - m_pMinusButton->GetHeight() * scale,
					m_pMinusButton->GetWidth() * scale,
					m_pMinusButton->GetHeight() * scale
				});
			m_pPlusButton->Draw(Rectf
				{
					rightTextMiddle + (m_pP2ControlsHeader->GetWidth() / 2.0f - m_pPlusButton->GetWidth()) * scale,
					layerTwoTextY - headerDistance - m_pPlusButton->GetHeight() * scale,
					m_pPlusButton->GetWidth() * scale,
					m_pPlusButton->GetHeight() * scale
				});
			m_pP2Controls->Draw(Rectf
				{
					rightTextMiddle - m_pP2Controls->GetWidth() / 2.0f * scale,
					layerTwoTextY - headerDistance - m_pP2Controls->GetHeight() * scale,
					m_pP2Controls->GetWidth() * scale,
					m_pP2Controls->GetHeight() * scale
				});
		}

		m_pStartButton->Draw(Rectf
			{
				m_WindowSize.x / 2.0f - m_pStartButton->GetWidth() / 2.0f * scale,
				startButtonY,
				m_pStartButton->GetWidth() * scale,
				m_pStartButton->GetHeight() * scale
			});
	}
}

std::pair<int, int> MainMenu::GetGameData() const
{
	return std::make_pair(m_Players, m_GridSize);
}

std::pair<InputType, InputType> MainMenu::GetInputData() const
{
	return std::make_pair(m_P1Controls, m_P2Controls);
}
