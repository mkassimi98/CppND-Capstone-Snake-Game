#ifndef SNAKE_H
#define SNAKE_H

#include "SDL.h"
#include <deque>

/**
 * @brief Manages the behavior of the snake in the game, including movement, growth, and collision detection.
 */
class Snake {
 public:
  /**
   * @brief Enumeration to define possible movement directions of the snake.
   */
  enum class Direction { kUp, kDown, kLeft, kRight };

  /**
   * @brief Construct a new Snake object positioned at the center of the grid.
   * 
   * @param grid_width The width of the game grid.
   * @param grid_height The height of the game grid.
   */
  Snake(int grid_width, int grid_height);

  /**
   * @brief Update the snake's position and check for collisions.
   */
  void Update();

  /**
   * @brief Increase the size of the snake by one segment.
   */
  void GrowBody();

  /**
   * @brief Check if a cell is occupied by the snake.
   * 
   * @param x The x-coordinate of the cell to check.
   * @param y The y-coordinate of the cell to check.
   * @return true if the cell is occupied by the snake.
   * @return false otherwise.
   */
  bool SnakeCell(int x, int y);

  /**
   * @brief Reset the snake to its initial state at the start of a new game.
   */
  void Reset();

  Direction direction = Direction::kUp; ///< Initial movement direction of the snake.

  float speed{0.2f}; ///< Speed of the snake, affects how quickly it moves.
  int size{1};       ///< Current size of the snake.
  bool alive{true};  ///< Status of the snake, alive or dead.
  float head_x;      ///< x-coordinate of the snake's head.
  float head_y;      ///< y-coordinate of the snake's head.
  std::deque<SDL_Point> body; ///< Deque storing the positions of the snake's segments.

 private:
  /**
   * @brief Update the position of the snake's head based on its direction.
   */
  void UpdateHead();

  /**
   * @brief Update the positions of the body segments following the head.
   * 
   * @param current_cell The current cell to update, usually starting with the head.
   * @param prev_cell The previous cell's position.
   */
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false}; ///< Flag to determine whether the snake should grow.
  int grid_width;      ///< Width of the game grid.
  int grid_height;     ///< Height of the game grid.
};

#endif // SNAKE_H
