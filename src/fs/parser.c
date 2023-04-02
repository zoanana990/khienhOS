#include <fs/parser.h>

static s32 path_valid_format(const s8 *filename)
{
    s32 len = strnlen(filename, KHIENHOS_MAX_PATH);
    return len;
}