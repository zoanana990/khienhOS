#include <fs/parser.h>
#include <memory/memory.h>
#include <memory/kheap.h>

static s32 path_valid_format(const s8 *filename)
{
    s32 len = strnlen(filename, KHIENHOS_MAX_PATH);
    return (len > 3 && is_digit(filename[0])) && memcmp((void *)&filename[1], ":/", 2) == 0;
}

/**
 * @func: path_parser_get_drive_by_path
 * @description:
 *      This function is used to
 * */
static s32 path_parser_get_drive_by_path(const s8 **path)
{
    if(!path_valid_format(*path))
    {
        return -kerr_BADPATH;
    }

    /* extract the data from 0:/text.txt to text.txt*/
    s32 drive_no = to_numeric_digit(*path[0]);

    *path += 3;
    return drive_no;
}

/* split the path string */
static path_root_t *path_parser_create_root(s32 device_number)
{
    path_root_t *path_r = kzalloc(sizeof(path_root_t));
    if(path_r == NULL)
    {
        return NULL;
    }
    path_r->drive_no = device_number;
    path_r->head = NULL;

    return path_r;
}

/**
 * This function is aims to avoid the forward slash in the path
 * e.g. a path is /home/khienh/CODE/khienhOS
 * and we want to avoid the "/" so that the path is
 * home, khienh, CODE, khienhOS
 * so use these string to access and find the files and directories
 * */
static const s8* path_parser_get_path_part(const s8 **path)
{
    s8 *result_path_part = kzalloc(KHIENHOS_MAX_PATH);
    if(result_path_part == NULL)
    {
        return NULL;
    }

    s32 i = 0;
    while(**path != '/' && **path != 0x00)
    {
        result_path_part[i] = **path;
        *path += 1;
        i++;
    }

    /* skip the forward slash to avoid problems */
    if(**path != '/')
    {
        *path += 1;
    }

    if(i == 0)
    {
        kfree(result_path_part);
        result_path_part = NULL;
    }

    return result_path_part;
}

/**
 * This function: path_parser_path_part is linked each string
 * which are split by the path_parser_get_path_part
 * */
path_t *path_parser_path_part(path_t *last, const s8 **path)
{
    const s8* path_part_string = path_parser_get_path_part(path);

    if(!path_part_string)
        return NULL;

    path_t *part = kzalloc(sizeof(path_t));

    part->part = path_part_string;
    part->next = NULL;

    if(last)
        last->next = part;

    return part;
}

/**
 * Free all memory in string
 */
void path_parser_free(path_root_t* root)
{
    path_t *part = root->head;

    while(part)
    {
        path_t *next = part->next;
        kfree((void *) part->part);
        kfree(part);
        part = next;
    }

    kfree(root);
}

path_root_t* path_parser_parse(const s8* path, const s8* current_directory_path)
{
    s32 no_drive;

    const s8* tmp_path = path;
    path_root_t *path_root = NULL;

    /* check the path length */
    if(strlen(path) > KHIENHOS_MAX_PATH)
    {
        goto out;
    }

    /* get the device drive number */
    no_drive = path_parser_get_drive_by_path(&tmp_path);
    if(no_drive < 0)
    {
        path_root = NULL;
        goto out;
    }

    /* create a root path e.g. 0:/, 1:/ etc.
     *
     * take the 0:/hello.txt as example:
     * so that the root path will be
     * drive_no = 0
     * head = NULL;
     * */
    path_root = path_parser_create_root(no_drive);
    if(path_root == NULL)
    {
        goto out;
    }

    /* create a path, used a linked list to connect the path string
     *
     * the current path is hello.txt
     * */
    path_t *first_part = path_parser_path_part(NULL, &tmp_path);
    if(!first_part)
    {
        goto out;
    }

    path_root->head = first_part;
    path_t *part = path_parser_path_part(first_part, &tmp_path);
    while(part)
    {
        part = path_parser_path_part(part, &tmp_path);
    }

    out:
    return path_root;
}