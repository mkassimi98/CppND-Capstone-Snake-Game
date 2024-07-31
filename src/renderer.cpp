#include "renderer.h"
#include <iostream>
#include <string>

/**
 * @brief Constructs a new Renderer object and initializes SDL components like the window, renderer, and textures.
 * 
 * @param screen_width Width of the screen.
 * @param screen_height Height of the screen.
 * @param grid_width Width of the game grid.
 * @param grid_height Height of the game grid.
 */
Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height),
      sdl_window(nullptr, SDL_DestroyWindow),
      sdl_renderer(nullptr, SDL_DestroyRenderer),
      background_texture(nullptr, SDL_DestroyTexture) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  sdl_window.reset(SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, screen_width,
                                    screen_height, SDL_WINDOW_SHOWN));
  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  sdl_renderer.reset(SDL_CreateRenderer(sdl_window.get(), -1, SDL_RENDERER_ACCELERATED));
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  SDL_Surface *background_surface = IMG_Load("../resources/background.jpg");
  if (!background_surface) {
    std::cerr << "Background image could not be loaded. IMG_Error: " << IMG_GetError() << "\n";
    std::exit(EXIT_FAILURE);
  }
  background_texture.reset(SDL_CreateTextureFromSurface(sdl_renderer.get(), background_surface));
  SDL_FreeSurface(background_surface);
}

/**
 * @brief Destroys the Renderer object, cleaning up SDL resources.
 */
Renderer::~Renderer() {
  SDL_Quit();
}

/**
 * @brief Render the game state including the snake and food.
 * 
 * Sets the background color, clears the current rendering target, draws the food and snake,
 * and updates the rendering to the screen.
 * 
 * @param snake Constant reference to the Snake object.
 * @param food Constant reference to the SDL_Point object representing food's location.
 */
void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_SetRenderDrawColor(sdl_renderer.get(), 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer.get());
  SDL_RenderCopy(sdl_renderer.get(), background_texture.get(), NULL, NULL);
  DrawFood(food);
  DrawSnake(snake);
  SDL_RenderPresent(sdl_renderer.get());
}

/**
 * @brief Draws the food on the grid.
 * 
 * @param food Constant reference to the SDL_Point object representing the food's location.
 */
void Renderer::DrawFood(const SDL_Point &food) {
    SDL_Rect block = {
        static_cast<int>(food.x * (screen_width / grid_width)),
        static_cast<int>(food.y * (screen_height / grid_height)),
        static_cast<int>(screen_width / grid_width),
        static_cast<int>(screen_height / grid_height)
    };
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xCC, 0x00, 0xFF);
    SDL_RenderFillRect(sdl_renderer.get(), &block);
}

/**
 * @brief Draws the snake on the grid.
 * 
 * Renders each segment of the snake's body and changes the color of the snake's head
 * based on whether it is alive or dead.
 * 
 * @param snake Constant reference to the Snake object.
 */
void Renderer::DrawSnake(const Snake &snake) {
  SDL_Rect block = {
        0, 0,
        static_cast<int>(screen_width / grid_width),
        static_cast<int>(screen_height / grid_height)
    };
  for (const SDL_Point &point : snake.body) {
    block.x = point.x * (screen_width / grid_width);
    block.y = point.y * (screen_height / grid_height);
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(sdl_renderer.get(), &block);
  }

  block.x = static_cast<int>(snake.head_x) * (screen_width / grid_width);
  block.y = static_cast<int>(snake.head_y) * (screen_height / grid_height);
  SDL_SetRenderDrawColor(sdl_renderer.get(), 
                         snake.alive ? 0x00 : 0xFF,
                         snake.alive ? 0x7A : 0x00,
                         snake.alive ? 0xCC : 0x00,
                         0xFF);
  SDL_RenderFillRect(sdl_renderer.get(), &block);
}

/**
 * @brief Updates the window title with the current score and frames per second.
 * 
 * @param score Current game score.
 * @param fps Current frames per second.
 */
void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window.get(), title.c_str());
}
