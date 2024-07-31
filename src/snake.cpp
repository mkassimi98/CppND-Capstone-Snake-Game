#include "snake.h"
#include <cmath>
#include <iostream>

/**
 * @brief Construct a new Snake object centered in the game grid.
 * 
 * @param grid_width The width of the game grid.
 * @param grid_height The height of the game grid.
 */
Snake::Snake(int grid_width, int grid_height)
  : head_x(grid_width / 2),
    head_y(grid_height / 2),
    grid_width(grid_width),
    grid_height(grid_height) {}

/**
 * @brief Updates the state of the snake, moving it according to its current direction and speed.
 * 
 * This function also handles the movement of the snake's body by updating each segment to follow the head.
 */
void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell before moving.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after moving.

  // Only update the body if the snake has moved to a new cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

/**
 * @brief Moves the snake's head based on its direction and speed.
 * 
 * Ensures that the snake wraps around the game grid if it moves beyond the grid boundaries.
 */
void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the snake around to the opposite side if it goes off the grid.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

/**
 * @brief Updates the body segments of the snake to follow the head's movement.
 * 
 * Also checks for collisions with itself, which would set the snake's alive status to false.
 * 
 * @param current_head_cell The current cell of the head after moving.
 * @param prev_head_cell The previous cell of the head before moving.
 */
void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to the beginning of the deque.
  body.push_front(prev_head_cell);

  if (!growing) {
    // Remove the tail segment if not growing.
    body.pop_back();
  } else {
    // If growing, do not remove the tail segment and increase the size.
    growing = false;
    size++;
  }

  // Check for collision with itself.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

/**
 * @brief Resets the snake to its initial state for a new game.
 */
void Snake::Reset() {
  head_x = grid_width / 2;
  head_y = grid_height / 2;
  body.clear();
  size = 1;
  alive = true;
  growing = false;
  speed = 0.2f;
  direction = Direction::kUp;
}

/**
 * @brief Initiates the growth process of the snake, increasing its size after the next move.
 */
void Snake::GrowBody() { growing = true; }

/**
 * @brief Checks if a specific grid cell is occupied by a part of the snake's body.
 * 
 * This method can be optimized for performance in larger grids or longer snake sizes.
 * 
 * @param x The x-coordinate of the cell to check.
 * @param y The y-coordinate of the cell to check.
 * @return true If the cell is occupied by the snake.
 * @return false If the cell is not occupied by the snake.
 */
bool Snake::SnakeCell(int x, int y) {
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}
