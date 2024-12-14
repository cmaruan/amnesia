#ifndef GAME_REGISTRY_H
#define GAME_REGISTRY_H

#define GAME_REGISTRY_FUNCTION __attribute__((constructor))

int game_registry_add(const char *name, void *data);
void *game_registry_get(const char *name);

#endif /* GAME_REGISTRY_H */