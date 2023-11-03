/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: brerr.h 1.4 1995/07/28 18:57:02 sam Exp $
 * $Locker:  $
 *
 * Error type
 */
#ifndef _BRERR_H_
#define _BRERR_H_

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * Error types are 32 bit integers, broken into three fields -
     *
     * Class	Subclass Number
     *
     * 33222222 22221111 1111110000000000
     * 10987654 32109876 5432109876543210
     *
     * A value of zero is reserved to indiciate no error
     */
    typedef br_uint_32 br_error;

#ifdef __cplusplus
};
#endif
#endif
