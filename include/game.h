#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <stdio.h>

#define GAME_MAX_INPUT_SIZE 256

enum {
        GAME_ACTION_EXIT,
        GAME_ACTION_HELP,
        GAME_ACTION_READ,
        GAME_ACTION_OPEN,
        GAME_ACTION_EAT,
        GAME_ACTION_INSPECT,
        GAME_ACTION_PICK,
        GAME_ACTION_TALK,
        GAME_ACTION_KNOCK,
        GAME_ACTION_REFLECT,
};

struct game;

struct game_action_entry {
        const char *text;
        int action;
};
struct game *game_create(void);
void game_init(struct game *game);
void game_destroy(struct game **game);

int game_running(struct game *game);
int game_run(struct game *game);
int game_process_input(struct game *game);
int game_printf(struct game *game, const char *fmt, ...);

const struct game_action_entry *game_action_lookup(const char *word,
                                                   register unsigned int len);
int game_action_get_current(struct game *game);
int game_action_get_next(struct game *game);
int game_action_has_next(struct game *game);
const char *game_action_get_text(struct game *game);

#endif /* GAME_GAME_H */