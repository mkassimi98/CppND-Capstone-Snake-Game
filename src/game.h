#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
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
   * @brief Destroy the Game object and perform any necessary cleanup.
   */
  ~Game();

  /**
   * @brief Runs the game loop, handling updates and rendering at a controlled frame rate.
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
  SDL_Point food;
  std::random_device dev;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::mt19937 engine;

  int score{0};

  /**
   * @brief Places food on a random location within the game grid.
   */
  void PlaceFood();

  /**
   * @brief Updates the state of the game for each frame.
   */
  void Update();

  /**
   * @brief Performs cleanup tasks, generally used before game shutdown.
   */
  void Cleanup();

  /**
   * @brief Resets the game to its initial state.
   */
  void ResetGame();
};

#endif // GAME_H
