#include "registry.h"

#include <stdlib.h>
#include <string.h>

struct game_registry_entry {
        const char *name;
        void *func;
};

struct game_registry_entry *entries;
size_t entries_size;
size_t entries_capacity;

static int game_registry_grow(void)
{
        size_t new_size = entries_capacity ? 2 * entries_capacity : 16;
        if (new_size <= entries_capacity) {
                return 0;
        }

        struct game_registry_entry *new_entries =
                realloc(entries, new_size * sizeof(*entries));
        if (!new_entries) {
                return -1;
        }
        entries = new_entries;
        entries_capacity = new_size;
        return 0;
}

int game_registry_add(const char *name, void *data)
{
        if (!name || !data) {
                return -1;
        }

        if (game_registry_grow() == -1) {
                return -1;
        }

        if (game_registry_get(name) != NULL) {
                return -1;
        }

        entries[entries_size].name = name;
        entries[entries_size].func = data;
        entries_size++;
        return 0;
}

void *game_registry_get(const char *name)
{
        if (!name) {
                return NULL;
        }

        for (size_t i = 0; i < entries_size; i++) {
                if (strcmp(entries[i].name, name) == 0) {
                        return entries[i].func;
                }
        }
        return NULL;
}