/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: order.h 1.1 1995/08/31 16:37:15 sam Exp $
 * $Locker:  $
 *
 * Definitons for an order table
 */
#ifndef _ORDER_H_
#define _ORDER_H_

#ifdef __cplusplus
extern "C" {
#endif

enum {
	BR_ORDER_TABLE_NEW_BOUNDS	= 0x0001,
};

typedef struct br_order_table {

	/*
	 * Order table array
	 */
	void **table;

	/*
	 * Number of entries in array
	 */
	br_uint_32 ot_size;

	/*
	 * Next order table in list
	 */
	struct br_order_table *next;

	/*
	 * Order table bounds
	 */
	br_scalar min_z;
	br_scalar max_z;

	/*
	 * Flags
	 */
	br_uint_32 flags;
    
} br_order_table;


#ifdef __cplusplus
};
#endif
#endif
