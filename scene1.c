#include <stdio.h>

#include "game.h"
#include "logging.h"
#include "registry.h"

static int scene_handle_inspect(struct game *game)
{
        if (!game_action_has_next(game)) {
                game_printf(game, "What do you want to inspect?");
                return 0;
        }
        const char *text;
        text = game_action_get_text(game);
        game_printf(game, "You inspect the %s\n", text);

        return 0;
}

static int scene_loop(struct game *game)
{
        int rv = 0;
        while (game_process_input(game) == -1) {
                game_printf(game,
                            "Hmm.. I didn'y undertand what you mean by that. "
                            "Maybe try again?\n");
        }

        switch (game_action_get_current(game)) {
        case GAME_ACTION_INSPECT:
                rv = scene_handle_inspect(game);
                break;
        }

        return rv;
}

static int scene(struct game *game)
{
        LTRACE("Running scene1");
        if (!game) {
                LWARN("Invalid game pointer");
                return -1;
        }
        LTRACE("Printing intro");
        game_printf(
                game,
                "You awaken on the cold stone floor. Your body aches, and "
                "sharp pains shoot through your knuckles. Around you, the room "
                "is quiet but littered with signs of chaos—a dented tray near "
                "the door, scraps of torn fabric in the corner, and a bloody "
                "handprint smeared across the wall.\n"
                "\n"
                "The lightbulb above flickers weakly, casting shifting shadows "
                "across the damp stone walls.\n"
                "\n"
                "What do you want to do?\n");

        while (game_running(game)) {
                scene_loop(game);
        }

        return 0;
}

GAME_REGISTRY_FUNCTION
void self_register(void)
{
        if (game_registry_add("scene1", (void *)scene) == -1) {
                fprintf(stderr, "Failed to register scene1\n");
        }
}