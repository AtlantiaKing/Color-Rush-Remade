#include "pch.h"
#include "HUD.h"
#include "Texture.h"

HUD::HUD(const std::pair<InputType, InputType>& inputData)
	: m_pNrRedTexture{ new Texture{ "Tiles: 0", "Resources/font.ttf", 25, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f } } }
	, m_pNrBlueTexture{ new Texture{ "Tiles: 0", "Resources/font.ttf", 25, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f } } }
	, m_InputData{ inputData }
	, m_pArrowsTexture{ new Texture{ "Resources/player2controls.png" } }
	, m_pWASDTexture{ new Texture{ "Resources/player1controls.png" } }
{
	//m_pNrRedTexture = new Texture{ "Tiles: 0", "Resources/font.ttf", 25, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f } };
	//m_pNrBlueTexture = new Texture{ "Tiles: 0", "Resources/font.ttf", 25, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f } };
}

HUD::~HUD()
{
	delete m_pNrBlueTexture;
	delete m_pNrRedTexture;
}

void HUD::UpdateData(int blueTiles, int redTiles, int bluePercentage, int redPercentage)
{
	delete m_pNrRedTexture;
	m_pNrRedTexture = new Texture{ "Tiles: " + std::to_string(redTiles) + " (" + std::to_string(redPercentage) + "%)", "Resources/font.ttf", 25, Color4f{1.0f, 1.0f, 1.0f, 1.0f}};
	delete m_pNrBlueTexture;
	m_pNrBlueTexture = new Texture{ "Tiles: " + std::to_string(blueTiles) + " (" + std::to_string(bluePercentage) + "%)", "Resources/font.ttf", 25, Color4f{ 1.0f, 1.0f, 1.0f, 1.0f } };
}

void HUD::Draw(float windowWidth, float windowHeight) const
{
	const float marginSide{ 175.0f };
	const float marginTop{ 100.0f };

	const Rectf redDstRect
	{
		marginSide - m_pNrRedTexture->GetWidth() / 2.0f,
		windowHeight - marginTop - m_pNrRedTexture->GetHeight(),
		m_pNrRedTexture->GetWidth(),
		m_pNrRedTexture->GetHeight()
	};

	switch (m_InputData.first)
	{
	case InputType::WASD:
	{
		const Rectf firstInputRect
		{
			redDstRect.left + redDstRect.width / 2.0f - m_pWASDTexture->GetWidth() / 3.0f,
			redDstRect.bottom - marginTop - m_pWASDTexture->GetHeight() / 3.0f * 2.0f,
			m_pWASDTexture->GetWidth() / 3.0f * 2.0f,
			m_pWASDTexture->GetHeight() / 3.0f * 2.0f
		};
		m_pWASDTexture->Draw(firstInputRect);
	}
		break;
	case InputType::Arrows:
	{
		const Rectf firstInputRect
		{
			redDstRect.left + redDstRect.width / 2.0f - m_pWASDTexture->GetWidth() / 3.0f,
			redDstRect.bottom - marginTop - m_pWASDTexture->GetHeight() / 3.0f * 2.0f,
			m_pWASDTexture->GetWidth() / 3.0f * 2.0f,
			m_pWASDTexture->GetHeight() / 3.0f * 2.0f
		};
		m_pArrowsTexture->Draw(firstInputRect);
	}
		break;
	case InputType::Both:
	{
		const Rectf firstInputRect
		{
			redDstRect.left + redDstRect.width / 2.0f - m_pWASDTexture->GetWidth() / 3.0f,
			redDstRect.bottom - marginTop - m_pWASDTexture->GetHeight() / 3.0f * 2.0f,
			m_pWASDTexture->GetWidth() / 3.0f * 2.0f,
			m_pWASDTexture->GetHeight() / 3.0f * 2.0f
		};
		m_pWASDTexture->Draw(firstInputRect);
		const Rectf secondInputRect
		{
			firstInputRect.left,
			firstInputRect.bottom - marginTop - m_pArrowsTexture->GetHeight() / 3.0f * 2.0f,
			m_pArrowsTexture->GetWidth() / 3.0f * 2.0f,
			m_pArrowsTexture->GetHeight() / 3.0f * 2.0f
		};
		m_pArrowsTexture->Draw(secondInputRect);
	}
		break;
	}

	m_pNrRedTexture->Draw(redDstRect);

	const Rectf blueDstRect
	{
		windowWidth - marginSide - m_pNrBlueTexture->GetWidth() / 2.0f,
		windowHeight - marginTop - m_pNrRedTexture->GetHeight(),
		m_pNrBlueTexture->GetWidth(),
		m_pNrBlueTexture->GetHeight()
	};

	m_pNrBlueTexture->Draw(blueDstRect);

	switch (m_InputData.second)
	{
	case InputType::WASD:
	{
		const Rectf firstInputRect
		{
			blueDstRect.left + blueDstRect.width / 2.0f - m_pWASDTexture->GetWidth() / 3.0f,
			blueDstRect.bottom - marginTop - m_pWASDTexture->GetHeight() / 3.0f * 2.0f,
			m_pWASDTexture->GetWidth() / 3.0f * 2.0f,
			m_pWASDTexture->GetHeight() / 3.0f * 2.0f
		};
		m_pWASDTexture->Draw(firstInputRect);
	}
	break;
	case InputType::Arrows:
	{
		const Rectf firstInputRect
		{
			blueDstRect.left + blueDstRect.width / 2.0f - m_pWASDTexture->GetWidth() / 3.0f,
			blueDstRect.bottom - marginTop - m_pWASDTexture->GetHeight() / 3.0f * 2.0f,
			m_pWASDTexture->GetWidth() / 3.0f * 2.0f,
			m_pWASDTexture->GetHeight() / 3.0f * 2.0f
		};
		m_pArrowsTexture->Draw(firstInputRect);
	}
	break;
	case InputType::Both:
	{
		const Rectf firstInputRect
		{
			blueDstRect.left + blueDstRect.width / 2.0f - m_pWASDTexture->GetWidth() / 3.0f,
			blueDstRect.bottom - marginTop - m_pWASDTexture->GetHeight() / 3.0f * 2.0f,
			m_pWASDTexture->GetWidth() / 3.0f * 2.0f,
			m_pWASDTexture->GetHeight() / 3.0f * 2.0f
		};
		m_pWASDTexture->Draw(firstInputRect);
		const Rectf secondInputRect
		{
			firstInputRect.left,
			firstInputRect.bottom - marginTop - m_pArrowsTexture->GetHeight() / 3.0f * 2.0f,
			m_pArrowsTexture->GetWidth() / 3.0f * 2.0f,
			m_pArrowsTexture->GetHeight() / 3.0f * 2.0f
		};
		m_pArrowsTexture->Draw(secondInputRect);
	}
	break;
	}
}
