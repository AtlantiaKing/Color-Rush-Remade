#include "pch.h"
#include "Engine.h"
#include "Game.h"
#include "MainMenu.h"
#include <iostream>
#include "Music.h"

Engine::Engine( const Window& window ) 
	: m_Window{ window }
	, m_pMainMenu{ new MainMenu{ window.width, window.height } }
	, m_pMusic{ new Music{ } }
{
	Initialize( );
}

Engine::~Engine( )
{
	Cleanup( );
}

void Engine::Initialize( )
{
	m_pMusic->Load("Resources/Music/MainTheme.mp3", true);
}

void Engine::Cleanup( )
{
	delete m_pGame;
	delete m_pMainMenu;
	delete m_pMusic;
}

void Engine::Update( float elapsedSec )
{
	if (m_pGame == nullptr)
	{
		if (m_pMainMenu->CreateAGame())
		{
			const int gridSize{ 13 };

			std::pair<int, int> gameData{ m_pMainMenu->GetGameData() };
			std::pair<InputType, InputType> inputData{ m_pMainMenu->GetInputData() };

			m_pGame = new Game{ m_Window.height, gameData.first, gameData.second, m_pMusic, inputData };
		}
	}
	else
	{
		m_pGame->Update(elapsedSec);
	}
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}

}

void Engine::Draw( ) const
{
	ClearBackground( );

	if (m_pGame == nullptr)
	{
		m_pMainMenu->Draw();
	}
	else
	{
		m_pGame->Draw(m_Window.width, m_Window.height);
	}
}

void Engine::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	if (m_pGame != nullptr)
	{
		m_pGame->KeyDownEvent(e);
	}
}

void Engine::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	if (m_pGame == nullptr)
	{
		m_pMainMenu->AnyButtonPressed();
	}
	/*else
	{
		if (m_pGame->IsGameOver())
		{
			m_pMainMenu->Return();
			delete m_pGame;
			m_pGame = nullptr;
			m_pMusic->Load("Resources/Music/MainTheme.mp3", true);
		}
	}*/
	if (e.keysym.sym == SDLK_ESCAPE)
	{
		if (m_pGame != nullptr)
		{
			delete m_pGame;
			m_pGame = nullptr;
			m_pMusic->Load("Resources/Music/MainTheme.mp3", true);
		}
		m_pMainMenu->Return();
	}
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Engine::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Engine::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	if (m_pGame == nullptr)
	{
		if (e.button == SDL_BUTTON_LEFT)
		{
			m_pMainMenu->Select(Point2f{ float(e.x), float(e.y) });
		}
	}
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Engine::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Engine::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
