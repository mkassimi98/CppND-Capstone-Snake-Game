#include "game.h"
#include <iostream>
#include "SDL.h"

/**
 * @brief Constructs a new Game object and initializes the game state including the snake, food placement, and RNG.
 * 
 * @param grid_width Width of the game grid.
 * @param grid_height Height of the game grid.
 */
Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      gameOverHandler(std::make_unique<GameOverHandler>()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)), 
      engine(dev()),
      running(true) {
  PlaceFood();
}

/**
 * @brief Destroys the Game object and performs cleanup by ensuring all threads are joined and calling Cleanup.
 */
Game::~Game() {
  if (gameOverThread.joinable())
    gameOverThread.join();
  
  Cleanup();
}

/**
 * @brief Cleans up the SDL resources before application shutdown, called by the destructor.
 */
void Game::Cleanup() {
  SDL_Quit();
}

/**
 * @brief Runs the main game loop which includes handling input, updating game state, and rendering.
 * 
 * Manages frame timing, input processing, game updates, and rendering within a controlled loop. Handles thread creation
 * for game over processing to ensure responsive UI even when ending game conditions are met.
 * 
 * @param controller Unique pointer to the controller managing user input.
 * @param renderer Unique pointer to the renderer for displaying the game.
 * @param target_frame_duration Desired duration of each frame in milliseconds to control game speed.
 */
void Game::Run(std::unique_ptr<Controller> controller, std::unique_ptr<Renderer> renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  while (running) {
    frame_start = SDL_GetTicks();

    // Handle user input, update game state, and render the game.
    controller->HandleInput(running, snake);
    Update();
    renderer->Render(snake, food);

    frame_end = SDL_GetTicks();
    frame_count++;
    frame_duration = frame_end - frame_start;

    // Update the window title with score and frame rate every second.
    if (frame_end - title_timestamp >= 1000) {
      renderer->UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // Delay to maintain the target frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }

    // Check if the snake is alive and a game over thread is not already running.
    if (!snake.alive && !gameOverThread.joinable()) {
      gameOverThread = std::thread(&Game::HandleGameOver, this);
    }
    if (gameOverThread.joinable()) {
      gameOverThread.join();
    }
  }
}

/**
 * @brief Places food at a random location on the grid that is not occupied by the snake.
 */
void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

/**
 * @brief Updates the state of the game by moving the snake and handling food consumption.
 */
void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  // Check if the snake's head is at the same position as the food.
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

/**
 * @brief Resets the game to its initial state for a new game, resetting score, snake size, and food placement.
 */
void Game::ResetGame() {
    snake.Reset();
    score = 0;
    PlaceFood();
}

/**
 * @brief Handles the game over logic in a separate thread to ensure the main game loop remains responsive.
 * Utilizes mutex locking to safely update and check game state variables shared with the main thread.
 */
void Game::HandleGameOver() {
  std::lock_guard<std::mutex> lock(mtx);
  if (gameOverHandler->ShowGameOverMessage(score)) {
    ResetGame();
  } else {
    running = false;
  }
}

/**
 * @brief Returns the current score of the game.
 * @return int Current game score.
 */
int Game::GetScore() const { return score; }

/**
 * @brief Returns the size of the snake.
 * @return int Number of segments in the snake.
 */
int Game::GetSize() const { return snake.size; }
