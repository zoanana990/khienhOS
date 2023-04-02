#ifndef KHIENHOS_PARSER_H
#define KHIENHOS_PARSER_H

#include <common/string.h>
#include <khienh/type.h>
#include <khienh/kernel.h>

typedef struct path
{
    const s8 *part;
    struct path *next;
} path_t;

typedef struct path_root
{
    s32 drive_no;
    path_t *head;
} path_root_t;

path_root_t* path_parser_parse(const s8* path, const s8* current_directory_path);
void path_parser_free(path_root_t* root);

#endif //KHIENHOS_PARSER_H
