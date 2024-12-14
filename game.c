#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "game.h"
#include "logging.h"
#include "registry.h"
#include "utils.h"

enum {
        GAME_STATE_STOPPED,
        GAME_STATE_RUNNING,
        GAME_STATE_PAUSED,
};

struct game {
        int game_state;
        FILE *in;
        FILE *out;
        const char *current_scene;
        char input[GAME_MAX_INPUT_SIZE];
        char *input_cursor;
        size_t input_size;
        const struct game_action_entry *action;
};

void game_init(struct game *game)
{
        if (!game) {
                return;
        }
        game->in = stdin;
        game->out = stdout;
        game->game_state = GAME_STATE_RUNNING;
        game->current_scene = "scene1";
}

struct game *game_create(void)
{
        struct game *game = calloc(1, sizeof(struct game));
        if (!game) {
                LERROR("Failed to allocate memory for game");
                return NULL;
        }
        game_init(game);
        return game;
}

static int game_scene_handler(struct game *game)
{
        if (!game) {
                return -1;
        }
        void *scene = game_registry_get(game->current_scene);
        if (!scene) {
                LERROR("Failed to get scene: %s", game->current_scene);
                return -1;
        }
        int (*scene_func)(struct game *game) =
                (int (*)(struct game *game))scene;
        return scene_func(game);
}

void game_shutdown(struct game *game)
{
        (void)game;
}

int game_running(struct game *game)
{
        if (!game) {
                return 0;
        }
        LTRACE("Game state: %d", game->game_state);
        return game->game_state != GAME_STATE_STOPPED;
}

int game_process_input(struct game *game)
{
        if (!game) {
                return -1;
        }

        game->input_size = GAME_MAX_INPUT_SIZE;
        game_printf(game, "\n> ");
        if (readline(game->in, game->input, &game->input_size) == -1) {
                LERROR("Failed to read input");
                return -1;
        }

        game->input_cursor = game->input;
        int action = game_action_get_next(game);
        if (action == -1) {
                LERROR("Failed to get next action");
                return -1;
        }
        LTRACE("Action: %d", action);
        // check for controlling commands here for now
        switch (action) {
        case GAME_ACTION_EXIT:
                LTRACE("User input exit");
                game->game_state = GAME_STATE_STOPPED;
                break;
        }

        return 0;
}

void game_destroy(struct game **game)
{
        if (game && *game) {
                game_shutdown(*game);
                *game = NULL;
        }
}

int game_run(struct game *game)
{
        if (!game) {
                return -1;
        }

        while (game_running(game)) {
                if (game_scene_handler(game) == -1) {
                        LERROR("Failed to handle scene");
                        return -1;
                }
        }

        return 0;
}

int game_printf(struct game *game, const char *fmt, ...)
{
        if (!game) {
                return -1;
        }
        va_list args;
        va_start(args, fmt);
        puts("");
        int ret = vfprintf(game->out, fmt, args);
        va_end(args);
        return ret;
}

int game_action_get_next(struct game *game)
{
        const struct game_action_entry *entry;

        if (!game) {
                return -1;
        }
        size_t first_word_len = strcspn(game->input_cursor, " ");
        if (first_word_len == 0) {
                return -1;
        }
        game->input_cursor[first_word_len] = '\0';

        entry = game_action_lookup(game->input_cursor, first_word_len);
        if (!entry) {
                return -1;
        }
        game->input_cursor += first_word_len;
        LTRACE("Found word: %s", game->input_cursor);
        game->input_cursor += strspn(game->input_cursor, " ");
        game->action = entry;
        return entry->action;
}

int game_action_get_current(struct game *game)
{
        if (!game) {
                return -1;
        }
        return game->action->action;
}

int game_action_has_next(struct game *game)
{
        if (!game) {
                return 0;
        }
        return strcspn(game->input_cursor, " ") > 0;
}

const char *game_action_get_text(struct game *game)
{
        if (!game) {
                return NULL;
        }
        return game->input_cursor;
}