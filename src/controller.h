#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

/**
 * @brief Class to handle input controls for the snake game.
 * 
 * This class provides functionality to handle user input and control the 
 * direction of the snake based on that input.
 */
class Controller {
 public:
  /**
   * @brief Handle user input for controlling the snake and the game state.
   * 
   * @param running Reference to a boolean that represents the game's running state.
   *                This is used to stop the game when the user inputs a quit command.
   * @param snake Reference to the Snake object which is being controlled by the input.
   */
  void HandleInput(bool &running, Snake &snake) const;

 private:
  /**
   * @brief Change the direction of the snake based on user input.
   * 
   * This method ensures that the snake does not reverse on itself and only changes
   * direction to valid new directions.
   * 
   * @param snake Reference to the Snake object whose direction is to be changed.
   * @param input The new direction inputted by the user.
   * @param opposite The opposite direction of the current direction of the snake.
   *                 Used to prevent the snake from reversing on itself.
   */
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif // CONTROLLER_H
