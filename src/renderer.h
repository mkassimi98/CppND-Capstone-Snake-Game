#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "SDL_image.h"
#include "snake.h"

/**
 * @brief Handles rendering of game elements to the screen using SDL.
 * 
 * This class is responsible for rendering the snake, food, and updating the window title
 * with the game score and frame rate. It encapsulates details about the SDL window,
 * renderer, and textures used in the game.
 */
class Renderer {
 public:
  /**
   * @brief Construct a new Renderer object.
   * 
   * Initializes SDL, sets up the window, and prepares the renderer and textures for the game.
   * 
   * @param screen_width Width of the screen.
   * @param screen_height Height of the screen.
   * @param grid_width Width of the game grid.
   * @param grid_height Height of the game grid.
   */
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);

  /**
   * @brief Destroy the Renderer object.
   * 
   * Cleans up the SDL resources, including window, renderer, and textures.
   */
  ~Renderer();

  /**
   * @brief Render the snake and food on the game window.
   * 
   * @param snake Constant reference to the Snake object to be rendered.
   * @param food Constant reference to the SDL_Point where food is located.
   */
  void Render(Snake const snake, SDL_Point const &food);

  /**
   * @brief Updates the game window title with the current score and frames per second.
   * 
   * @param score Current score in the game.
   * @param fps Frames per second currently being rendered.
   */
  void UpdateWindowTitle(int score, int fps);

 private:
  /**
   * @brief Draw food on the game grid.
   * 
   * @param food Constant reference to the SDL_Point where food is located.
   */
  void DrawFood(const SDL_Point &food);

  /**
   * @brief Draw the snake on the game grid.
   * 
   * @param snake Constant reference to the Snake object to be rendered.
   */
  void DrawSnake(const Snake &snake);
  
  const std::size_t screen_width;   ///< Width of the screen.
  const std::size_t screen_height;  ///< Height of the screen.
  const std::size_t grid_width;     ///< Width of the game grid.
  const std::size_t grid_height;    ///< Height of the game grid.

  std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> sdl_window;       ///< Smart pointer managing the SDL_Window.
  std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> sdl_renderer; ///< Smart pointer managing the SDL_Renderer.
  std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> background_texture; ///< Smart pointer for managing a texture used as the background.
};

#endif
