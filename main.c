#include <string.h>

#include "game.h"
#include "logging.h"

int main(int argc, const char *argv[])
{
        (void)argc;
        (void)argv;
        LTRACE("Starting game engine");
        struct game *game = game_create();

        game_run(game);

        LTRACE("Shutting down game engine");
        game_destroy(&game);
        return 0;
}
