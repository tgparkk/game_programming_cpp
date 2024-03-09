// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(true)
	//, mPaddleDir(0)
	, mPaddlesPos()
	, mBallPos()
	, mBallPos2()
	, mBallVel()
	, mBallVel2()
	, m_ball_info()
{
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	//
	
	Vector2 player1{ 10.0f ,768.0f / 2.0f }; // left wall
	Vector2 player2{ 1024 - thickness ,768.0f / 2.0f }; // right wall?
	mPaddlesPos.push_back(Paddle{ player1, 0, static_cast<short>(PLAYER::FIRST) });
	mPaddlesPos.push_back(Paddle{ player2, 0, static_cast<short>(PLAYER::SECOND) });


	mBallPos.m_x = 1024.0f/2.0f;
	mBallPos.m_y = 768.0f/2.0f;
	mBallVel.m_x = -200.0f;
	mBallVel.m_y = 235.0f;

	mBallPos2.m_x = 514.0f / 2.0f;
	mBallPos2.m_y = 768.0f / 2.0f;
	mBallVel2.m_x = -100.0f;
	mBallVel2.m_y = 235.0f;

	m_ball_info.push_back(Ball_Info(mBallPos, mBallVel));
	m_ball_info.push_back(Ball_Info(mBallPos2, mBallVel2));
	m_ball_info.push_back(Ball_Info(Vector2(300.0f / 2.0f, 768.0f / 2.0f), Vector2(-100.0f, 235.0f)));
	m_ball_info.push_back(Ball_Info(Vector2(200.0f / 2.0f, 768.0f / 2.0f), Vector2(-150.0f, 235.0f)));
	m_ball_info.push_back(Ball_Info(Vector2(200.0f / 2.0f, 768.0f / 2.0f), Vector2(-50.0f, 235.0f)));
	m_ball_info.push_back(Ball_Info(Vector2(200.0f / 2.0f, 768.0f / 2.0f), Vector2(-25.0f, 235.0f)));
	m_ball_info.push_back(Ball_Info(Vector2(200.0f / 2.0f, 768.0f / 2.0f), Vector2(-55.0f, 235.0f)));

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	//Update paddle direction based on W/S keys
	for (auto it = mPaddlesPos.begin(); it != mPaddlesPos.end(); ++it)
	{
		auto& Paddle = *it;
		Paddle.m_paddleDir = 0;
		if (Paddle.m_player == static_cast<short>(PLAYER::FIRST))
		{
			if (state[SDL_SCANCODE_W])
			{
				Paddle.m_paddleDir -= 1;
			}
			if (state[SDL_SCANCODE_S])
			{
				Paddle.m_paddleDir += 1;
			}
		}

		if (Paddle.m_player == static_cast<short>(PLAYER::SECOND))
		{
			if (state[SDL_SCANCODE_I])
			{
				Paddle.m_paddleDir -= 1;
			}
			if (state[SDL_SCANCODE_K])
			{
				Paddle.m_paddleDir += 1;
			}
		}
		
	}

}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
	// Update paddle position based on direction
	for (auto it = mPaddlesPos.begin(); it != mPaddlesPos.end(); ++it)
	{
		auto& Paddle = *it;
		if (Paddle.m_player == static_cast<short>(PLAYER::FIRST))
		{
			if (Paddle.m_paddleDir != 0)
			{
				Paddle.m_paddle.m_y += Paddle.m_paddleDir * 300.0f * deltaTime;
				// Make sure paddle doesn't move off screen!
				if (Paddle.m_paddle.m_y < (paddleH / 2.0f + thickness))
				{
					Paddle.m_paddle.m_y = paddleH / 2.0f + thickness;
				}
				else if (Paddle.m_paddle.m_y > (768.0f - paddleH / 2.0f - thickness))
				{
					Paddle.m_paddle.m_y = 768.0f - paddleH / 2.0f - thickness;
				}
			}
		}

		if (Paddle.m_player == static_cast<short>(PLAYER::SECOND))
		{
			if (Paddle.m_paddleDir != 0)
			{
				Paddle.m_paddle.m_y += Paddle.m_paddleDir * 300.0f * deltaTime;
				// Make sure paddle doesn't move off screen!
				if (Paddle.m_paddle.m_y < (paddleH / 2.0f + thickness))
				{
					Paddle.m_paddle.m_y = paddleH / 2.0f + thickness;
				}
				else if (Paddle.m_paddle.m_y > (768.0f - paddleH / 2.0f - thickness))
				{
					Paddle.m_paddle.m_y = 768.0f - paddleH / 2.0f - thickness;
				}
			}
		}

	}
	
	// Update ball position based on ball velocity
	mBallPos.m_x += mBallVel.m_x * deltaTime;
	mBallPos.m_y += mBallVel.m_y * deltaTime;

	mBallPos2.m_x += mBallVel2.m_x * deltaTime;
	mBallPos2.m_y += mBallVel2.m_y * deltaTime;

	for (auto& ball_info : m_ball_info)
	{
		auto& ball_pos = ball_info.m_ballPos;
		auto& ball_vel = ball_info.m_ballVel;

		ball_pos.m_x += ball_vel.m_x * deltaTime;
		ball_pos.m_y += ball_vel.m_y * deltaTime;
	}
	
	// Bounce if needed
	// Did we intersect with the paddle?
	
	

	//float diff = mPaddlesPos[0].m_paddle.y - mBallPos.y;
	for (auto it = mPaddlesPos.begin(); it != mPaddlesPos.end(); ++it)
	{
		auto& Paddle = *it;
		
		for (auto& ball_info : m_ball_info)
		{
			auto& ball_pos = ball_info.m_ballPos;
			auto& ball_vel = ball_info.m_ballVel;

			float diff = Paddle.m_paddle.m_y - ball_pos.m_y;

			// Take absolute value of difference
			diff = (diff > 0.0f) ? diff : -diff;
			if (
				// Our y-difference is small enough
				diff <= paddleH / 2.0f &&
				// We are in the correct x-position
				ball_pos.m_x <= 25.0f && ball_pos.m_x >= 20.0f &&
				// The ball is moving to the left
				ball_vel.m_x < 0.0f)
			{
				ball_vel.m_x *= -1.0f;
			}
			else if (
				// Our y-difference is small enough
				diff <= paddleH / 2.0f &&
				// We are in the correct x-position
				ball_pos.m_x >= (1000.0f - thickness) && ball_pos.m_x <= (1005.0f - thickness) &&
				// The ball is moving to the left
				ball_vel.m_x > 0)
			{
				ball_vel.m_x *= -1.0f;
			}
			// Did the ball go off the screen? (if so, end game)
			else if (ball_pos.m_x <= 0.0f || ball_pos.m_x > (1024.0f - thickness))
			{
				//mIsRunning = false;
			}
			//// Did the ball collide with the right wall?
			//else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
			//{
			//	mBallVel.x *= -1.0f;
			//}

			// Did the ball collide with the top wall?
			if (ball_pos.m_y <= thickness && ball_vel.m_y < 0.0f)
			{
				ball_vel.m_y *= -1;
			}
			// Did the ball collide with the bottom wall?
			else if (ball_pos.m_y >= (768 - thickness) &&
				ball_vel.m_y > 0.0f)
			{
				ball_vel.m_y *= -1;
			}

		}


		//float diff = Paddle.m_paddle.m_y - mBallPos.m_y;

		//// Take absolute value of difference
		//diff = (diff > 0.0f) ? diff : -diff;
		//if (
		//	// Our y-difference is small enough
		//	diff <= paddleH / 2.0f &&
		//	// We are in the correct x-position
		//	mBallPos.m_x <= 25.0f && mBallPos.m_x >= 20.0f &&
		//	// The ball is moving to the left
		//	mBallVel.m_x < 0.0f)
		//{
		//	mBallVel.m_x *= -1.0f;
		//}
		//else if (
		//	// Our y-difference is small enough
		//	diff <= paddleH / 2.0f &&
		//	// We are in the correct x-position
		//	mBallPos.m_x >= (1000.0f - thickness) && mBallPos.m_x <= (1005.0f - thickness) &&
		//	// The ball is moving to the left
		//	mBallVel.m_x > 0)
		//{
		//	mBallVel.m_x *= -1.0f;
		//}
		//// Did the ball go off the screen? (if so, end game)
		//else if (mBallPos.m_x <= 0.0f || mBallPos.m_x > (1024.0f - thickness))
		//{
		//	mIsRunning = false;
		//}
		////// Did the ball collide with the right wall?
		////else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
		////{
		////	mBallVel.x *= -1.0f;
		////}

		//// Did the ball collide with the top wall?
		//if (mBallPos.m_y <= thickness && mBallVel.m_y < 0.0f)
		//{
		//	mBallVel.m_y *= -1;
		//}
		//// Did the ball collide with the bottom wall?
		//else if (mBallPos.m_y >= (768 - thickness) &&
		//	mBallVel.m_y > 0.0f)
		//{
		//	mBallVel.m_y *= -1;
		//}
	}

	
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw right wall
	//wall.x = 1024 - thickness;
	//wall.y = 0;
	//wall.w = thickness;
	//wall.h = 1024;
	//SDL_RenderFillRect(mRenderer, &wall);
	

	for (auto it = mPaddlesPos.begin(); it != mPaddlesPos.end(); ++it)
	{
		auto& Paddle = *it;
		
		// Draw paddle
		SDL_Rect paddle{
			static_cast<int>(Paddle.m_paddle.m_x),
			static_cast<int>(Paddle.m_paddle.m_y - paddleH / 2),
			thickness,
			static_cast<int>(paddleH)
		};
		SDL_RenderFillRect(mRenderer, &paddle);
	}
	
	//// Draw ball
	//SDL_Rect ball{	
	//	static_cast<int>(mBallPos.m_x - thickness/2),
	//	static_cast<int>(mBallPos.m_y - thickness/2),
	//	thickness,
	//	thickness
	//};
	//SDL_RenderFillRect(mRenderer, &ball);

	//// Draw ball
	//SDL_Rect ball2{
	//	static_cast<int>(mBallPos2.m_x - thickness / 2),
	//	static_cast<int>(mBallPos2.m_y - thickness / 2),
	//	thickness,
	//	thickness
	//};
	//SDL_RenderFillRect(mRenderer, &ball2);

	// Draw ball
	for (auto& ball_info : m_ball_info)
	{
		auto& ball_pos = ball_info.m_ballPos;
		auto& ball_vel = ball_info.m_ballVel;

		SDL_Rect ball{
		static_cast<int>(ball_pos.m_x - thickness / 2),
		static_cast<int>(ball_pos.m_y - thickness / 2),
		thickness,
		thickness
		};
		SDL_RenderFillRect(mRenderer, &ball);
	}


	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
