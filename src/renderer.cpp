#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);
  DrawFood(food);
  DrawSnake(snake);
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::DrawFood(const SDL_Point &food) {
    SDL_Rect block = {
        static_cast<int>(food.x * (screen_width / grid_width)), // Cast to int
        static_cast<int>(food.y * (screen_height / grid_height)), // Cast to int
        static_cast<int>(screen_width / grid_width), // Cast to int
        static_cast<int>(screen_height / grid_height) // Cast to int
    };
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
    SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::DrawSnake(const Snake &snake) {
  SDL_Rect block = {
        0, 0,
        static_cast<int>(screen_width / grid_width),  // Explicitly cast the division result to int
        static_cast<int>(screen_height / grid_height) // Explicitly cast the division result to int
    };
  for (const SDL_Point &point : snake.body) {
    block.x = point.x * (screen_width / grid_width);
    block.y = point.y * (screen_height / grid_height);
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF); // White for body
    SDL_RenderFillRect(sdl_renderer, &block);
  }
  // Head in blue or red if dead
  block.x = static_cast<int>(snake.head_x) * (screen_width / grid_width);
  block.y = static_cast<int>(snake.head_y) * (screen_height / grid_height);
  SDL_SetRenderDrawColor(sdl_renderer, 
                         snake.alive ? 0x00 : 0xFF, // Red component
                         snake.alive ? 0x7A : 0x00, // Green component
                         snake.alive ? 0xCC : 0x00, // Blue component
                         0xFF); // Alpha component is always full
  SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
