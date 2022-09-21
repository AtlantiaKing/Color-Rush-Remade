#include "pch.h"
#include "Game.h"
#include "Grid.h"
#include "Player.h"
#include "Bot.h"
#include "ParticleManager.h"
#include "HUD.h"
#include <typeinfo>
#include "Music.h"
#include "Texture.h"

Game::Game(float windowHeight, int nrPlayers, int gridSize, Music* pMusic, const std::pair<InputType, InputType>& inputData)
	: m_pParticleManager{ new ParticleManager{} }
	, m_pMusic{ pMusic }
	, m_InputData{ inputData }
	, m_pDrawTexture{ new Texture{ "Resources/draw.png" } }
	, m_pRedWinsTexture{ new Texture{ "Resources/player1_wins.png" } }
	, m_pBlueWinsTexture{ new Texture{ "Resources/player2_wins.png" } }
{
	const float margin{ 30.0f };
	m_TileSize = (windowHeight - margin * 2) / gridSize;

	m_pGrid = new Grid{ gridSize, m_TileSize };

	switch (nrPlayers)
	{
	case 0:
		m_pRedPlayer = new Bot{ Color::red, Point2i{0,0}, m_TileSize };
		m_pBluePlayer = new Bot{ Color::blue, Point2i{gridSize - 1,gridSize - 1}, m_TileSize };
		m_InputData.first = InputType::None;
		m_InputData.second = InputType::None;
		break;
	case 1:
		m_pRedPlayer = new Bot{ Color::red, Point2i{0,0}, m_TileSize };
		m_pBluePlayer = new Player{ Color::blue, Point2i{gridSize - 1,gridSize - 1}, m_TileSize, inputData.second };
		m_InputData.first = InputType::None;
		break;
	case 2:
		m_pRedPlayer = new Player{ Color::red, Point2i{0,0}, m_TileSize, inputData.first };
		m_pBluePlayer = new Player{ Color::blue, Point2i{gridSize - 1,gridSize - 1}, m_TileSize, inputData.second };
		break;
	default:
		m_pRedPlayer = nullptr;
		m_pBluePlayer = nullptr;
	}
	
	m_pHUD = new HUD{ m_InputData };
	//m_pRedPlayer = new Player{ Color::red, Point2i{0,0}, m_TileSize, false };
	//m_pRedPlayer = new Bot{ Color::red, Point2i{0,0}, m_TileSize };
	//m_pBluePlayer = new Bot{ Color::blue, Point2i{gridSize - 1,gridSize - 1}, m_TileSize };
	//m_pBluePlayer = new Player{ Color::blue, Point2i{gridSize - 1,gridSize - 1}, m_TileSize, true };
	m_pMusic->Load("Resources/Music/GameStart.mp3", false);
}

Game::~Game()
{
	delete m_pGrid;
	delete m_pRedPlayer;
	delete m_pBluePlayer;
	delete m_pParticleManager;
	delete m_pHUD;
}

void Game::Update(float elapsedSec)
{
	if (!m_GameOver)
	{
		const int prevBluePercent{ m_PercentageBlue };
		const int prevRedPercent{ m_PercentageRed };

		if (!m_IsIntroMusicPlaying)
		{
			m_pRedPlayer->Update(elapsedSec, m_TileSize, m_pGrid, m_pParticleManager);
			m_pBluePlayer->Update(elapsedSec, m_TileSize, m_pGrid, m_pParticleManager);
		}

		const int gridSize{ m_pGrid->GetSize() };
		const int nrBlocks{ gridSize * gridSize - m_pGrid->GetCorruptionAmount() };

		const int nrRedTiles{ m_pGrid->GetTilesAmount(Color::red) };
		const int nrBlueTiles{ m_pGrid->GetTilesAmount(Color::blue) };

		m_PercentageBlue = nrBlueTiles * 100 / nrBlocks;
		m_PercentageRed = nrRedTiles * 100 / nrBlocks;

		m_pHUD->UpdateData(/*nrBlueTiles*/int(1.0f/elapsedSec), nrRedTiles, m_PercentageBlue, m_PercentageRed);

		if (!m_pRedPlayer->CanPlace(m_pGrid) && !m_pBluePlayer->CanPlace(m_pGrid) || nrRedTiles > nrBlocks / 2 || nrBlueTiles > nrBlocks / 2)
		{
			m_GameOver = true;
		}
	}
	m_pParticleManager->Update(elapsedSec);

	if (!m_pMusic->IsPlaying())
	{
		m_IsIntroMusicPlaying = false;
		if (m_PercentageBlue >= 10 || m_PercentageRed >= 10)
		{
			if (m_HasMusicTransitioned)
			{
				m_pMusic->Load("Resources/Music/MainThemeEpic.mp3", true);
			}
			else
			{
				if (m_PercentageBlue > 30 || m_PercentageRed > 30)
				{
					m_pMusic->Load("Resources/Music/MainThemeTransitionQuick.mp3", false);
				}
				else
				{
					m_pMusic->Load("Resources/Music/MainThemeTransition.mp3", false);
				}
				m_HasMusicTransitioned = true;
			}
		}
		else
		{
			m_pMusic->Load("Resources/Music/MainThemeInGame.mp3", false);
		}
	}
}

void Game::KeyDownEvent(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_SPACE)
	{
		if (typeid(*m_pRedPlayer) != typeid(Bot))
		{
			m_pRedPlayer->SubmitColor(true);
		}
		if (typeid(*m_pBluePlayer) != typeid(Bot))
		{
			m_pBluePlayer->SubmitColor(true);
		}
	}
	else if(e.keysym.sym == SDLK_RETURN)
	{
		if (typeid(*m_pRedPlayer) != typeid(Bot))
		{
			m_pRedPlayer->SubmitColor(false);
		}
		if (typeid(*m_pBluePlayer) != typeid(Bot))
		{
			m_pBluePlayer->SubmitColor(false);
		}
	}
}

void Game::Draw(float windowWidth, float windowHeight) const
{
	utils::SetColor(Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
	utils::FillRect(0.0f, 0.0f, windowWidth / 2.0f, windowHeight);
	utils::SetColor(Color4f{ 0.0f, 0.0f, 1.0f, 1.0f });
	utils::FillRect(windowWidth / 2.0f, 0.0f, windowWidth / 2.0f, windowHeight);

	const int gridSize{ m_pGrid->GetSize() };

	const float tileSize{ m_TileSize };
	const Point2f bottomLeft
	{
		windowWidth / 2.0f - gridSize / 2.0f * tileSize,
		windowHeight / 2.0f - gridSize / 2.0f * tileSize
	};

	const float gridBorderSize{ 4.0f };
	utils::SetColor(Color4f{ 0.0f, 0.0f, 0.0f, 1.0f });
	utils::FillRect(
		bottomLeft.x - gridBorderSize, 
		bottomLeft.y - gridBorderSize, 
		gridSize * tileSize + gridBorderSize * 2.0f,
		gridSize * tileSize + gridBorderSize * 2.0f);

	glPushMatrix();
	{
		glTranslatef(bottomLeft.x, bottomLeft.y, 0.0f);

		m_pGrid->Draw();
		if (!m_GameOver)
		{
			m_pParticleManager->Draw(tileSize);	
			m_pRedPlayer->Draw(tileSize);
			m_pBluePlayer->Draw(tileSize);
		}
	}
	glPopMatrix();

	m_pHUD->Draw(windowWidth, windowHeight);

	if (m_GameOver)
	{
		const int nrRedTiles{ m_pGrid->GetTilesAmount(Color::red) };
		const int nrBlueTiles{ m_pGrid->GetTilesAmount(Color::blue) };

		const Rectf gameOverDstRect
		{
			0.0f,
			windowHeight / 2.0f - m_pDrawTexture->GetHeight() / 2.0f,
			windowWidth,
			m_pDrawTexture->GetHeight()
		};

		if (nrRedTiles == nrBlueTiles)
		{
			m_pDrawTexture->Draw(gameOverDstRect);
		}
		else if (nrRedTiles > nrBlueTiles)
		{
			m_pRedWinsTexture->Draw(gameOverDstRect);
		}
		else
		{
			m_pBlueWinsTexture->Draw(gameOverDstRect);
		}
	}
}

bool Game::IsGameOver() const
{
	return m_GameOver;
}
