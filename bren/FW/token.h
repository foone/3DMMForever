/*
 * Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: token.h 1.1 1995/07/28 19:03:08 sam Exp $
 * $Locker:  $
 *
 */
#ifndef _TOKEN_H_
#define _TOKEN_H_

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * Tokens are unique 32 bit numbers that are assoicated with a name
     * and a value type.
     */
    typedef br_uint_32 br_token;

    /*
     * Token values are 32 bit
     */
    typedef br_uint_32 br_token_value;

    /*
     * Values that can be associated with a token
     */
    enum
    {
        /* Suffix used on tokens */
        BR_TT_NONE,    /* 		*/
        BR_TT_BOOLEAN, /* B	*/
        BR_TT_POINTER, /* P	*/
        BR_TT_STRING,  /* S	*/
        BR_TT_TOKEN,   /* T	*/

        BR_TT_INT_8,   /* I8	*/
        BR_TT_UINT_8,  /* U8	*/
        BR_TT_INT_16,  /* I16	*/
        BR_TT_UINT_16, /* U16	*/
        BR_TT_INT_32,  /* I32	*/
        BR_TT_UINT_32, /* U32	*/

        BR_TT_FIXED,           /* X  	*/
        BR_TT_FIXED_FRACTION,  /* XF 	*/
        BR_TT_FIXED_UFRACTION, /* XUF	*/

        BR_TT_FLOAT, /* F  	*/

        BR_TT_DEVICE_HANDLE, /* H  	*/
        BR_TT_VECTOR2,       /* V2 	*/
        BR_TT_VECTOR3,       /* V3 	*/
        BR_TT_VECTOR4,       /* V4 	*/
        BR_TT_FVECTOR2,      /* FV2	*/
        BR_TT_FVECTOR3,      /* FV3	*/
        BR_TT_FVECTOR4,      /* FV4	*/
        BR_TT_MATRIX23,      /* M23	*/
        BR_TT_MATRIX34,      /* M34	*/
        BR_TT_MATRIX4,       /* M4 	*/

        BR_TT_ANGLE,      /* A	*/
        BR_TT_COLOUR_RGB, /* RGB	*/
    };

#ifdef __cplusplus
};
#endif
#endif
