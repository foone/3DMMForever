/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: zsproto.h 1.1 1995/08/31 16:37:20 sam Exp $
 * $Locker:  $
 *
 * Function prototypes for bucket Z-sort renderer
 */
#ifndef _ZSPROTO_H_
#define _ZSPROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Sort types for primitive insertion
 */
enum {
	BR_ZSORT_MIN		= 0x0001,
	BR_ZSORT_MAX		= 0x0002,
	BR_ZSORT_AVERAGE	= 0x0004,
};

#if 0

#define BR_BOUNDS_MIN_X	0
#define BR_BOUNDS_MIN_Y	1
#define BR_BOUNDS_MAX_X	2
#define BR_BOUNDS_MAX_Y	3

/*
 * Callback function invoked when an actor is
 * rendered
 */
typedef void BR_CALLBACK br_renderbounds_cbfn(
	br_actor *actor,
	br_model *model,
	br_material *material,
	void *render_data,
	br_uint_8 style,
	br_matrix4 *model_to_screen,
	br_int_32 bounds[4]);

#endif

/*
 * Callback function invoked when primitive is generated
 */
typedef void BR_CALLBACK br_primitive_cbfn(
	void *primitive,
	br_actor *actor,
	br_model *model,
	br_material *material,
	br_order_table *order_table,
	br_scalar z[3]);

#ifndef _NO_PROTOTYPES

void BR_PUBLIC_ENTRY BrZsBegin(br_uint_8 colour_type, void *primitive, br_uint_32 size);
void BR_PUBLIC_ENTRY BrZsEnd(void);

void BR_PUBLIC_ENTRY BrZsSceneRenderBegin(br_actor *world,
										  br_actor *camera,
										  br_pixelmap *colour_buffer,
										  br_uint_8 db_index,
										  br_uint_32 flags);

void BR_PUBLIC_ENTRY BrZsSceneRenderAdd(br_actor *tree);
void BR_PUBLIC_ENTRY BrZsSceneRenderEnd(void);

/*
 * Wrapper that invokes above three calls in order
 */
void BR_PUBLIC_ENTRY BrZsSceneRender(br_actor *world,
									 br_actor *camera,
									 br_pixelmap *colour_buffer,
									 br_uint_8 db_index,
									 br_uint_32 flags);

/*
 * Used within custom model callbacks to render other models
 */
void BR_PUBLIC_ENTRY BrZsModelRender(br_actor *actor,
									 br_model *model,
									 br_material *material,
									 br_order_table *order_table,
									 br_uint_8 style,
									 int on_screen,
									 int use_custom);

br_primitive_cbfn * BR_PUBLIC_ENTRY BrZsSetPrimitiveCallback(br_primitive_cbfn *new_cbfn);

br_order_table * BR_PUBLIC_ENTRY BrZsSetActorOrderTable(br_actor *actor, br_order_table *order_table);
br_order_table * BR_PUBLIC_ENTRY BrZsGetActorOrderTable(br_actor *actor);
br_order_table * BR_PUBLIC_ENTRY BrZsGetDefaultOrderTable(void);

void BR_PUBLIC_ENTRY BrZsClearOrderTable(br_order_table *order_table);
void BR_PUBLIC_ENTRY BrZsClearDefaultOrderTable(void);

#ifdef __cplusplus
};
#endif
#endif
#endif
