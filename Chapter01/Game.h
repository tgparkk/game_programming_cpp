// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SDL/SDL.h"
#include <vector>

// Vector2 struct just stores x/y coordinates
// (for now)
struct Vector2
{
	Vector2() : m_x(), m_y() {};
	Vector2(float px, float py) : m_x(px), m_y(py) {};
	Vector2(const Vector2& other) {
		m_x = other.m_x;
		m_y = other.m_y;
	};
	float m_x;
	float m_y;
};

struct Paddle
{
	Paddle(Vector2 vector2, int paddleDir, short player)
		: m_paddle(vector2), m_paddleDir(paddleDir), m_player(player) {};
	Vector2 m_paddle;
	int m_paddleDir;
	short m_player;
};

struct Ball_Info
{
	Ball_Info(): m_ballPos(), m_ballVel() {};
	Ball_Info(Vector2 ballPos, Vector2 ballVel): m_ballPos(ballPos), m_ballVel(ballVel) {};

	Vector2 m_ballPos;
	Vector2 m_ballVel;
};

enum class PLAYER {
	FIRST = 0,
	SECOND = 1
};

// Game class
class Game
{
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;
	
	// Pong specific
	// Direction of paddle
	//int mPaddleDir;

	// Position of paddle
	//Vector2 mPaddlePos;
	std::vector<Paddle> mPaddlesPos; // paddle, paddle info

	// Position of ball
	Vector2 mBallPos;
	Vector2 mBallPos2;


	// Velocity of ball
	Vector2 mBallVel;
	Vector2 mBallVel2;

	std::vector<Ball_Info> m_ball_info;

};
