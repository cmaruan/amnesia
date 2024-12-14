#ifndef GAME_LOGGING_H
#define GAME_LOGGING_H

#include <stdio.h>

#define GAME_LOGGER_TRACE 0
#define GAME_LOGGER_DEBUG 1
#define GAME_LOGGER_INFO 2
#define GAME_LOGGER_WARN 3
#define GAME_LOGGER_ERROR 4
#define GAME_LOGGER_FATAL 5

#ifndef GAME_LOGGER_DEFAULT_LOG_LEVEL
#define GAME_LOGGER_DEFAULT_LOG_LEVEL GAME_LOGGER_INFO
#endif

void game_logger(int level, const char *filename, int line, const char *fmt,
                 ...);

#define LOGGER(level, ...) game_logger(level, __FILE__, __LINE__, __VA_ARGS__)
#define LTRACE(...) LOGGER(GAME_LOGGER_TRACE, __VA_ARGS__)
#define LDEBUG(...) LOGGER(GAME_LOGGER_DEBUG, __VA_ARGS__)
#define LINFO(...) LOGGER(GAME_LOGGER_INFO, __VA_ARGS__)
#define LWARN(...) LOGGER(GAME_LOGGER_WARN, __VA_ARGS__)
#define LERROR(...) LOGGER(GAME_LOGGER_ERROR, __VA_ARGS__)

#endif /* GAME_LOGGING_H */
