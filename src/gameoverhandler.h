#ifndef GAME_OVER_HANDLER_H
#define GAME_OVER_HANDLER_H

#include "SDL.h"
#include <string>

class GameOverHandler {
public:
    GameOverHandler();
    ~GameOverHandler();

    // Show game over message using score by const reference
    bool ShowGameOverMessage(const int& score) const;

private:
    static constexpr int messageBoxFlags = SDL_MESSAGEBOX_INFORMATION;
};

#endif // GAME_OVER_HANDLER_H
