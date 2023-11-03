/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: file.c 1.6 1995/08/31 16:29:24 sam Exp $
 * $Locker:  $
 *
 * Low level wrappers for file system access
 */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "fw.h"
#include "brassert.h"

static char rscid[] = "$Id: file.c 1.6 1995/08/31 16:29:24 sam Exp $";

/*
 * Resource block used to represent an open file
 */
typedef struct br_file
{
    void *raw_file;
    br_boolean writing;
    int mode;
    char name[1];
} br_file;

/*
 * Resource destructor for files
 */
void BR_CALLBACK _BrFileFree(void *res, br_uint_8 res_class, br_size_t size)
{
    br_file *file = res;

    /*
     * Close low level file
     */
    ASSERT(fw.fsys->close != NULL);

    fw.fsys->close(((br_file *)file)->raw_file);
}

br_uint_32 BR_PUBLIC_ENTRY BrFileAttributes(void)
{
    ASSERT(fw.fsys->attributes != NULL);

    return fw.fsys->attributes();
}

void *BR_PUBLIC_ENTRY BrFileOpenRead(char *name, br_size_t n_magics, br_mode_test_cbfn *mode_test, int *mode_result)
{
    void *raw_file;
    br_file *file;

    ASSERT(fw.fsys->open_read != NULL);

    /*
     * Invoke low level file access
     */
    raw_file = fw.fsys->open_read(name, n_magics, mode_test, mode_result);

    /*
     * Catch failure
     */
    if (raw_file == NULL)
        return NULL;

    /*
     * Create a file block (which includes the name and mode)
     */
    file = BrResAllocate(fw.res, sizeof(br_file) + strlen(name), BR_MEMORY_FILE);
    file->writing = BR_FALSE;
    file->mode = *mode_result;
    file->raw_file = raw_file;
    strcpy(file->name, name);

    return file;
}

void *BR_PUBLIC_ENTRY BrFileOpenWrite(char *name, int mode)
{
    void *raw_file;
    br_file *file;

    ASSERT(fw.fsys->open_write != NULL);

    raw_file = fw.fsys->open_write(name, mode);

    /*
     * Catch failure
     */
    if (raw_file == NULL)
        return NULL;

    /*
     * Create a file block (which includes the name and mode)
     */
    file = BrResAllocate(fw.res, sizeof(br_file) + strlen(name), BR_MEMORY_FILE);
    file->writing = BR_TRUE;
    file->mode = mode;
    file->raw_file = raw_file;
    strcpy(file->name, name);

    return file;
}

void BR_PUBLIC_ENTRY BrFileClose(void *f)
{
    BrResFree(f);
}

int BR_PUBLIC_ENTRY BrFileEof(void *f)
{
    ASSERT(fw.fsys->eof != NULL);

    return fw.fsys->eof(((br_file *)f)->raw_file);
}

int BR_PUBLIC_ENTRY BrFileGetChar(void *f)
{
    ASSERT(fw.fsys->getchr != NULL);

    return fw.fsys->getchr(((br_file *)f)->raw_file);
}

void BR_PUBLIC_ENTRY BrFilePutChar(int c, void *f)
{
    ASSERT(fw.fsys->putchr != NULL);

    fw.fsys->putchr(c, ((br_file *)f)->raw_file);
}

int BR_PUBLIC_ENTRY BrFileRead(void *buf, int size, int n, void *f)
{

    ASSERT(fw.fsys->read != NULL);

    return fw.fsys->read(buf, size, n, ((br_file *)f)->raw_file);
}

int BR_PUBLIC_ENTRY BrFileWrite(void *buf, int size, int n, void *f)
{
    ASSERT(fw.fsys->write != NULL);

    return fw.fsys->write(buf, size, n, ((br_file *)f)->raw_file);
}

int BR_PUBLIC_ENTRY BrFileGetLine(char *buf, br_size_t buf_len, void *f)
{
    return fw.fsys->getline(buf, buf_len, ((br_file *)f)->raw_file);
}

void BR_PUBLIC_ENTRY BrFilePutLine(char *buf, void *f)
{
    ASSERT(fw.fsys->putline != NULL);

    fw.fsys->putline(buf, ((br_file *)f)->raw_file);
}

void BR_PUBLIC_ENTRY BrFileAdvance(long int count, void *f)
{
    ASSERT(fw.fsys->advance != NULL);

    fw.fsys->advance(count, ((br_file *)f)->raw_file);
}

int BR_PUBLIC_ENTRY BrFilePrintf(void *f, char *fmt, ...)
{
    int n;
    va_list args;

    ASSERT(fw.fsys->write != NULL);

    va_start(args, fmt);
    n = vsprintf(_br_scratch_string, fmt, args);
    va_end(args);

    fw.fsys->write(_br_scratch_string, 1, n, ((br_file *)f)->raw_file);

    return n;
}
