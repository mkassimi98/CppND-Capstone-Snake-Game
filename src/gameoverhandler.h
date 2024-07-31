#ifndef GAME_OVER_HANDLER_H
#define GAME_OVER_HANDLER_H

#include "SDL.h"
#include <string>

/**
 * @brief Handles the game over scenario, providing an interface to show a game over message box.
 */
class GameOverHandler {
public:
    /**
     * @brief Construct a new GameOverHandler object.
     */
    GameOverHandler();

    /**
     * @brief Destroy the GameOverHandler object.
     */
    ~GameOverHandler();

    /**
     * @brief Displays a game over message box with the player's score and an option to play again.
     * 
     * @param score Constant reference to an integer representing the player's final score.
     * @return true if the player chooses to play again.
     * @return false if the player chooses not to play again.
     */
    bool ShowGameOverMessage(const int& score) const;

private:
    static constexpr int messageBoxFlags = SDL_MESSAGEBOX_INFORMATION;  ///< SDL message box flag set to show information.
};

#endif // GAME_OVER_HANDLER_H
