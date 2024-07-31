#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <thread>
#include <mutex>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "gameoverhandler.h"

/**
 * @brief Game class handles the main game loop, interactions, and state management for a snake game.
 * 
 * This class manages the game's main components such as the snake, food, and game over conditions.
 * It uses unique pointers for handling controllers and renderers, emphasizing ownership and lifecycle management.
 * Additionally, it manages a separate thread for handling game over scenarios, allowing for asynchronous
 * decision-making without interrupting the main game loop. Thread safety is ensured using a mutex.
 */
class Game {
 public:
  /**
   * @brief Construct a new Game object with specified grid dimensions.
   * 
   * @param grid_width Width of the game grid.
   * @param grid_height Height of the game grid.
   */
  Game(std::size_t grid_width, std::size_t grid_height);

  /**
   * @brief Destroy the Game object and perform any necessary cleanup, ensuring that all threads are properly joined.
   */
  ~Game();

  /**
   * @brief Runs the game loop, handling updates and rendering at a controlled frame rate.
   * 
   * This method also checks for the game over condition and may initiate a separate thread to handle
   * game over logic.
   * 
   * @param controller Unique pointer to a Controller object for handling user input.
   * @param renderer Unique pointer to a Renderer object for drawing the game state.
   * @param target_frame_duration Target duration of each frame in milliseconds to control game speed.
   */
  void Run(std::unique_ptr<Controller> controller, std::unique_ptr<Renderer> renderer,
           std::size_t target_frame_duration);

  /**
   * @brief Get the current score of the game.
   * 
   * @return int Current score.
   */
  int GetScore() const;

  /**
   * @brief Get the size of the snake in the game.
   * 
   * @return int Number of segments in the snake.
   */
  int GetSize() const;

 private:
  Snake snake;
  std::unique_ptr<GameOverHandler> gameOverHandler;
  std::thread gameOverThread;  ///< Thread used for handling game over conditions asynchronously.
  std::mutex mtx;  ///< Mutex for protecting shared resources across threads.
  SDL_Point food;
  std::random_device dev;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::mt19937 engine;

  bool running{true};  ///< Flag to control the running state of the game loop.

  int score{0};

  /**
   * @brief Places food on a random location within the game grid.
   */
  void PlaceFood();

  /**
   * @brief Updates the state of the game for each frame.
   * 
   * Includes moving the snake, checking for food consumption, and managing collisions.
   */
  void Update();

  /**
   * @brief Performs cleanup tasks, generally used before game shutdown.
   * 
   * Ensures all SDL resources are properly released.
   */
  void Cleanup();

  /**
   * @brief Resets the game to its initial state.
   * 
   * Used after a game over when starting a new game.
   */
  void ResetGame();

  /**
   * @brief Handles the game over logic in a separate thread.
   * 
   * Displays a game over message and prompts the user to play again or exit. This method is protected by a mutex.
   */
  void HandleGameOver();
};

#endif // GAME_H
