#include "gameoverhandler.h"
#include <string>

/**
 * @brief Constructs a new GameOverHandler object.
 */
GameOverHandler::GameOverHandler() {}

/**
 * @brief Destroys the GameOverHandler object.
 */
GameOverHandler::~GameOverHandler() {}

/**
 * @brief Displays a message box with the game over message and options to continue or exit.
 * 
 * This function creates a message box displaying the player's final score and asking if they wish to play again.
 * The message box has two buttons: "Yes" and "No".
 * 
 * @param score Constant reference to an integer holding the player's score.
 * @return true If the player clicks "Yes".
 * @return false If the player clicks "No" or if an error occurs in displaying the message box.
 */
bool GameOverHandler::ShowGameOverMessage(const int& score) const {
    const std::string message = "Game Over! Your score was: " + std::to_string(score) + "\nPlay again?";
    const SDL_MessageBoxButtonData buttons[] = {
        { /* .flags, .buttonid, .text */ 0, 0, "No" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes" },
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Game Over", /* .title */
        message.c_str(), /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        NULL /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("SDL_ShowMessageBox failed: %s", SDL_GetError());
        return false;
    }
    return buttonid == 1;  // Return true if 'Yes' was clicked
}
