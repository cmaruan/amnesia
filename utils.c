#include "utils.h"

#include <string.h>

int readline(FILE *stream, char *buffer, size_t *size)
{
        if (!stream || !buffer || !size || *size == 0) {
                return -1;
        }

        if (fgets(buffer, *size, stream) == NULL) {
                return -1;
        }
        size_t len = strlen(buffer);
        if (len == 0) {
                return -1;
        }
        if (buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
                len--;
        }
        *size = len;
        return 0;
}