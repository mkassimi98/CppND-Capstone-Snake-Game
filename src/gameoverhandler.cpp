#include "gameoverhandler.h"
#include <string>

GameOverHandler::GameOverHandler() {}

GameOverHandler::~GameOverHandler() {}

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
        SDL_Log("error displaying message box");
        return false;
    }
    return buttonid == 1;  // Return true if 'Yes' was clicked
}