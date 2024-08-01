#include "snake.h"
#include <cmath>
#include <iostream>

/**
 * @brief Construct a new Snake object centered in the game grid.
 * Initializes the snake's head position to the center of the grid and sets up grid dimensions.
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
 * The function calculates the new position of the snake's head and updates each body segment to follow the head.
 * Also handles the snake's wrapping around the grid boundaries.
 * 
 * @param elapsed_time The time elapsed since the last update, used to make the movement frame-rate independent.
 */
void Snake::Update(float elapsed_time) {
    SDL_Point prev_cell = { static_cast<int>(head_x), static_cast<int>(head_y) };
    UpdateHead(elapsed_time); // Update the head's position based on direction and speed
    SDL_Point current_cell = { static_cast<int>(head_x), static_cast<int>(head_y) };

    if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
        UpdateBody(current_cell, prev_cell);
    }
}

/**
 * @brief Moves the snake's head based on its direction and speed.
 * 
 * Updates the head's position according to the direction and speed. Ensures the snake wraps around the grid if
 * it moves beyond the boundaries.
 * 
 * @param elapsed_time The time elapsed since the last update, affecting the movement distance.
 */
void Snake::UpdateHead(float elapsed_time) {
    // Adjust position based on direction and speed * elapsed time
    switch (direction) {
        case Direction::kUp:
            head_y -= speed * elapsed_time;
            break;
        case Direction::kDown:
            head_y += speed * elapsed_time;
            break;
        case Direction::kLeft:
            head_x -= speed * elapsed_time;
            break;
        case Direction::kRight:
            head_x += speed * elapsed_time;
            break;
    }
    // Wrap the snake around to the opposite side if it goes off the grid
    head_x = fmod(head_x + grid_width, grid_width);
    head_y = fmod(head_y + grid_height, grid_height);
}

/**
 * @brief Updates the body segments of the snake to follow the head's movement.
 * 
 * Adds the previous head location to the front of the body segments and removes the tail segment if the snake is
 * not growing. Checks for self-collision, which would end the game if detected.
 * 
 * @param current_head_cell The current cell of the head after moving.
 * @param prev_head_cell The previous cell's position before the move.
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
 * 
 * Sets the snake's head to the center of the grid, clears the body, resets the size, speed, and status to default
 * values, and sets the direction to the default "up."
 */
void Snake::Reset() {
  head_x = grid_width / 2;
  head_y = grid_height / 2;
  body.clear();
  size = 1;
  alive = true;
  growing = false;
  speed = 10.0f;
  direction = Direction::kUp;
}

/**
 * @brief Initiates the growth process of the snake, increasing its size after the next move.
 * 
 * Sets a flag to indicate that the snake should grow during the next update cycle, which results in increasing
 * the size of the snake by one segment.
 */
void Snake::GrowBody() {
  std::lock_guard<std::mutex> lock(snake_mutex);
  growing = true;
}

/**
 * @brief Checks if a specific grid cell is occupied by a part of the snake's body.
 * 
 * This method is useful for determining whether a cell is occupied by the snake, which is important for placing
 * food on the grid and checking collisions.
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

/**
 * @brief Increases the speed of the snake.
 * 
 * This function boosts the snake's speed by 10%, making the game progressively harder as the snake grows.
 */
void Snake::IncreaseSpeed() {
    speed *= 1.1;  // Increase speed by 10% of the current speed
}
