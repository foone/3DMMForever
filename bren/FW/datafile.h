/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: datafile.h 1.22 1995/08/31 16:29:22 sam Exp $
 * $Locker:  $
 *
 * Constants and structures inside a 3d data file
 */

#ifndef _DATAFILE_H_
#define _DATAFILE_H_

#ifndef _BRHTON_H_
#include "brhton.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Version number written into new files
 */
#define BR_FILE_VERSION 2

/*
 * File open modes
 */
#define DFOPEN_READ 0
#define DFOPEN_WRITE 1
#define DFOPEN_BINARY 2

    /*
     * File chunk IDs
     */
    enum
    {
        FID_END,

        FID_IMAGE_PLANE,     /* UNUSED */
        FID_RLE_IMAGE_PLANE, /* UNUSED */
        FID_PIXELMAP,

        FID_MATERIAL,
        FID_ADD_MATERIAL, /* UNUSED */

        FID_OLD_ACTOR,       /* UNUSED */
        FID_OLD_ADD_SIBLING, /* UNUSED */
        FID_OLD_ADD_CHILD,   /* UNUSED */

        FID_OLD_MATERIAL_INDEX,
        FID_OLD_VERTICES,
        FID_OLD_VERTICES_UV,
        FID_OLD_FACES,
        FID_OLD_MODEL,

        FID_ADD_MODEL,

        FID_ANIM,
        FID_ANIM_TRANSFORM,
        FID_ANIM_RATE,
        FID_FILE_INFO,
        FID_OLD_LIGHT,  /* UNUSED */
        FID_OLD_CAMERA, /* UNUSED */
        FID_PIVOT,

        FID_MATERIAL_INDEX,
        FID_VERTICES,
        FID_VERTEX_UV,
        FID_OLD_FACES_1,
        FID_FACE_MATERIAL,
        FID_OLD_MODEL_1,

        FID_COLOUR_MAP_REF,
        FID_OPACITY_MAP_REF, /* UNUSED */
        FID_INDEX_BLEND_REF,
        FID_INDEX_SHADE_REF,
        FID_SCREENDOOR_REF,

        FID_PIXELS,
        FID_ADD_MAP,

        FID_ACTOR,

        FID_ACTOR_MODEL,
        FID_ACTOR_TRANSFORM,
        FID_ACTOR_MATERIAL,

        FID_ACTOR_LIGHT,
        FID_ACTOR_CAMERA,
        FID_ACTOR_BOUNDS,

        FID_ACTOR_ADD_CHILD,

        FID_TRANSFORM_MATRIX34,
        FID_TRANSFORM_MATRIX34_LP,
        FID_TRANSFORM_QUAT,
        FID_TRANSFORM_EULER,
        FID_TRANSFORM_LOOK_UP,
        FID_TRANSFORM_TRANSLATION,
        FID_TRANSFORM_IDENTITY,

        FID_BOUNDS,
        FID_LIGHT,
        FID_CAMERA,

        FID_FACES,
        FID_MODEL,

        FID_ACTOR_CLIP_PLANE,
        FID_PLANE,

        FID_SATURN_FACES,
        FID_SATURN_MODEL
    };

    /*
     * Data Stack types
     */
    enum
    {
        DFST_NONE,
        DFST_IMAGE_PLANE,
        DFST_PIXELMAP,
        DFST_MATERIAL,
        DFST_ACTOR,
        DFST_MATERIAL_INDEX,
        DFST_VERTICES,
        DFST_FACES,
        DFST_MODEL,
        DFST_ANIM,
        DFST_ANIM_NAME,
        DFST_ANIM_TRANSFORM,
        DFST_ANIM_COUNT,
        DFST_ANIM_RATE,
        DFST_FILE_INFO,
        DFST_PIVOT,
        DFST_TRANSFORM,
        DFST_LIGHT,
        DFST_CAMERA,
        DFST_BOUNDS,
        DFST_PLANE,
        DFST_MARK,

        DFST_SATURN_FACES,
        DFST_SATURN_MODEL,

        DFST_MAX
    };

    /*
     * File info types
     */
    enum
    {
        FILE_TYPE_NONE,

        /*	FILE_TYPE_MODEL, */
        FILE_TYPE_ACTORS,
        /*	FILE_TYPE_ANIMATION, */
        FILE_TYPE_PIXELMAP,
        FILE_TYPE_LIGHT,
        FILE_TYPE_CAMERA,
        FILE_TYPE_MATERIAL,

        /*
         * Old Types
         */
        FILE_TYPE_MODEL = 0xFACE,
        FILE_TYPE_ANIMATION = 0x0A11,
        FILE_TYPE_TREE = 0x5EED
    };

    /**
     ** General structure reading/writing system
     **/

    /*
     * Member types
     */
    enum
    {
        FSM_INT_8,
        FSM_UINT_8,
        FSM_INT_16,
        FSM_UINT_16,
        FSM_INT_32,
        FSM_UINT_32,
        FSM_FIXED,
        FSM_ANGLE,
        FSM_FLOAT,
        FSM_DOUBLE,
        FSM_SCALAR,
        FSM_FRACTION,
        FSM_UFRACTION,
        FSM_ENUM_8,
        FSM_ENUM_16,
        FSM_ENUM_32,
        FSM_STRUCT,
        FSM_ASCIZ,
        FSM_COLOUR,
        FSM_VECTOR2,
        FSM_VECTOR3,
        FSM_VECTOR4
    };

    typedef struct br_file_struct_member
    {
        br_uint_16 type;   /* Type of member 							*/
        br_uint_32 offset; /* Offset in memory from base of structure	*/
        char *name;        /* text identifier for debug purposes		*/
        void *extra;       /* Any extra type data						*/
    } br_file_struct_member;

    typedef struct br_file_struct
    {
        char *name;
        br_uint_32 nmembers;
        br_file_struct_member *members;
        int mem_size;
    } br_file_struct;

/*
 * Some nasty macros to make building structure descriptions easier
 * Requires that a macro _STRUCT_NAME be defined - the type of the
 * structure being referenced
 */
#define _INT_8(m)                                                                                                      \
    {                                                                                                                  \
        FSM_INT_8, offsetof(_STRUCT_NAME, m), #m                                                                       \
    }
#define _UINT_8(m)                                                                                                     \
    {                                                                                                                  \
        FSM_UINT_8, offsetof(_STRUCT_NAME, m), #m                                                                      \
    }
#define _INT_16(m)                                                                                                     \
    {                                                                                                                  \
        FSM_INT_16, offsetof(_STRUCT_NAME, m), #m                                                                      \
    }
#define _UINT_16(m)                                                                                                    \
    {                                                                                                                  \
        FSM_UINT_16, offsetof(_STRUCT_NAME, m), #m                                                                     \
    }
#define _INT_32(m)                                                                                                     \
    {                                                                                                                  \
        FSM_INT_32, offsetof(_STRUCT_NAME, m), #m                                                                      \
    }
#define _UINT_32(m)                                                                                                    \
    {                                                                                                                  \
        FSM_UINT_32, offsetof(_STRUCT_NAME, m), #m                                                                     \
    }
#define _FIXED(m)                                                                                                      \
    {                                                                                                                  \
        FSM_FIXED, offsetof(_STRUCT_NAME, m), #m                                                                       \
    }
#define _ANGLE(m)                                                                                                      \
    {                                                                                                                  \
        FSM_ANGLE, offsetof(_STRUCT_NAME, m), #m                                                                       \
    }
#define _FLOAT(m)                                                                                                      \
    {                                                                                                                  \
        FSM_FLOAT, offsetof(_STRUCT_NAME, m), #m                                                                       \
    }
#define _DOUBLE(m)                                                                                                     \
    {                                                                                                                  \
        FSM_DOUBLE, offsetof(_STRUCT_NAME, m), #m                                                                      \
    }
#define _SCALAR(m)                                                                                                     \
    {                                                                                                                  \
        FSM_SCALAR, offsetof(_STRUCT_NAME, m), #m                                                                      \
    }
#define _FRACTION(m)                                                                                                   \
    {                                                                                                                  \
        FSM_FRACTION, offsetof(_STRUCT_NAME, m), #m                                                                    \
    }
#define _UFRACTION(m)                                                                                                  \
    {                                                                                                                  \
        FSM_UFRACTION, offsetof(_STRUCT_NAME, m), #m                                                                   \
    }
#define _ENUM_8(m, e)                                                                                                  \
    {                                                                                                                  \
        FSM_ENUM_8, offsetof(_STRUCT_NAME, m), #m, &(e)                                                                \
    }
#define _ENUM_16(m, e)                                                                                                 \
    {                                                                                                                  \
        FSM_ENUM_16, offsetof(_STRUCT_NAME, m), #m, &(e)                                                               \
    }
#define _ENUM_32(m, e)                                                                                                 \
    {                                                                                                                  \
        FSM_ENUM_32, offsetof(_STRUCT_NAME, m), #m, &(e)                                                               \
    }
#define _STRUCT(m, s)                                                                                                  \
    {                                                                                                                  \
        FSM_STRUCT, offsetof(_STRUCT_NAME, m), #m, &(s)                                                                \
    }
#define _ASCIZ(m)                                                                                                      \
    {                                                                                                                  \
        FSM_ASCIZ, offsetof(_STRUCT_NAME, m), #m                                                                       \
    }
#define _COLOUR(m)                                                                                                     \
    {                                                                                                                  \
        FSM_COLOUR, offsetof(_STRUCT_NAME, m), #m                                                                      \
    }
#define _VECTOR2(m)                                                                                                    \
    {                                                                                                                  \
        FSM_VECTOR2, offsetof(_STRUCT_NAME, m), #m                                                                     \
    }
#define _VECTOR3(m)                                                                                                    \
    {                                                                                                                  \
        FSM_VECTOR3, offsetof(_STRUCT_NAME, m), #m                                                                     \
    }
#define _VECTOR4(m)                                                                                                    \
    {                                                                                                                  \
        FSM_VECTOR4, offsetof(_STRUCT_NAME, m), #m                                                                     \
    }

#define _FILE_STRUCT(name)                                                                                             \
    br_file_struct name##_F = {                                                                                        \
        #name,                                                                                                         \
        BR_ASIZE(name##_FM),                                                                                           \
        name##_FM,                                                                                                     \
        sizeof(_STRUCT_NAME),                                                                                          \
    }

    /*
     * Structures for describing an enum
     */
    typedef struct br_file_enum_member
    {
        int value;
        char *name;
    } br_file_enum_member;

    typedef struct br_file_enum
    {
        br_uint_32 nmembers;
        br_file_enum_member *members;
    } br_file_enum;

/*
 * Macros for building enums
 */
#define _ENUM_MEMBER(e)                                                                                                \
    {                                                                                                                  \
        e, #e                                                                                                          \
    }

#define _FILE_ENUM(name)                                                                                               \
    br_file_enum name##_F = {                                                                                          \
        BR_ASIZE(name##_FM),                                                                                           \
        name##_FM,                                                                                                     \
    }

/*
 * Constants used to grab bytes out of a byte array mapped onto each type
 */
#if BR_ENDIAN_BIG

#define BR_HTON_64(b) (b)
#define BR_HTON_32(b) (b)
#define BR_HTON_24(b) (b)
#define BR_HTON_16(b) (b)
#define BR_HTON_8(b) (b)
#define BR_HTON_F(b) (b)
#define BR_HTON_D(b) (b)

#define BR_NTOH_64(b) (b)
#define BR_NTOH_32(b) (b)
#define BR_NTOH_24(b) (b)
#define BR_NTOH_16(b) (b)
#define BR_NTOH_8(b) (b)
#define BR_NTOH_F(b) (b)
#define BR_NTOH_D(b) (b)

#endif

#if BR_ENDIAN_LITTLE

#define BR_HTON_64(b) (7 - (b))
#define BR_HTON_32(b) (3 - (b))
#define BR_HTON_24(b) (2 - (b))
#define BR_HTON_16(b) (1 - (b))
#define BR_HTON_8(b) (b)
#define BR_HTON_F(b) (3 - (b))
#define BR_HTON_D(b) (7 - (b))

#define BR_NTOH_64(b) (7 - (b))
#define BR_NTOH_32(b) (3 - (b))
#define BR_NTOH_24(b) (2 - (b))
#define BR_NTOH_16(b) (1 - (b))
#define BR_NTOH_8(b) (b)
#define BR_NTOH_F(b) (3 - (b))
#define BR_NTOH_D(b) (7 - (b))

#endif

    struct br_file_primitives;

    typedef struct br_datafile
    {
        /*
         * File handle
         */
        void *h;

        /*
         * Primitives
         */
        struct br_file_primitives *prims;

        /*
         * Resource that any datafile allocations should be attached to
         */
        void *res;

    } br_datafile;

    /*
     * Table used to associate chunk ID's with handlers
     */
    typedef struct br_chunks_table_entry
    {
        br_uint_32 id;
        br_uint_8 has_count;
        int (*handler)(br_datafile *df, br_uint_32 id, br_uint_32 length, br_uint_32 count);
    } br_chunks_table_entry;

    typedef struct br_chunks_table
    {
        int nentries;
        br_chunks_table_entry *entries;
    } br_chunks_table;

    /*
     * Vector of functions for reading and writing datafile primitives
     */
    typedef struct br_file_primitives
    {

        char *identifier;

        /*
         * High level file operations
         */

        int (*skip)(br_datafile *df, br_uint_32 length);

        int (*chunk_write)(br_datafile *df, br_uint_32 id, br_uint_32 length);
        int (*chunk_read)(br_datafile *df, br_uint_32 *plength);

        void (*count_write)(br_datafile *df, br_uint_32 count);
        br_uint_32 (*count_read)(br_datafile *df);
        int (*count_size)(br_datafile *df);

        br_uint_32 (*struct_write)(br_datafile *df, br_file_struct *str, void *base);
        br_uint_32 (*struct_read)(br_datafile *df, br_file_struct *str, void *base);
        int (*struct_size)(br_datafile *df, br_file_struct *str, void *base);

        int (*block_write)(br_datafile *df, void *base, int block_size, int block_stride, int block_count, int size);
        void *(*block_read)(br_datafile *df, void *base, int *pcount, int size, int mtype);
        int (*block_size)(br_datafile *df, void *base, int block_size, int block_stride, int block_count, int size);

        int (*name_write)(br_datafile *df, char *name);
        char *(*name_read)(br_datafile *df, char *name);
        int (*name_size)(br_datafile *df, char *name);

    } br_file_primitives;

#ifndef NO_PROTOTYPES
    /*
     * Datafile prototypes - thse are here so that 'trusted' applications
     * can get lower level access to the file IO by including "datafile.h"
     * as well as "brender.h"
     */
    void *DfPop(int type, int *countp);
    void DfPush(int type, void *value, int count);
    void *DfTop(int type, int *countp);
    int DfTopType(void);

    int DfChunksInterpret(br_datafile *df, struct br_chunks_table *table);

    br_uint_32 DfStructWriteArray(br_datafile *df, struct br_file_struct *str, void *base, int n);
    br_uint_32 DfStructReadArray(br_datafile *df, struct br_file_struct *str, void *base, int n);

    br_datafile *DfOpen(char *name, int write);
    void DfClose(br_datafile *df);
#endif

#ifdef __cplusplus
};
#endif
#endif
