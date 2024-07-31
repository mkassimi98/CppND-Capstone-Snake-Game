#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

/**
 * @brief Changes the direction of the snake to the input direction unless it's the opposite of the current direction.
 * 
 * The snake's direction will only change if the input direction is not the opposite of the current direction,
 * except when the snake has only one segment, in which case it can turn back on itself.
 * 
 * @param snake Reference to the Snake object whose direction needs to be changed.
 * @param input The desired new direction for the snake.
 * @param opposite The direction opposite to the current direction of the snake.
 */
void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) {
    snake.direction = input;
  }
  return;
}

/**
 * @brief Processes input events from SDL and applies game controls.
 * 
 * This method polls for SDL events and processes them to control the game state and the snake's direction.
 * Key events for arrow keys will change the snake's direction, and the SDL_QUIT event will stop the game.
 * 
 * @param running Reference to a boolean that indicates whether the game is still running.
 * @param snake Reference to the Snake object which is controlled by keyboard input.
 */
void Controller::HandleInput(bool &running, Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;
      }
    }
  }
}
