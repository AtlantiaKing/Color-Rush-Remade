#pragma once
#include "InputType.h"

class Grid;
class Player;
class ParticleManager;
class HUD;
class Music;
class Texture;

class Game final
{
public:
	Game(float windowHeight, int nrPlayers, int gridSize, Music* pMusic, const std::pair<InputType, InputType>& inputData);
	Game(const Game& other) = delete;
	Game(Game&& other) = delete;
	~Game();
	Game& operator=(const Game& other) = delete;
	Game& operator=(Game&& other) = delete;

	void Update(float elapsedSec);
	void KeyDownEvent(const SDL_KeyboardEvent& e);

	void Draw(float windowWidth, float windowHeight) const;
	bool IsGameOver() const;
private:
	float m_TileSize;

	Grid* m_pGrid;
	Player* m_pRedPlayer;
	Player* m_pBluePlayer;
	ParticleManager* m_pParticleManager;
	HUD* m_pHUD;
	Music* m_pMusic;
	bool m_GameOver{ false };
	bool m_HasMusicTransitioned{ false };
	bool m_IsIntroMusicPlaying{ true };
	std::pair<InputType, InputType> m_InputData;

	Texture* m_pDrawTexture;
	Texture* m_pRedWinsTexture;
	Texture* m_pBlueWinsTexture;

	int m_PercentageBlue{};
	int m_PercentageRed{};
};

