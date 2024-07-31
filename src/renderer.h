#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>
#include "SDL.h"
#include "SDL_image.h"
#include "snake.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food);
  void UpdateWindowTitle(int score, int fps);

 private:
  void DrawFood(const SDL_Point &food);
  void DrawSnake(const Snake &snake);

  std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> sdl_window;
  std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> sdl_renderer;
  std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> background_texture;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif
