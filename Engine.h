#pragma once

class Game;
class MainMenu;
class Music;

class Engine final
{
public:
	explicit Engine( const Window& window );
	Engine(const Engine& other) = delete;
	Engine& operator=(const Engine& other) = delete;
	Engine( Engine&& other) = delete;
	Engine& operator=(Engine&& other) = delete;
	~Engine();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	Game* m_pGame{ nullptr };
	MainMenu* m_pMainMenu;
	Music* m_pMusic;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
};