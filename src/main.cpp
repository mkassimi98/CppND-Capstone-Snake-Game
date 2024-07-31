#include <iostream>
#include <memory>
#include "controller.h"
#include "game.h"
#include "renderer.h"

/**
 * @brief Entry point for the Snake game application.
 * 
 * Initializes game components including the renderer, controller, and game logic,
 * then runs the game loop and displays the final score and snake size upon termination.
 * 
 * @return int Returns 0 to signal normal termination of the program.
 */
int main() {
  // Frame rate configuration.
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond}; // Milliseconds per frame.

  // Screen and grid configuration.
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  // Create renderer and controller objects using smart pointers for automatic resource management.
  std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  std::unique_ptr<Controller> controller = std::make_unique<Controller>();
  
  // Initialize the game with grid dimensions.
  Game game(kGridWidth, kGridHeight);

  // Run the game loop until termination.
  game.Run(std::move(controller), std::move(renderer), kMsPerFrame);

  // Output final score and snake size when the game terminates.
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";

  return 0;
}
