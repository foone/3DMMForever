/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: brexcept.h 1.2 1995/07/28 19:01:35 sam Exp $
 * $Locker:  $
 *
 * Simple exception handling
 */
#ifndef _BREXCEPT_H_
#define _BREXCEPT_H_

#ifndef __H2INC__
#include <setjmp.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * Exception types are the same as error types
     *
     * A value of zero is reserved
     */
    typedef br_error br_exception;

/*
 * Useful macros for generating types
 */
#define BR_EXCEPTION(num) BR_##_EXCEPTION_CLASS##_EXEPCTION_SUBCLASS##_##num

    /*
     * Exception handler - allocated as a resource
     */
    typedef struct br_exception_handler
    {
        struct br_exception_handler *prev;

        /*
         * setjmp/longjmp context to throw to
         */
#ifndef __H2INC__
        jmp_buf context;
#endif
    } br_exception_handler;

/*
 * Public macros
 */
#define BrExceptionCatch(evp) (_BrExceptionValueFetch((br_exception)setjmp(_BrExceptionCatch()->context), (evp)))
#define BrExceptionThrow(et, ev) _BrExceptionThrow((br_exception)(ev), (void *)(ev))
#define BR_EXCEPTION_RESOURCE _BrExceptionHandler

#ifdef __cplusplus
};
#endif
#endif
