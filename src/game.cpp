#include "game.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "SDL.h"

/**
 * @brief Constructs a new Game object and initializes the game state including the snake, food placement, and RNG.
 * 
 * Initializes threads and game components necessary to start the game loop, manages food placement, and sets up RNG for food randomization.
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
  // Initiates the thread that handles snake updates based on a fixed time interval.
  snakeThread = std::make_unique<std::thread>(&Game::ThreadedUpdate, this);
}

/**
 * @brief Destroys the Game object and performs cleanup by ensuring all threads are properly joined and calling Cleanup.
 * 
 * Ensures that all resources are properly released and all threads are terminated safely before the game object is destroyed.
 */
Game::~Game() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    running = false;
    cv.notify_all(); // Notify the thread to stop waiting and exit
  }

  if (snakeThread && snakeThread->joinable()) {
    snakeThread->join();
  }

  if (gameOverThread.joinable()) {
    gameOverThread.join();
  }

  Cleanup();
}

/**
 * @brief Cleans up the SDL resources before application shutdown, called by the destructor.
 * 
 * This function is responsible for releasing all SDL-related resources to prevent resource leaks.
 */
void Game::Cleanup() {
  SDL_Quit();
}

/**
 * @brief Runs the main game loop which includes handling input, updating game state, and rendering.
 * 
 * This function is the core of the game's execution cycle, managing everything from user input to rendering graphics on the screen.
 * It also ensures the game loop runs at a specified frame rate, making adjustments as necessary to maintain smooth gameplay.
 * 
 * @param controller Unique pointer to the Controller object managing user input.
 * @param renderer Unique pointer to the Renderer object for displaying the game.
 * @param target_frame_duration Desired duration of each frame in milliseconds to control game speed.
 */
void Game::Run(std::unique_ptr<Controller> controller, std::unique_ptr<Renderer> renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  int frame_count = 0;

  while (running) {
      Uint32 frame_start = SDL_GetTicks();

      controller->HandleInput(running, snake);
      renderer->Render(snake, food);

      Uint32 frame_end = SDL_GetTicks();
      frame_count++;
      Uint32 frame_duration = frame_end - frame_start;

      if (frame_end - title_timestamp >= 1000) {
          renderer->UpdateWindowTitle(score, frame_count);
          frame_count = 0;
          title_timestamp = frame_end;
      }

      if (frame_duration < target_frame_duration) {
          SDL_Delay(target_frame_duration - frame_duration);
      }

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
 * 
 * This method ensures that the food does not appear on any part of the snake's body.
 */
void Game::PlaceFood() {
  int x, y;
  do {
    x = random_w(engine);
    y = random_h(engine);
  } while (snake.SnakeCell(x, y));

  food.x = x;
  food.y = y;
}

/**
 * @brief Resets the game to its initial state for a new game, resetting score, snake size, and food placement.
 * 
 * This function also safely stops and restarts the update thread, ensuring that all game components are correctly
 * initialized for a new game session.
 */
void Game::ResetGame() {
    {
      running = false;
      cv.notify_all(); // Notify the thread to stop waiting and exit
    }

    if (snakeThread && snakeThread->joinable()) {
        snakeThread->join();
    }

    snake.Reset();
    score = 0;
    PlaceFood();

    running = true;
    snake.alive = true;

    snakeThread = std::make_unique<std::thread>(&Game::ThreadedUpdate, this);
}

/**
 * @brief Handles the game over logic in a separate thread to ensure the main game loop remains responsive.
 * 
 * This method uses mutexes to safely update and check game state variables shared with the main thread, allowing
 * for a smooth transition between game over and restart scenarios.
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

/**
 * @brief The function updates the snake based on elapsed time to ensure smooth movement across varying frame rates.
 * 
 * This thread is crucial for maintaining consistent game physics and responsiveness by adjusting the snake's position
 * based on the time elapsed since the last update.
 */
void Game::ThreadedUpdate() {
  auto lastUpdateTime = std::chrono::steady_clock::now();

  while (running && snake.alive) {
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait_for(lock, std::chrono::milliseconds(10), [this]() { return !running || !snake.alive; });
      
      if (!running || !snake.alive) break;

      auto currentTime = std::chrono::steady_clock::now();
      float elapsed_time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastUpdateTime).count();
      lastUpdateTime = currentTime;

      snake.Update(elapsed_time);

      int new_x = static_cast<int>(snake.head_x);
      int new_y = static_cast<int>(snake.head_y);
      if (food.x == new_x && food.y == new_y) {
          score++;
          PlaceFood();
          snake.GrowBody();
          snake.IncreaseSpeed();
      }
    }
}