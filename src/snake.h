#ifndef SNAKE_H
#define SNAKE_H

#include "SDL.h"
#include <deque>
#include <mutex>

/**
 * @brief Manages the behavior of the snake in the game, including movement, growth, and collision detection.
 * This class encapsulates all attributes and behaviors of the snake, such as its position, size, speed, and movement mechanics.
 */
class Snake {
public:
  /**
   * @brief Enumeration to define possible movement directions of the snake.
   */
  enum class Direction { kUp, kDown, kLeft, kRight };

  /**
   * @brief Construct a new Snake object positioned at the center of the grid.
   * Initializes the snake in the middle of the grid, setting the initial direction, speed, and setting the snake as alive.
   * 
   * @param grid_width The width of the game grid.
   * @param grid_height The height of the game grid.
   */
  Snake(int grid_width, int grid_height);

  /**
   * @brief Update the snake's position based on elapsed time and check for collisions.
   * This function adjusts the snake's position according to its speed and direction while checking for collisions
   * with the game boundaries or itself.
   * 
   * @param elapsed_time The time elapsed since the last update, used for frame-rate independent movement.
   */
  void Update(float elapsed_time);

  /**
   * @brief Increase the size of the snake by one segment.
   * This function is called when the snake eats food and needs to grow. It adjusts the snake's size and ensures
   * the growth effect is reflected in the game.
   */
  void GrowBody();

  /**
   * @brief Check if a cell is occupied by the snake.
   * Used primarily for placing food on the grid to ensure it does not appear on the snake.
   * 
   * @param x The x-coordinate of the cell to check.
   * @param y The y-coordinate of the cell to check.
   * @return true if the cell is occupied by the snake.
   * @return false otherwise.
   */
  bool SnakeCell(int x, int y);

  /**
   * @brief Reset the snake to its initial state at the start of a new game.
   * Resets the snake's attributes including its size, speed, and position to their initial values.
   */
  void Reset();

  /**
   * @brief Increase the speed of the snake.
   * This function increases the snake's speed, typically called when the snake consumes food to increase the game's difficulty.
   */
  void IncreaseSpeed();

  Direction direction = Direction::kUp; ///< Initial movement direction of the snake.
  float speed{10.0f}; ///< Speed of the snake, affects how quickly it moves across the grid.
  int size{1};        ///< Current size of the snake, increased by consuming food.
  bool alive{true};   ///< Status of the snake, alive or dead.
  float head_x;       ///< x-coordinate of the snake's head.
  float head_y;       ///< y-coordinate of the snake's head.
  std::deque<SDL_Point> body; ///< Deque storing the positions of the snake's segments, used for rendering and collision detection.

  std::mutex snake_mutex; ///< Mutex to ensure thread-safe updates to the snake's state.

private:
  /**
   * @brief Update the position of the snake's head based on its direction and elapsed time.
   * Calculates the new position of the snake's head considering its current speed and the time elapsed to ensure smooth movement.
   */
  void UpdateHead(float elapsed_time);

  /**
   * @brief Update the positions of the body segments following the head.
   * Adjusts the positions of the snake's body segments to follow the head, handling the mechanics of the snake's movement.
   * 
   * @param current_cell The current cell of the head after moving.
   * @param prev_cell The previous cell's position, typically the old head position.
   */
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false}; ///< Flag to determine whether the snake should grow during the next update cycle.
  int grid_width;      ///< Width of the game grid, used for boundary checking.
  int grid_height;     ///< Height of the game grid, used for boundary checking.
};

#endif // SNAKE
