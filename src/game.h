#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "gameoverhandler.h"

/**
 * @brief Manages the main game loop, interactions, and state management for a snake game.
 * 
 * This class is responsible for orchestrating the core components of the game such as the snake mechanics, 
 * food generation, and handling game over scenarios. It employs unique pointers for resource management of controllers,
 * renderers, and game over handlers to ensure proper cleanup. The game leverages multithreading to manage game state updates
 * and game over logic independently, enhancing responsiveness and performance.
 */
class Game {
public:
  /**
   * @brief Constructs a new Game object with specified grid dimensions.
   * 
   * This initializes all game components, sets the game state, and starts the thread responsible for updating
   * the snake's movement and game logic continuously.
   * 
   * @param grid_width Width of the game grid.
   * @param grid_height Height of the game grid.
   */
  Game(std::size_t grid_width, std::size_t grid_height);

  /**
   * @brief Destroys the Game object and ensures all resources are properly released and all threads are terminated.
   * 
   * This includes joining the game over and snake update threads to prevent any potential resource leakage.
   */
  ~Game();

  /**
   * @brief Runs the main game loop which includes handling user input, updating the game state, and rendering graphics.
   * 
   * The loop continues to execute until an exit condition is met, and it ensures the game updates and renders 
   * are performed at a consistent and controlled frame rate.
   * 
   * @param controller Unique pointer to the Controller object managing user input.
   * @param renderer Unique pointer to the Renderer object for displaying the game state.
   * @param target_frame_duration Duration of each frame in milliseconds to maintain a stable frame rate.
   */
  void Run(std::unique_ptr<Controller> controller, std::unique_ptr<Renderer> renderer,
           std::size_t target_frame_duration);

  /**
   * @brief Retrieves the current score of the game.
   * 
   * @return int Current score based on the number of foods consumed by the snake.
   */
  int GetScore() const;

  /**
   * @brief Retrieves the current size of the snake, measured in the number of segments.
   * 
   * @return int Number of segments in the snake.
   */
  int GetSize() const;

private:
  Snake snake; ///< Handles the behavior and state of the snake.
  std::unique_ptr<GameOverHandler> gameOverHandler; ///< Manages game over scenarios.
  std::thread gameOverThread; ///< Thread for processing game over logic asynchronously.
  std::mutex mtx; ///< Mutex for synchronizing access to shared resources.
  SDL_Point food; ///< Current position of the food on the grid.
  std::random_device dev; ///< Device used to generate seeds for the random number generator.
  std::uniform_int_distribution<int> random_w; ///< Distribution for randomizing food's horizontal position.
  std::uniform_int_distribution<int> random_h; ///< Distribution for randomizing food's vertical position.
  std::mt19937 engine; ///< Random number generator.
  std::condition_variable cv; ///< Condition variable for synchronizing the snake update thread.


  std::unique_ptr<std::thread> snakeThread; ///< Thread for continuously updating the game state.

  bool running{true}; ///< Indicates whether the game loop is active.

  int score{0}; ///< Tracks the number of points scored by the player.

  void ThreadedUpdate(); ///< Updates the game state in a dedicated thread.

  /**
   * @brief Randomly places food on the grid where it is not occupied by the snake.
   */
  void PlaceFood();

  /**
   * @brief Cleans up game resources upon shutdown, ensuring a clean exit.
   */
  void Cleanup();

  /**
   * @brief Resets the game to its initial state for a new session.
   * 
   * This is typically invoked after a game over to restart the game with initial settings.
   */
  void ResetGame();

  /**
   * @brief Manages the game over process in a separate thread to avoid blocking the main game loop.
   * 
   * It securely checks game state and handles user decisions to either restart or exit.
   */
  void HandleGameOver();
};

#endif // GAME_H
