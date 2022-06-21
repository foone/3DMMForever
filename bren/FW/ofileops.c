/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: ofileops.c 1.5 1995/02/22 21:42:15 sam Exp $
 * $Locker:  $
 *
 * $ BC<"make -f makefile.wtc %s.pp;">
 *
 * Out of date file operations
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>
#include <math.h>

#include "brender.h"
#include "brhton.h"
#include "animate.h"
#include "datafile.h"

static char rscid[] = "$Id: ofileops.c 1.5 1995/02/22 21:42:15 sam Exp $";

/*
 * For digging around in points etc - makes things slightly easier to read
 */
#define X 0
#define Y 1
#define Z 2
#define W 3

#define U 0
#define V 1

#if 0
/*
 * File structures
 */
typedef struct br_file_info {
	br_int_32 type;
	br_int_32 version;
} br_file_info;

typedef struct br_chunk_header {
	br_int_32 id;
	br_int_32 length;
} br_chunk_header;

typedef struct br_file_pixmap {
	br_int_32 width;
	br_int_32 height;
} br_file_pixmap;

typedef struct br_file_material {
	/* */
	br_uint_16 flags;

	br_float red;
	br_float green;
	br_float blue;

	br_float ka;
	br_float kd;
	br_float ks;

	br_float opacity;

	char identifier[1];
} br_file_material;

typedef struct br_file_actor_data {
		char		identifier[16];
		long		type;
		br_float	matrix34[4][3];
} br_file_actor_data;

typedef struct br_file_actor {
	/* */

	br_file_actor_data file;

	struct br_file_actor *prev;
	struct br_file_actor *next;
	struct br_file_actor *child;
	struct br_file_actor *sibling;
	struct br_file_actor *parent;
} br_file_actor;

typedef struct br_file_animation {
	br_float matrix[4][4];
} br_file_animation;

typedef struct br_file_anim {
	/* */
	
	char *identifier;
	struct br_animation *frames;
}  br_file_anim ;

typedef struct br_file_model {
	/* */
	char identifier[1];
} br_file_model;

typedef struct br_file_vertex {
	br_float x;
	br_float y;
	br_float z;
} br_file_vertex;

typedef struct br_file_vertex_uv {
	br_float x;
	br_float y;
	br_float z;
	br_float u;
	br_float v;
} br_file_vertex_uv;

typedef struct br_file_face {
	br_uint_16 v1;
	br_uint_16 v2;
	br_uint_16 v3;

	br_uint_16 material;
	br_uint_32 smoothing;
} br_file_face;
#endif

extern br_animation *anim[20];

br_model *objects[50];

/*
 * Read animation frames from the file.
 */
int BR_PUBLIC_ENTRY BrOldAnimLoad(char *filename, br_animation ***anim, int *num_frames, br_model **objects)
{
    int count = 0;
    int frame = 0, num = 0;
    br_animation *temp;
    br_animation **frames;
    FILE *fp;
    char *name;

    fp = FOPEN(filename, "rb");

    for (;;)
    {
        if (DfDataInterpret(fread, fseek, fp) == 0)
            break;

        temp = (br_animation *)DfPop(DFST_ANIM_TRANSFORM, &count);
        NEW_PTR_N(frames, count + 1);
        *num_frames = count;
        while (count > 0)
        {
            frames[count] = temp;
            temp = (br_animation *)DfPop(DFST_ANIM_TRANSFORM, &count);
        }
        frames[count] = temp;
        name = (char *)DfPop(DFST_ANIM_NAME, NULL);
        count = BrActorFind(name, objects);

        if (count >= 0)
            anim[count] = frames;
        num++;
    }

    FCLOSE(fp);

    return num;
}

/*
 * Find the named model in the renderer's current set, return Model pointer
 */
br_model *BR_PUBLIC_ENTRY BrOldModelFind(char *name, br_model **objects)
{
    int i;
    for (i = 1; objects[i]; i++)
        if (strcmp(name, objects[i]->identifier) == 0)
            return objects[i];
    return objects[0];
}

/*
 * Find the named model in the renderer's current set, return Actor index
 */
br_uint_32 BR_PUBLIC_ENTRY BrOldActorFind(char *name, br_model **objects)
{
    int i;
    for (i = 0; objects[i]; i++)
        if (strncmp(name, objects[i]->identifier, 10) == 0)
            return i;
    return -1;
}

/*
 * read file type of a file from the p3d_file_info block
 * a file
 */
br_uint_32 BR_PUBLIC_ENTRY BrOldTypeRead(char *filename)
{
    br_file_info *info;
    FILE *fp;

    fp = FOPEN(filename, "rb");
    DfTypeInterpret(fread, fseek, fp);
    info = (br_file_info *)DfPop(DFST_FILE_INFO, NULL);
    FCLOSE(fp);

    return info->type;
}
