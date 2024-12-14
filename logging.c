#include "logging.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *game_log_level_text[] = {
        [GAME_LOGGER_TRACE] = "TRACE", [GAME_LOGGER_DEBUG] = "DEBUG",
        [GAME_LOGGER_INFO] = "INFO",   [GAME_LOGGER_WARN] = "WARN",
        [GAME_LOGGER_ERROR] = "ERROR", [GAME_LOGGER_FATAL] = "FATAL",
};

static int game_get_logger_level(void);
static int game_logging_get_level_from_env(void);

static int game_logger_current_level;
static int (*game_logger_level)(void) = game_logging_get_level_from_env;

void game_logger(int level, const char *filename, int line, const char *fmt,
                 ...)
{
        if (level < game_logger_level()) {
                return;
        }

        va_list args;
        va_start(args, fmt);
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char buffer[4096];
        char datetime[32];
        char *ptr = buffer;
        strftime(datetime, sizeof datetime, "%Y-%m-%dT%H:%M:%S", tm);
        ptr += snprintf(ptr, sizeof buffer - (ptr - buffer),
                        "[%s] [%s:%d] [%s] ", datetime, filename, line,
                        game_log_level_text[level]);
        ptr += vsnprintf(ptr, sizeof buffer - (ptr - buffer), fmt, args);
        fprintf(stderr, "%s\n", buffer);
        va_end(args);
}

static int game_logging_get_level_from_env(void)
{
        const char *env = getenv("GAME_LOG_LEVEL");
        if (env == NULL) {
                game_logger_current_level = GAME_LOGGER_DEFAULT_LOG_LEVEL;
        } else if (strcmp(env, "TRACE") == 0) {
                game_logger_current_level = GAME_LOGGER_TRACE;
        } else if (strcmp(env, "DEBUG") == 0) {
                game_logger_current_level = GAME_LOGGER_DEBUG;
        } else if (strcmp(env, "INFO") == 0) {
                game_logger_current_level = GAME_LOGGER_INFO;
        } else if (strcmp(env, "WARN") == 0) {
                game_logger_current_level = GAME_LOGGER_WARN;
        } else if (strcmp(env, "ERROR") == 0) {
                game_logger_current_level = GAME_LOGGER_ERROR;
        } else if (strcmp(env, "FATAL") == 0) {
                game_logger_current_level = GAME_LOGGER_FATAL;
        } else {
                game_logger_current_level = GAME_LOGGER_DEFAULT_LOG_LEVEL;
        }
        game_logger_level = game_get_logger_level;
        return game_logger_current_level;
}

static int game_get_logger_level(void)
{
        return game_logger_current_level;
}
