
option expr32
option casemap:none

; Begin of file zb.h
; 
;  * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: zb.h 1.44 1995/08/31 16:47:50 sam Exp $
;  * $Locker:  $
;  *
;  * Internal types and structures for z-buffer renderer
;  
; 
;  * Pull in all the framework and public definitions
;  
; Begin of file ..\fw\fw.h
; 
;  * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: fw.h 1.39 1995/07/28 19:01:47 sam Exp $
;  * $Locker: sam $
;  *
;  * Internal types and structures for framework
;  
; 
;  * Pull in all the public definitions/declarations
;  
; Begin of file ..\inc\brender.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: brender.h 1.19 1995/08/31 16:36:12 sam Exp $
;  * $Locker:  $
;  *
;  * Master include file for BRender
;  *
;  
; Begin of file ..\inc\compiler.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: compiler.h 1.22 1995/08/31 16:36:15 sam Exp $
;  * $Locker: sam $
;  *
;  * Misc host compiler configuration (types & special declarators etc.)
;  
; 
;  * Fixed bitsize integers
;  
br_int_32		TYPEDEF		SDWORD

br_uint_32		TYPEDEF		DWORD

br_int_16		TYPEDEF		SWORD

br_uint_16		TYPEDEF		WORD

br_int_8		TYPEDEF		SBYTE

br_uint_8		TYPEDEF		BYTE

; 
;  * Generic size type (in case target environment does not have size_t)
;  
br_size_t		TYPEDEF		DWORD

; 
;  * Boolean type
;  
br_boolean		TYPEDEF		SDWORD

BR_TRUE		EQU		1t
BR_FALSE		EQU		0t
; *
;  ** Compiler specific declarations
;  **
;  *
;  * BR_PUBLIC_ENTRY are the declarators for public entry points into the library
;  *
;  * BR_CALLBACK are the declarators for function pointers to callbacks
;  *
;  * BR_ASM_DATA is the declarator for data that is shared with ASM files
;  *
;  * BR_ASM_CALL is the declarator for function declarations that are ASM functions
;  * called from C
;  *
;  * BR_ASM_CALLBACK is the declarator for function definitions that are C functions
;  * called from ASM
;  *
;  * BR_SUFFIX_HOST is a string used for banners/titles etc
;  *
;  * br_framebuffer_ptr is a type used for pointing at the pixels of a frame buffer
;  *
;  * BR_HAS_FAR is !0 if language supports __far pointers
;  
; 
;  * WATCOM C/C++ 32
;  
BR_HAS_FAR		EQU		0t
MEMAVL		EQU		0t
; 
;  * Metaware High-C Version 1
;  
; 
;  * Avoid some tokens that masm chokes on
;  
; 
;  * Supress compiler specific declarators
;  
; 
;  * Macros for producing banners & copyright messages
;  
; 
;  * Useful macro for sizing an array
;  
; End of file ..\inc\compiler.h
; Begin of file ..\inc\brlimits.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: brlimits.h 1.5 1995/02/22 21:37:01 sam Exp $
;  * $Locker:  $
;  *
;  * Various limits imposed by the renderer
;  
; 
;  * Maximum length of an item's name
;  
BR_MAX_NAME		EQU		256t
; 
;  * Maximum number of active lights
;  
BR_MAX_LIGHTS		EQU		16t
; 
;  * Maximum number of active clip planes
;  
BR_MAX_CLIP_PLANES		EQU		6t
; 
;  * Maximum number of resource classes
;  
BR_MAX_RESOURCE_CLASSES		EQU		256t
; 
;  * Maximum depth of camera in hierachy
;  
MAX_CAMERA_DEPTH		EQU		16t
; End of file ..\inc\brlimits.h
; Begin of file ..\inc\fixed.h
; 
;  * Copyright (c) 1992,1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: fixed.h 1.17 1995/02/22 21:37:06 sam Exp $
;  * $Locker:  $
;  
INLINE_FIXED		EQU		1t
; 
;  * Fixed point types
;  
br_fixed_ls		TYPEDEF		SDWORD

;  Long Signed fixed			15.16 
br_fixed_lsf		TYPEDEF		SWORD

;  Long Signed fraction			 0.15 
br_fixed_lu		TYPEDEF		DWORD

;  Long unsigned fixed			16.16 
br_fixed_luf		TYPEDEF		WORD

;  Long unsigned fixed fraction  0.16 
br_fixed_ss		TYPEDEF		SWORD

;  Short Signed fixed			 7.8  
br_fixed_ssf		TYPEDEF		SBYTE

;  Short Signed fraction		 0.7  
br_fixed_su		TYPEDEF		WORD

;  Short unsigned fixed			 8.8  
br_fixed_suf		TYPEDEF		BYTE

;  Short unsigned fixed fraction 0.8  
; 
;  * 1 in various fixed point forms
;  
BR_ONE_LS		EQU		000010000h
BR_ONE_LSF		EQU		000008000h
BR_ONE_LU		EQU		000010000h
BR_ONE_LUF		EQU		000010000h
BR_ONE_SS		EQU		000000100h
BR_ONE_SSF		EQU		000000080h
BR_ONE_SU		EQU		000000100h
BR_ONE_SUF		EQU		000000100h
; End of file ..\inc\fixed.h
; Begin of file ..\inc\scalar.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: scalar.h 1.21 1995/08/31 16:36:20 sam Exp $
;  * $Locker:  $
;  *
;  * Scalar type
;  *
;  * External defines:
;  *
;  * BASED_FIXED
;  *  True if system is based on signed 16.16 fixed point
;  *
;  * BASED_FLOAT
;  *  True if system is based on 32 bit floating point
;  *
;  * One of these must be defined and the appropriate libraries must be used
;  
BASED_FLOAT		EQU		0t
; *
;  ** Basic types - either fixed or float
;  *
; 
;  * Reference an external symbol to make sure
;  * the right library is linked
;  
EXTERNDEF		C _BR_Fixed_Point_Scalar:SDWORD

; 
; 	 * Fixed base types
; 	 
br_scalar		TYPEDEF		br_fixed_ls

br_fraction		TYPEDEF		br_fixed_lsf

br_ufraction		TYPEDEF		br_fixed_luf

BR_SCALAR_EPSILON		EQU		000000001h
BR_SCALAR_MAX		EQU		07fffffffh
BR_SCALAR_MIN		EQU		080000000h
; 
;  * Macros for static initialisation
;  
; 
;  * Macros for type conversion
;  
; 
;  * Various arithmetic operations
;  
; 
;  * Make sure PI is defined
;  
; End of file ..\inc\scalar.h
; Begin of file ..\inc\colour.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: colour.h 1.4 1995/08/31 16:36:14 sam Exp $
;  * $Locker:  $
;  *
;  * Colour type and macros
;  *
;  
br_colour		TYPEDEF		DWORD

; End of file ..\inc\colour.h
; Begin of file ..\inc\matrix.h
; 
;  * Copyright (c) 1992,1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: matrix.h 1.3 1995/02/22 21:37:13 sam Exp $
;  * $Locker:  $
;  *
;  * Structrures describing matrices
;  
br_matrix4		STRUCT 4t
m		br_scalar		4t DUP (4t DUP (?))
br_matrix4		ENDS

br_matrix34		STRUCT 4t
m		br_scalar		4t DUP (3t DUP (?))
br_matrix34		ENDS

br_matrix23		STRUCT 4t
m		br_scalar		3t DUP (2t DUP (?))
br_matrix23		ENDS

; End of file ..\inc\matrix.h
; Begin of file ..\inc\vector.h
; 
;  * Copyright (c) 1992,1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: vector.h 1.9 1995/06/30 16:09:07 sam Exp $
;  * $Locker:  $
;  *
;  * Vector types
;  
; 
;  * Vector types
;  
br_vector2		STRUCT 4t
v		br_scalar		2t DUP (?)
br_vector2		ENDS

br_vector3		STRUCT 4t
v		br_scalar		3t DUP (?)
br_vector3		ENDS

br_vector4		STRUCT 4t
v		br_scalar		4t DUP (?)
br_vector4		ENDS

; 
;  * Integer vectors (used for integer bounding boxes)
;  
br_vector2i		STRUCT 4t
v		br_int_32		2t DUP (?)
br_vector2i		ENDS

br_vector3i		STRUCT 4t
v		br_int_32		3t DUP (?)
br_vector3i		ENDS

br_vector4i		STRUCT 4t
v		br_int_32		4t DUP (?)
br_vector4i		ENDS

; 
;  * Fraction vector types
;  
br_fvector2		STRUCT 4t
v		br_fraction		2t DUP (?)
br_fvector2		ENDS

br_fvector3		STRUCT 4t
v		br_fraction		3t DUP (?)
br_fvector3		ENDS

br_fvector4		STRUCT 4t
v		br_fraction		4t DUP (?)
br_fvector4		ENDS

; 
;  * General structures for describing an axis aligned bounds
;  
;  Minimum corner 
;  Maximum corner 
br_bounds2		STRUCT 4t
min		br_vector2		<>
max		br_vector2		<>
br_bounds2		ENDS

;  Minimum corner 
;  Maximum corner 
br_bounds3		STRUCT 4t
min		br_vector3		<>
max		br_vector3		<>
br_bounds3		ENDS

;  Minimum corner 
;  Maximum corner 
br_bounds4		STRUCT 4t
min		br_vector4		<>
max		br_vector4		<>
br_bounds4		ENDS

; 
;  * Integer bounding boxes
;  
;  Minimum corner 
;  Maximum corner 
br_bounds2i		STRUCT 4t
min		br_vector2i		<>
max		br_vector2i		<>
br_bounds2i		ENDS

;  Minimum corner 
;  Maximum corner 
br_bounds3i		STRUCT 4t
min		br_vector3i		<>
max		br_vector3i		<>
br_bounds3i		ENDS

;  Minimum corner 
;  Maximum corner 
br_bounds4i		STRUCT 4t
min		br_vector4i		<>
max		br_vector4i		<>
br_bounds4i		ENDS

; 
;  * Backwards compatibility
;  
br_bounds		TYPEDEF		br_bounds3

; 
;  * Macros for static initialisation
;  
; End of file ..\inc\vector.h
; Begin of file ..\inc\angles.h
; 
;  * Copyright (c) 1992,1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: angles.h 1.10 1995/08/31 16:36:11 sam Exp $
;  * $Locker:  $
;  *
;  
; 
;  * Angles are alway 0.16 fixed point
;  
br_angle		TYPEDEF		br_fixed_luf

; 
;  * Fields that go into br_euler.order
;  
BR_EULER_FIRST		EQU		3t
BR_EULER_FIRST_X		EQU		0t
BR_EULER_FIRST_Y		EQU		1t
BR_EULER_FIRST_Z		EQU		2t
BR_EULER_PARITY		EQU		4t
BR_EULER_PARITY_EVEN		EQU		0t
BR_EULER_PARITY_ODD		EQU		4t
BR_EULER_REPEAT		EQU		8t
BR_EULER_REPEAT_NO		EQU		0t
BR_EULER_REPEAT_YES		EQU		8t
BR_EULER_FRAME		EQU		16t
BR_EULER_FRAME_STATIC		EQU		0t
BR_EULER_FRAME_ROTATING		EQU		16t

; 
;  * Various possible orders
;  
BR_EULER_XYZ_S		EQU		0t
BR_EULER_XYX_S		EQU		8t
BR_EULER_XZY_S		EQU		4t
BR_EULER_XZX_S		EQU		12t
BR_EULER_YZX_S		EQU		1t
BR_EULER_YZY_S		EQU		9t
BR_EULER_YXZ_S		EQU		5t
BR_EULER_YXY_S		EQU		13t
BR_EULER_ZXY_S		EQU		2t
BR_EULER_ZXZ_S		EQU		10t
BR_EULER_ZYX_S		EQU		6t
BR_EULER_ZYZ_S		EQU		14t
BR_EULER_ZYX_R		EQU		16t
BR_EULER_XYX_R		EQU		24t
BR_EULER_YZX_R		EQU		20t
BR_EULER_XZX_R		EQU		28t
BR_EULER_XZY_R		EQU		17t
BR_EULER_YZY_R		EQU		25t
BR_EULER_ZXY_R		EQU		21t
BR_EULER_YXY_R		EQU		29t
BR_EULER_YXZ_R		EQU		18t
BR_EULER_ZXZ_R		EQU		26t
BR_EULER_XYZ_R		EQU		22t
BR_EULER_ZYZ_R		EQU		30t

; 
;  * A triple of euler angles and a description of how they are to
;  * be applied - loosely based on -
;  * 	"Euler Angle Convertion" Ken Shoemake, Graphics Gems IV pp. 222
;  
br_euler		STRUCT 4t
_a		br_angle		?
_b		br_angle		?
_c		br_angle		?
order		br_uint_8		?
br_euler		ENDS

; End of file ..\inc\angles.h
; Begin of file ..\inc\quat.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: quat.h 1.5 1995/02/22 21:37:21 sam Exp $
;  * $Locker:  $
;  *
;  * Type descibing a unit quaternion
;  
br_quat		STRUCT 4t
x		br_scalar		?
y		br_scalar		?
z		br_scalar		?
w		br_scalar		?
br_quat		ENDS

; End of file ..\inc\quat.h
; Begin of file ..\inc\transfrm.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: transfrm.h 1.11 1995/02/22 21:37:24 sam Exp $
;  * $Locker:  $
;  *
;  * Structure describing an affine transform from one coordinate space
;  * to another
;  
; 
;  * Type of actor position
;  
BR_TRANSFORM_MATRIX34		EQU		0t
BR_TRANSFORM_MATRIX34_LP		EQU		1t
BR_TRANSFORM_QUAT		EQU		2t
BR_TRANSFORM_EULER		EQU		3t
BR_TRANSFORM_LOOK_UP		EQU		4t
BR_TRANSFORM_TRANSLATION		EQU		5t
BR_TRANSFORM_IDENTITY		EQU		6t
BR_TRANSFORM_MAX		EQU		7t

; 
; 	 * Type of position
; 	 
; 
; 	 * Union of the various means of describing a transform -
; 	 * these are explicity arrranged so that any exlicit transform
; 	 * will always be available as br_transform.t.translate
; 	 
; 
; 		 * Affine 3x4 matrix
; 		 
; 
; 		 * Euler angles and translation
; 		 
@tag_4		STRUCT 4t
e		br_euler		<>
_pad		br_scalar		7t DUP (?)
t		br_vector3		<>
@tag_4		ENDS

; 
; 		 * Unit quaternion and translation
; 		 
@tag_5		STRUCT 4t
q		br_quat		<>
_pad		br_scalar		5t DUP (?)
t		br_vector3		<>
@tag_5		ENDS

; 
; 		 * Lookat vector, up vector and translation
; 		 
@tag_6		STRUCT 4t
look		br_vector3		<>
up		br_vector3		<>
_pad		br_scalar		3t DUP (?)
t		br_vector3		<>
@tag_6		ENDS

; 
; 		 * Just a translation
; 		 
@tag_7		STRUCT 4t
_pad		br_scalar		9t DUP (?)
t		br_vector3		<>
@tag_7		ENDS

@tag_3		UNION
mat		br_matrix34		<>
euler		@tag_4		<>
quat		@tag_5		<>
look_up		@tag_6		<>
translate		@tag_7		<>
@tag_3		ENDS

br_transform		STRUCT 4t
_type		br_uint_16		?
t		@tag_3		<>
br_transform		ENDS

; End of file ..\inc\transfrm.h
; Begin of file ..\inc\order.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: order.h 1.1 1995/08/31 16:37:15 sam Exp $
;  * $Locker:  $
;  *
;  * Definitons for an order table
;  
BR_ORDER_TABLE_NEW_BOUNDS		EQU		1t

; 
; 	 * Order table array
; 	 
@t_0		TYPEDEF		PTR PTR 
; 
; 	 * Number of entries in array
; 	 
; 
; 	 * Next order table in list
; 	 
@t_1		TYPEDEF		PTR br_order_table
; 
; 	 * Order table bounds
; 	 
; 
; 	 * Flags
; 	 
br_order_table		STRUCT 4t
table		@t_0		?
ot_size		br_uint_32		?
next		@t_1		?
min_z		br_scalar		?
max_z		br_scalar		?
flags		br_uint_32		?
br_order_table		ENDS

; End of file ..\inc\order.h
; Begin of file ..\inc\pixelmap.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: pixelmap.h 1.17 1995/08/31 16:36:19 sam Exp $
;  * $Locker:  $
;  *
;  * A stopgap 2D pixelmap structure for brender. This should really be the
;  * pixelmap data type from the underlying 2D system (whatever that will
;  * be)
;  *
;  * Used for input (maps) and output (render buffer)
;  
; 
;  * Various types of pixel
;  
; 
; 	 * Each pixel is an index into a colour map
; 	 
BR_PMT_INDEX_1		EQU		0t
BR_PMT_INDEX_2		EQU		1t
BR_PMT_INDEX_4		EQU		2t
BR_PMT_INDEX_8		EQU		3t
; 
; 	 * True colour RGB
; 	 
BR_PMT_RGB_555		EQU		4t
;  16 bits per pixel 
BR_PMT_RGB_565		EQU		5t
;  16 bits per pixel 
BR_PMT_RGB_888		EQU		6t
;  24 bits per pixel 
BR_PMT_RGBX_888		EQU		7t
;  32 bits per pixel 
BR_PMT_RGBA_8888		EQU		8t
;  32 bits per pixel 
; 
; 	 * YUV
; 	 
BR_PMT_YUYV_8888		EQU		9t
;  YU YV YU YV ... 
BR_PMT_YUV_888		EQU		10t
; 
; 	 * Depth
; 	 
BR_PMT_DEPTH_16		EQU		11t
BR_PMT_DEPTH_32		EQU		12t
; 
; 	 * Opacity
; 	 
BR_PMT_ALPHA_8		EQU		13t
; 
; 	 * Opacity + Index
; 	 
BR_PMT_INDEXA_88		EQU		14t
; 
; 	 * Bump maps
; 	 
BR_PMT_NORMAL_INDEX_8		EQU		15t
BR_PMT_NORMAL_XYZ		EQU		16t
; 
; 	 * Wrong way around 15 bit true colour
; 	 
BR_PMT_BGR_555		EQU		17t

; 
;  * pixelmap flags
;  
; 
; 		 * No direct access to pixels
; 		 
BR_PMF_NO_ACCESS		EQU		1t
BR_PMF_LINEAR		EQU		2t
BR_PMF_ROW_WHOLEPIXELS		EQU		4t

; 
; 	 * Optional identifier (when maps used as textures/tables etc.)
; 	 
@t_2		TYPEDEF		PTR SBYTE
; 
; 	 * pointer to raw pixel data
; 	 
@t_3		TYPEDEF		PTR 
; 
; 	 * Optional pixel map when pixels are indexed.
; 	 
@t_4		TYPEDEF		PTR br_pixelmap
; 
; 	 * Byte difference between pixels at same column of adjacent rows
; 	 
; 
; 	 * Type of pixels
; 	 
; 
; 	 * Flags
; 	 
; 
; 	 * top left visible region in pixels from pixel at 'pixel' pointer
; 	 
; 
; 	 * Width and height of bitmap in pixels
; 	 
; 
; 	 * Local origin for any graphics system rendering into map, relative
; 	 * to 'base'
; 	 
; 
; 	 * Device pointer - if pixelmap originated from a device
; 	 
@t_5		TYPEDEF		PTR 
br_pixelmap		STRUCT 4t
identifier		@t_2		?
pixels		@t_3		?
_reserved0		br_uint_32		?
map		@t_4		?
row_bytes		br_int_16		?
_type		br_uint_8		?
flags		br_uint_8		?
base_x		br_uint_16		?
base_y		br_uint_16		?
_width		br_uint_16		?
height		br_uint_16		?
origin_x		br_int_16		?
origin_y		br_int_16		?
context		@t_5		?
br_pixelmap		ENDS

; 
;  * Flags to BrPixelMapAllocate
;  
BR_PMAF_INVERTED		EQU		1t
;  Setup pixelmap so that 0th scanline is at high memory 
;  Setup pixelmap so that 0th scanline is at low memory 
BR_PMAF_NORMAL		EQU		0t

; 
;  * Channel flags
;  
BR_PMCHAN_INDEX		EQU		1t
BR_PMCHAN_RGB		EQU		2t
BR_PMCHAN_DEPTH		EQU		4t
BR_PMCHAN_ALPHA		EQU		8t
BR_PMCHAN_YUV		EQU		16t

; 
;  * Matching pixelmap types
;  
BR_PMMATCH_OFFSCREEN		EQU		0t
BR_PMMATCH_DEPTH_16		EQU		1t
BR_PMMATCH_MAX		EQU		2t

; End of file ..\inc\pixelmap.h
; Begin of file ..\inc\brfont.h
; 
;  * Copyright (c) 1992,1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: brfont.h 1.4 1995/03/29 16:42:55 sam Exp $
;  * $Locker:  $
;  *
;  
@t_6		TYPEDEF		PTR br_int_8
@t_7		TYPEDEF		PTR br_uint_16
@t_8		TYPEDEF		PTR br_uint_8
br_font		STRUCT 4t
flags		br_uint_32		?
glyph_x		br_uint_16		?
glyph_y		br_uint_16		?
spacing_x		br_int_16		?
spacing_y		br_int_16		?
_width		@t_6		?
encoding		@t_7		?
glyphs		@t_8		?
br_font		ENDS

; 
;  * Flags
;  
BR_FONTF_PROPORTIONAL		EQU		1t
; 
;  * Default fonts that are available in framework
;  
EXTERNDEF		C BrFontFixed3x5:PTR br_font

EXTERNDEF		C BrFontProp4x6:PTR br_font

EXTERNDEF		C BrFontProp7x9:PTR br_font

; End of file ..\inc\brfont.h
; Begin of file ..\inc\material.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: material.h 1.19 1995/05/25 13:37:32 sam Exp $
;  * $Locker:  $
;  *
;  * Describes the appearance of a material than can be applied to a surface
;  
@t_9		TYPEDEF		PTR SBYTE
; 
; 	 * Specular power
; 	 
; 
; 	 * Controls how texture is rendered
; 	 
; 
; 	 * 2x3 transform applied to model U,V
; 	 
; 
; 	 * The direct index ramp base and range
; 	 
; 
; 	 * Pointers to colour map
; 	 
@t_10		TYPEDEF		PTR br_pixelmap
; 
; 	 * Pointers to tables
; 	 
@t_11		TYPEDEF		PTR br_pixelmap
;  Screen door opacity  
@t_12		TYPEDEF		PTR br_pixelmap
;  Index shading		
@t_13		TYPEDEF		PTR br_pixelmap
;  Index blending		
; 
; 	 * Private fields
; 	 
@t_14		TYPEDEF		PTR 
br_material		STRUCT 4t
identifier		@t_9		?
colour		br_colour		?
opacity		br_uint_8		?
ka		br_ufraction		?
kd		br_ufraction		?
ks		br_ufraction		?
power		br_scalar		?
flags		br_uint_32		?
map_transform		br_matrix23		<>
index_base		br_uint_8		?
index_range		br_uint_8		?
colour_map		@t_10		?
screendoor		@t_11		?
index_shade		@t_12		?
index_blend		@t_13		?
prep_flags		br_uint_8		?
rptr		@t_14		?
br_material		ENDS

; 
;  * Bits for br_material->flags
;  
BR_MATF_LIGHT		EQU		1t
BR_MATF_PRELIT		EQU		2t
BR_MATF_SMOOTH		EQU		4t
BR_MATF_ENVIRONMENT_I		EQU		8t
BR_MATF_ENVIRONMENT_L		EQU		16t
BR_MATF_PERSPECTIVE		EQU		32t
BR_MATF_DECAL		EQU		64t
BR_MATF_I_FROM_U		EQU		128t
BR_MATF_I_FROM_V		EQU		256t
BR_MATF_U_FROM_I		EQU		512t
BR_MATF_V_FROM_I		EQU		1024t
BR_MATF_ALWAYS_VISIBLE		EQU		2048t
BR_MATF_TWO_SIDED		EQU		4096t
BR_MATF_FORCE_Z_0		EQU		8192t
BR_MATF_DITHER		EQU		16384t

; 
;  * Backwards compatibility
;  
BR_MATF_MAP_COLOUR		EQU		0t
; 
;  * Flags to BrMaterialUpdate()
;  
BR_MATU_MAP_TRANSFORM		EQU		1t
BR_MATU_RENDERING		EQU		2t
BR_MATU_LIGHTING		EQU		4t
BR_MATU_COLOURMAP		EQU		8t
BR_MATU_ALL		EQU		32767t

; 
;  * Flags to BrMapUpdate()
;  
BR_MAPU_ALL		EQU		32767t

; 
;  * Flags to BrTableUpdate()
;  
BR_TABU_ALL		EQU		32767t

; End of file ..\inc\material.h
; Begin of file ..\inc\model.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: model.h 1.20 1995/08/31 16:36:18 sam Exp $
;  * $Locker:  $
;  *
;  * In-memory structures for models, both public and private areas
;  
;  Point in model space		
;  Mapping coordinates		
; 
; 	 * Index for prelit models
; 	 
; 
; 	 * true colour for pre-lit models
; 	 
; 
; 	 * Private fields
; 	 
;  Surface normal at vertex	
br_vertex		STRUCT 4t
p		br_vector3		<>
map		br_vector2		<>
index		br_uint_8		?
red		br_uint_8		?
grn		br_uint_8		?
blu		br_uint_8		?
r		br_uint_16		?
n		br_fvector3		<>
br_vertex		ENDS

;  Vertices around face 				
;  Edges around face					
@t_15		TYPEDEF		PTR br_material
;  Face material (or NULL) 				
;  Controls if shared edges are smooth	
;  Bits 0,1 and 2 denote internal edges	
;  Plane equation of face				
br_face		STRUCT 4t
vertices		br_uint_16		3t DUP (?)
edges		br_uint_16		3t DUP (?)
material		@t_15		?
smoothing		br_uint_16		?
flags		br_uint_8		?
_pad0		br_uint_8		?
n		br_fvector3		<>
d		br_scalar		?
br_face		ENDS

; 
;  * Bits for face flags
;  
BR_FACEF_COPLANAR_0		EQU		1t
;  The face adjoining edge 0 is coplanar with this face 
BR_FACEF_COPLANAR_1		EQU		2t
;          ""              1          ""                
;          ""              1          ""                
BR_FACEF_COPLANAR_2		EQU		4t

@t_16		TYPEDEF		PTR br_material
;  Group material (or NULL) 			
@t_17		TYPEDEF		PTR br_face
;  faces in group						
;  Number of faces in a group			
br_face_group		STRUCT 4t
material		@t_16		?
faces		@t_17		?
nfaces		br_uint_16		?
br_face_group		ENDS

@t_18		TYPEDEF		PTR br_material
;  Group material (or NULL) 			
@t_19		TYPEDEF		PTR br_vertex
;  vertices in group					
;  Number of vertices in a group		
br_vertex_group		STRUCT 4t
material		@t_18		?
vertices		@t_19		?
nvertices		br_uint_16		?
br_vertex_group		ENDS

; 
;  * Callback function type for custom models
;  
@proto_0		TYPEDEF		PROTO C :PTR br_actor, :PTR br_model, :PTR br_material, :PTR , :br_uint_8, :SDWORD, :PTR br_matrix34, :PTR br_matrix4
br_model_custom_cbfn		TYPEDEF		@proto_0

@t_20		TYPEDEF		PTR SBYTE
@t_21		TYPEDEF		PTR br_vertex
@t_22		TYPEDEF		PTR br_face
; 
; 	 * Offset of model's pivot point (where it attaches to parent)
; 	 
; 
; 	 * Flags describing what is allowed in ModelPrepare()
; 	 
; 
; 	 * Application call
; 	 
@t_23		TYPEDEF		PTR br_model_custom_cbfn
; 
; 	 * Application defined data - untouched by system
; 	 
@t_24		TYPEDEF		PTR 
; 
; 	 * Generated by ModelUpdate
; 	 
; 
; 	 * Bounding radius of model from origin
; 	 
; 
; 	 * Axis-aligned box that bound model in model coords
; 	 
; 
; 	 * Vertices and faces that have been sorted
; 	 * into groups, removing conflicts at material boundaries
; 	 * and smoothign groups
; 	 
@t_25		TYPEDEF		PTR br_face
@t_26		TYPEDEF		PTR br_vertex
; 
; 	 * Groups of faces and vertices, by material
; 	 
@t_27		TYPEDEF		PTR br_face_group
@t_28		TYPEDEF		PTR br_vertex_group
; 
; 	 * Upper limit on the face->edges[] entries
; 	 
; 
; 	 * Pointers to tables that map prepared face and vertex indexes
; 	 * back to original face index
; 	 *
; 	 * Only generated if BR_MODF_GENERATE_TAGS is set
; 	 
@t_29		TYPEDEF		PTR br_uint_16
@t_30		TYPEDEF		PTR br_uint_16
; 
; 	 * Private fields
; 	 
@t_31		TYPEDEF		PTR br_uint_16
@t_32		TYPEDEF		PTR 
br_model		STRUCT 4t
identifier		@t_20		?
vertices		@t_21		?
faces		@t_22		?
nvertices		br_uint_16		?
nfaces		br_uint_16		?
pivot		br_vector3		<>
flags		br_uint_16		?
custom		@t_23		?
user		@t_24		?
radius		br_scalar		?
bounds		br_bounds		<>
nprepared_vertices		br_uint_16		?
nprepared_faces		br_uint_16		?
prepared_faces		@t_25		?
prepared_vertices		@t_26		?
nface_groups		br_uint_16		?
nvertex_groups		br_uint_16		?
face_groups		@t_27		?
vertex_groups		@t_28		?
nedges		br_uint_16		?
face_tags		@t_29		?
vertex_tags		@t_30		?
prep_flags		br_uint_32		?
smooth_strings		@t_31		?
rptr		@t_32		?
br_model		ENDS

; 
;  * Bits for br_model->flags
;  
BR_MODF_DONT_WELD		EQU		1t
;  Vertices with same x,y,z cannot be merged	
BR_MODF_KEEP_ORIGINAL		EQU		2t
;  Don't release model->faces/vertices during ModelUpdate() 
BR_MODF_GENERATE_TAGS		EQU		4t
;  Allocate and fill in the face and vertex tag structures 
BR_MODF_QUICK_UPDATE		EQU		16t
;  ModelUpdate is fast - but may produce slower models 
BR_MODF_CUSTOM		EQU		32t
;  Invoke custom callback for this model 
;  Model structure is pre-prepared - update performs no work 
BR_MODF_PREPREPARED		EQU		64t

; 
;  * Flags to BrModelUpdate()
;  
BR_MODU_NORMALS		EQU		1t
BR_MODU_EDGES		EQU		2t
BR_MODU_RADIUS		EQU		4t
BR_MODU_GROUPS		EQU		8t
BR_MODU_BOUNDING_BOX		EQU		16t
BR_MODU_MATERIALS		EQU		32t
BR_MODU_ALL		EQU		32767t

; 
;  * Backwards compatibility
;  
; 
;  * Values for BrModelApplyMap()
;  
BR_APPLYMAP_PLANE		EQU		0t
BR_APPLYMAP_SPHERE		EQU		1t
BR_APPLYMAP_CYLINDER		EQU		2t
BR_APPLYMAP_DISC		EQU		3t
BR_APPLYMAP_NONE		EQU		4t

; 
;  * Axis values for BrModelFitMap()
;  
BR_FITMAP_PLUS_X		EQU		0t
BR_FITMAP_PLUS_Y		EQU		1t
BR_FITMAP_PLUS_Z		EQU		2t
BR_FITMAP_MINUS_X		EQU		3t
BR_FITMAP_MINUS_Y		EQU		4t
BR_FITMAP_MINUS_Z		EQU		5t

;  
;  * Local Variables:
;  * tab-width: 4
;  * End:
;  
; End of file ..\inc\model.h
; Begin of file ..\inc\actor.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: actor.h 1.15 1995/08/31 16:36:10 sam Exp $
;  * $Locker:  $
;  *
;  * Definitons for an Actor
;  
; *
;  ** Definition of base actor structure
;  *
; 
;  * Basic types of actor
;  
BR_ACTOR_NONE		EQU		0t
BR_ACTOR_MODEL		EQU		1t
BR_ACTOR_LIGHT		EQU		2t
BR_ACTOR_CAMERA		EQU		3t
_BR_ACTOR_RESERVED		EQU		4t
BR_ACTOR_BOUNDS		EQU		5t
BR_ACTOR_BOUNDS_CORRECT		EQU		6t
BR_ACTOR_CLIP_PLANE		EQU		7t
BR_ACTOR_MAX		EQU		8t

; 
;  * Render styles - an actor inherits it's style from the most _distant_
;  * ancestor included in this traversal that does not have default set
;  * (unlike model & material which are inherited from the nearest ancestor)
;  
BR_RSTYLE_DEFAULT		EQU		0t
BR_RSTYLE_NONE		EQU		1t
BR_RSTYLE_POINTS		EQU		2t
BR_RSTYLE_EDGES		EQU		3t
BR_RSTYLE_FACES		EQU		4t
BR_RSTYLE_BOUNDING_POINTS		EQU		5t
BR_RSTYLE_BOUNDING_EDGES		EQU		6t
BR_RSTYLE_BOUNDING_FACES		EQU		7t
BR_RSTYLE_MAX		EQU		8t

; 
;  * The actor structure
;  
; 
; 	 * Doubly linked list of siblings
; 	 * br_actor can be used as br_simple_node
; 	 
@t_33		TYPEDEF		PTR br_actor
@t_34		TYPEDEF		PTR PTR br_actor
; 
; 	 * First in list of children, or NULL if none
; 	 * br_actor.children can be used as br_simple_list
; 	 
@t_35		TYPEDEF		PTR br_actor
; 
; 	 * Parent, or NULL, if no higher parent
; 	 
@t_36		TYPEDEF		PTR br_actor
; 
; 	 * Depth of actor from furthest ancestor
; 	 
; 
; 	 * Type of actor
; 	 
; 
; 	 * Optional name of actor
; 	 
@t_37		TYPEDEF		PTR SBYTE
; 
; 	 * Reference to model associated with this actor, NULL will inherit
; 	 * from parent (root inherits default_model)
; 	 
@t_38		TYPEDEF		PTR br_model
; 
; 	 * Default material for model - NULL will inherit from parent. Root
; 	 * inherits default material
; 	 
@t_39		TYPEDEF		PTR br_material
; 
; 	 * Render style for actor, and all it's decendants if not BR_RSTYLE_DEFAULT
; 	 
; 
; 	 * Reference to renderer specific data associated with this actor - NULL will
; 	 * inherit from parent (root inherits default data)
; 	 
@t_40		TYPEDEF		PTR 
; 
; 	 * Postiton of this actor within parent space
; 	 
; 
; 	 * Reference to any type specific data block
; 	 
@t_41		TYPEDEF		PTR 
br_actor		STRUCT 4t
next		@t_33		?
prev		@t_34		?
children		@t_35		?
parent		@t_36		?
depth		br_uint_16		?
_type		br_uint_8		?
identifier		@t_37		?
model		@t_38		?
material		@t_39		?
render_style		br_uint_8		?
render_data		@t_40		?
t		br_transform		<>
type_data		@t_41		?
br_actor		ENDS

; End of file ..\inc\actor.h
; Begin of file ..\inc\camera.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: camera.h 1.4 1995/05/25 13:37:27 sam Exp $
;  * $Locker:  $
;  *
;  * Definitons for a camera
;  
BR_CAMERA_PARALLEL		EQU		0t
BR_CAMERA_PERSPECTIVE_FOV		EQU		1t
BR_CAMERA_PERSPECTIVE_WHD		EQU		2t

; 
;  * Backwards compatibility
;  
; 
; 	 * Optional identifier
; 	 
@t_42		TYPEDEF		PTR SBYTE
; 
; 	 * Type of camera
; 	 
; 
; 	 * Field of view
; 	 * (BR_CAMERA_PERSPECTIVE_FOV only)
; 	 
; 
; 	 * Front and back of view volume in view coordinates
; 	 
; 
; 	 * Aspect ratio of viewport
; 	 * (BR_CAMERA_PERSPECTIVE_FOV only)
; 	 
; 
; 	 * Width and height of projection surface
; 	 * (BR_CAMERA_PERSPECTIVE_WHD and BR_CAMERA_PARALLEL only)
; 	 
; 
; 	 * Distance of projection plane from center of projection
; 	 * (BR_CAMERA_PERSPECTIVE_WHD only)
; 	 
br_camera		STRUCT 4t
identifier		@t_42		?
_type		br_uint_8		?
field_of_view		br_angle		?
hither_z		br_scalar		?
yon_z		br_scalar		?
aspect		br_scalar		?
_width		br_scalar		?
height		br_scalar		?
distance		br_scalar		?
br_camera		ENDS

; End of file ..\inc\camera.h
; Begin of file ..\inc\light.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: light.h 1.6 1995/05/25 13:37:32 sam Exp $
;  * $Locker:  $
;  *
;  * Definitons for a light
;  
; 
; 	 * Type of light
; 	 
BR_LIGHT_TYPE		EQU		3t
BR_LIGHT_POINT		EQU		0t
BR_LIGHT_DIRECT		EQU		1t
BR_LIGHT_SPOT		EQU		2t
; 
;      * Flag idicating that caluculations are done in view space
;      
BR_LIGHT_VIEW		EQU		4t

; 
; 	 * Optional identifier
; 	 
@t_43		TYPEDEF		PTR SBYTE
; 
; 	 * Type of light
; 	 
; 
; 	 * Colour of light (if renderer supports it)
; 	 
; 
; 	 * Attenuation of light with distance - constant, linear, and quadratic
; 	 * l & q only apply to point and spot lights
; 	 
; 
; 	 * Cone angles for spot light
; 	 
br_light		STRUCT 4t
identifier		@t_43		?
_type		br_uint_8		?
colour		br_colour		?
attenuation_c		br_scalar		?
attenuation_l		br_scalar		?
attenuation_q		br_scalar		?
cone_outer		br_angle		?
cone_inner		br_angle		?
br_light		ENDS

; End of file ..\inc\light.h
; Begin of file ..\inc\pool.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: pool.h 1.7 1995/02/22 21:37:18 sam Exp $
;  * $Locker:  $
;  *
;  * Fixed size block pools
;  
BR_POOL_DEBUG		EQU		0t
BR_POOL_ALIGN		EQU		7t
@t_44		TYPEDEF		PTR br_pool_block
br_pool_block		STRUCT 4t
next		@t_44		?
br_pool_block		ENDS

@t_45		TYPEDEF		PTR br_pool_block
br_pool		STRUCT 4t
free		@t_45		?
block_size		br_uint_32		?
chunk_size		br_uint_32		?
mem_type		SDWORD		?
br_pool		ENDS

; 
;  * Speedup macros
;  
; End of file ..\inc\pool.h
; Begin of file ..\inc\custsupt.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: custsupt.h 1.2 1995/02/22 21:37:05 sam Exp $
;  * $Locker:  $
;  *
;  * Definitions for support routines available during render callbacks
;  *
;  
; 
;  * Outcode bits
;  
OUTCODE_LEFT		EQU		000000001h
OUTCODE_RIGHT		EQU		000000002h
OUTCODE_TOP		EQU		000000004h
OUTCODE_BOTTOM		EQU		000000008h
OUTCODE_HITHER		EQU		000000010h
OUTCODE_YON		EQU		000000020h
OUTCODE_USER		EQU		000000040h
OUTCODE_USER_ALL		EQU		000000fc0h
OUTCODES_ALL		EQU		000000fffh
OUTCODE_N_LEFT		EQU		000010000h
OUTCODE_N_RIGHT		EQU		000020000h
OUTCODE_N_TOP		EQU		000040000h
OUTCODE_N_BOTTOM		EQU		000080000h
OUTCODE_N_HITHER		EQU		000100000h
OUTCODE_N_YON		EQU		000200000h
OUTCODE_N_USER		EQU		000400000h
OUTCODE_N_USER_ALL		EQU		00fc00000h
OUTCODES_NOT		EQU		00fff0000h
; 
;  * Values for on screen test
;  
OSC_REJECT		EQU		0t
OSC_PARTIAL		EQU		1t
OSC_ACCEPT		EQU		2t

; End of file ..\inc\custsupt.h
; Begin of file ..\inc\brfile.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: brfile.h 1.7 1995/02/22 21:36:59 sam Exp $
;  * $Locker:  $
;  *
;  * Brender's interface to file IO
;  
@proto_1		TYPEDEF		PROTO C :PTR br_uint_8, :br_size_t
br_mode_test_cbfn		TYPEDEF		@proto_1

; 
;  * Interface to filesystem
;  
@proto_2		TYPEDEF		PROTO C 
brfile_attributes_cbfn		TYPEDEF		@proto_2

@proto_3		TYPEDEF		PROTO C :PTR SBYTE, :br_size_t, :PTR br_mode_test_cbfn, :PTR SDWORD
brfile_open_read_cbfn		TYPEDEF		@proto_3

@proto_4		TYPEDEF		PROTO C :PTR SBYTE, :SDWORD
brfile_open_write_cbfn		TYPEDEF		@proto_4

@proto_5		TYPEDEF		PROTO C :PTR 
brfile_close_cbfn		TYPEDEF		@proto_5

@proto_6		TYPEDEF		PROTO C :PTR 
brfile_eof_cbfn		TYPEDEF		@proto_6

@proto_7		TYPEDEF		PROTO C :PTR 
brfile_getchr_cbfn		TYPEDEF		@proto_7

@proto_8		TYPEDEF		PROTO C :SDWORD, :PTR 
brfile_putchr_cbfn		TYPEDEF		@proto_8

@proto_9		TYPEDEF		PROTO C :PTR , :br_size_t, :DWORD, :PTR 
brfile_read_cbfn		TYPEDEF		@proto_9

@proto_10		TYPEDEF		PROTO C :PTR , :br_size_t, :DWORD, :PTR 
brfile_write_cbfn		TYPEDEF		@proto_10

@proto_11		TYPEDEF		PROTO C :PTR SBYTE, :br_size_t, :PTR 
brfile_getline_cbfn		TYPEDEF		@proto_11

@proto_12		TYPEDEF		PROTO C :PTR SBYTE, :PTR 
brfile_putline_cbfn		TYPEDEF		@proto_12

@proto_13		TYPEDEF		PROTO C :br_size_t, :PTR 
brfile_advance_cbfn		TYPEDEF		@proto_13

@t_46		TYPEDEF		PTR SBYTE
; 
; 	 * Inquire about attributes of file system
; 	 
@t_47		TYPEDEF		PTR brfile_attributes_cbfn
; 
; 	 * Locate and open a file, and then grab 'n_magics' bytes
; 	 * from the front of the file and pass them to a an 'identify'
; 	 * function that returns the mode of the file
; 	 *
; 	 * Returns file handle, or NULL if open failed
; 	 *
; 	 * Stores open mode through mode_result if not NULL
; 	 
@t_48		TYPEDEF		PTR brfile_open_read_cbfn
; 
; 	 * Open a file for writing, possibly with text translation
; 	 *
; 	 * Returns fil handle, or NULL if open failed
; 	 
@t_49		TYPEDEF		PTR brfile_open_write_cbfn
; 
; 	 * Close an opened file
; 	 
@t_50		TYPEDEF		PTR brfile_close_cbfn
; 
; 	 * Find out if at end of file
; 	 
@t_51		TYPEDEF		PTR brfile_eof_cbfn
; 
; 	 * Read and write single characters
; 	 
@t_52		TYPEDEF		PTR brfile_getchr_cbfn
@t_53		TYPEDEF		PTR brfile_putchr_cbfn
; 
; 	 * Read and write blocks
; 	 
@t_54		TYPEDEF		PTR brfile_read_cbfn
@t_55		TYPEDEF		PTR brfile_write_cbfn
; 
; 	 * Read and write lines of text, excluding any line terminators
; 	 * Will only be used if  text translation is in use
; 	 
@t_56		TYPEDEF		PTR brfile_getline_cbfn
@t_57		TYPEDEF		PTR brfile_putline_cbfn
; 
; 	 * Advance <count> bytes through stream
; 	 *
; 	 * Will not be invoked if text translation is in use
; 	 
@t_58		TYPEDEF		PTR brfile_advance_cbfn
br_filesystem		STRUCT 4t
identifier		@t_46		?
attributes		@t_47		?
open_read		@t_48		?
open_write		@t_49		?
close		@t_50		?
eof		@t_51		?
getchr		@t_52		?
putchr		@t_53		?
read		@t_54		?
write		@t_55		?
getline		@t_56		?
putline		@t_57		?
advance		@t_58		?
br_filesystem		ENDS

; 
;  * Bitmask returned by fs->attributes
;  
BR_FS_ATTR_READABLE		EQU		1t
BR_FS_ATTR_WRITEABLE		EQU		2t
BR_FS_ATTR_HAS_TEXT		EQU		4t
BR_FS_ATTR_HAS_BINARY		EQU		8t
BR_FS_ATTR_HAS_ADVANCE		EQU		16t

; 
;  * Possible values returner by open_read identify callback
;  
BR_FS_MODE_BINARY		EQU		0t
BR_FS_MODE_TEXT		EQU		1t
BR_FS_MODE_UNKNOWN		EQU		2t

; 
;  * Maximum number of magic bytes that can be requested on open_read
;  
BR_MAX_FILE_MAGICS		EQU		16t
; 
;  * Returned by filesys->getchr at end of file
;  
BR_EOF		EQU		0ffffffffh
; End of file ..\inc\brfile.h
; Begin of file ..\inc\brdiag.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: brdiag.h 1.1 1995/07/28 18:57:13 sam Exp $
;  * $Locker:  $
;  *
;  * Brender's interface to diagnostic handling
;  
; 
;  * Instance of an error handler
;  
@proto_14		TYPEDEF		PROTO C :PTR SBYTE
br_diag_warning_cbfn		TYPEDEF		@proto_14

@proto_15		TYPEDEF		PROTO C :PTR SBYTE
br_diag_failure_cbfn		TYPEDEF		@proto_15

@t_59		TYPEDEF		PTR SBYTE
@t_60		TYPEDEF		PTR br_diag_warning_cbfn
@t_61		TYPEDEF		PTR br_diag_failure_cbfn
br_diaghandler		STRUCT 4t
identifier		@t_59		?
warning		@t_60		?
failure		@t_61		?
br_diaghandler		ENDS

; 
;  * For backwards compatibility
;  
@t_62		TYPEDEF		PTR SBYTE
@t_63		TYPEDEF		PTR br_diag_warning_cbfn
@t_64		TYPEDEF		PTR br_diag_failure_cbfn
br_errorhandler		STRUCT 4t
identifier		@t_62		?
message		@t_63		?
error		@t_64		?
br_errorhandler		ENDS

; *
;  ** Macros for diagnostic generation
;  *
; 
;  * Report message and exit - should not return to application
;  
; 
;  * Report message and continue
;  
; 
;  * Report message and exit, including source file and line number
;  
; 
;  * Backwards compatibility
;  
; End of file ..\inc\brdiag.h
; Begin of file ..\inc\brmem.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: brmem.h 1.15 1995/08/31 16:36:13 sam Exp $
;  * $Locker:  $
;  *
;  * Brender's interface to memory allocation
;  
; 
;  * Instance of a memory allocator
;  
@proto_16		TYPEDEF		PROTO C :br_size_t, :br_uint_8
brmem_allocate_cbfn		TYPEDEF		@proto_16

@proto_17		TYPEDEF		PROTO C :PTR 
brmem_free_cbfn		TYPEDEF		@proto_17

@proto_18		TYPEDEF		PROTO C :br_uint_8
brmem_inquire_cbfn		TYPEDEF		@proto_18

@t_65		TYPEDEF		PTR SBYTE
; 
; 	 * Allocate a new chunk of memory - must return the new pointer
; 	 * or handle the error itself
; 	 
@t_66		TYPEDEF		PTR brmem_allocate_cbfn
; 
; 	 * Release previously allocated block
; 	 
@t_67		TYPEDEF		PTR brmem_free_cbfn
; 
; 	 * Inquire as to the amount of memeory available for a given type
; 	 
@t_68		TYPEDEF		PTR brmem_inquire_cbfn
br_allocator		STRUCT 4t
identifier		@t_65		?
allocate		@t_66		?
free		@t_67		?
inquire		@t_68		?
br_allocator		ENDS

; 
;  * Classes of resource that brender allocates
;  *
;  * Valid values are 1 to 255
;  
; 
; 	 * System classes
; 	 
BR_MEMORY_SCRATCH		EQU		1t
BR_MEMORY_PIXELMAP		EQU		2t
BR_MEMORY_PIXELS		EQU		3t
BR_MEMORY_VERTICES		EQU		4t
BR_MEMORY_FACES		EQU		5t
BR_MEMORY_GROUPS		EQU		6t
BR_MEMORY_MODEL		EQU		7t
BR_MEMORY_MATERIAL		EQU		8t
BR_MEMORY_MATERIAL_INDEX		EQU		9t
BR_MEMORY_ACTOR		EQU		10t
BR_MEMORY_PREPARED_VERTICES		EQU		11t
BR_MEMORY_PREPARED_FACES		EQU		12t
BR_MEMORY_LIGHT		EQU		13t
BR_MEMORY_CAMERA		EQU		14t
BR_MEMORY_BOUNDS		EQU		15t
BR_MEMORY_CLIP_PLANE		EQU		16t
BR_MEMORY_STRING		EQU		17t
BR_MEMORY_REGISTRY		EQU		18t
BR_MEMORY_TRANSFORM		EQU		19t
BR_MEMORY_RESOURCE_CLASS		EQU		20t
BR_MEMORY_FILE		EQU		21t
BR_MEMORY_ANCHOR		EQU		22t
BR_MEMORY_POOL		EQU		23t
BR_MEMORY_RENDER_MATERIAL		EQU		24t
BR_MEMORY_DATAFILE		EQU		25t
BR_MEMORY_IMAGE		EQU		26t
BR_MEMORY_IMAGE_ARENA		EQU		27t
BR_MEMORY_IMAGE_SECTIONS		EQU		28t
BR_MEMORY_IMAGE_NAMES		EQU		29t
BR_MEMORY_EXCEPTION_HANDLER		EQU		30t
BR_MEMORY_RENDER_DATA		EQU		31t
; 
; 	 * Application classes
; 	 
BR_MEMORY_APPLICATION		EQU		128t
; 
; 	 * User defined classed are BR_MEMORY_APPLICATION + 1 ... 127
; 	 
BR_MEMORY_MAX		EQU		256t

; 
;  * A resource class structure
;  
@proto_19		TYPEDEF		PROTO C :PTR , :br_uint_8, :br_size_t
br_resourcefree_cbfn		TYPEDEF		@proto_19

@t_69		TYPEDEF		PTR SBYTE
@t_70		TYPEDEF		PTR br_resourcefree_cbfn
br_resource_class		STRUCT 4t
identifier		@t_69		?
res_class		br_uint_8		?
free_cb		@t_70		?
br_resource_class		ENDS

; End of file ..\inc\brmem.h
; Begin of file ..\inc\brerr.h
; 
;  * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: brerr.h 1.4 1995/07/28 18:57:02 sam Exp $
;  * $Locker:  $
;  *
;  * Error type
;  
; 
;  * Error types are 32 bit integers, broken into three fields -
;  *
;  * Class	Subclass Number
;  *
;  * 33222222 22221111 1111110000000000
;  * 10987654 32109876 5432109876543210
;  *
;  * A value of zero is reserved to indiciate no error
;  
br_error		TYPEDEF		br_uint_32

; End of file ..\inc\brerr.h
; Begin of file ..\inc\fmt.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: fmt.h 1.5 1995/02/22 21:37:06 sam Exp $
;  * $Locker:  $
;  *
;  * Definitions for foreign file format support
;  
; 
;  * Type of things to load from file
;  
BR_FMT_MODELS		EQU		1t
;  Create model definitions from file	
BR_FMT_ACTORS		EQU		2t
;  Create actor							
BR_FMT_LIGHTS		EQU		4t
;  Create lights instances				
BR_FMT_CAMERAS		EQU		8t
;  Create cameras instances				
;  Create materials						
BR_FMT_MATERIALS		EQU		16t

; 
;  * Interchange structure for passing to file format loaders
;  *
;  * XXX Not yet used by anything
;  
; 
; 	 * pointers to tables of each loaded allocated by BrFMTxxxLoad()
; 	 
@t_71		TYPEDEF		PTR br_model
@t_72		TYPEDEF		PTR br_model
@t_73		TYPEDEF		PTR br_model
@t_74		TYPEDEF		PTR br_model
@t_75		TYPEDEF		PTR br_model
; 
; 	 * Size of each allocated array
; 	 
br_fmt_results		STRUCT 4t
models		@t_71		?
materials		@t_72		?
lights		@t_73		?
cameras		@t_74		?
actors		@t_75		?
nmodels		SDWORD		?
nmaterials		SDWORD		?
nlights		SDWORD		?
ncameras		SDWORD		?
nactors		SDWORD		?
br_fmt_results		ENDS

; Begin of file ..\inc\fmtproto.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: fmtproto.h 1.8 1995/05/30 16:57:38 sam Exp $
;  * $Locker:  $
;  *
;  * Function prototypes for foreign file format support
;  
; 
;  * Eric Haines' NFF format
;  
@proto_20		TYPEDEF		PROTO C :PTR SBYTE
BrFmtNFFLoad		PROTO		@proto_20

; 
;  * 3D Studio .ASC files
;  
@proto_21		TYPEDEF		PROTO C :PTR SBYTE, :PTR PTR br_model, :br_uint_16
BrFmtASCLoad		PROTO		@proto_21

; 
;  * Script files
;  
@proto_22		TYPEDEF		PROTO C :PTR SBYTE, :PTR PTR br_material, :br_uint_16
BrFmtScriptMaterialLoadMany		PROTO		@proto_22

@proto_23		TYPEDEF		PROTO C :PTR SBYTE
BrFmtScriptMaterialLoad		PROTO		@proto_23

@proto_24		TYPEDEF		PROTO C :PTR SBYTE, :PTR PTR br_material, :br_uint_16
BrFmtScriptMaterialSaveMany		PROTO		@proto_24

@proto_25		TYPEDEF		PROTO C :PTR SBYTE, :PTR br_material
BrFmtScriptMaterialSave		PROTO		@proto_25

; 
;  * Windows .BMP files
;  
@proto_26		TYPEDEF		PROTO C :PTR SBYTE, :br_uint_32
BrFmtBMPLoad		PROTO		@proto_26

; 
;  * .TGA files
;  
@proto_27		TYPEDEF		PROTO C :PTR SBYTE, :br_uint_32
BrFmtTGALoad		PROTO		@proto_27

; 
;  * .GIF files
;  
@proto_28		TYPEDEF		PROTO C :PTR SBYTE, :br_uint_32
BrFmtGIFLoad		PROTO		@proto_28

; 
;  * .IFF/.LBM files
;  
@proto_29		TYPEDEF		PROTO C :PTR SBYTE, :br_uint_32
BrFmtIFFLoad		PROTO		@proto_29

; End of file ..\inc\fmtproto.h
; End of file ..\inc\fmt.h
; 
; 	 * Function prototypes
; 	 
; Begin of file ..\inc\fwproto.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: fwproto.h 1.44 1995/08/31 16:36:16 sam Exp $
;  * $Locker: sam $
;  *
;  * Function prototypes for brender framework
;  
; End of file ..\inc\fwproto.h
; Begin of file ..\inc\zbproto.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: zbproto.h 1.9 1995/08/31 16:36:24 sam Exp $
;  * $Locker:  $
;  *
;  * Function prototypes for Z buffer renderer
;  
BR_BOUNDS_MIN_X		EQU		0t
BR_BOUNDS_MIN_Y		EQU		1t
BR_BOUNDS_MAX_X		EQU		2t
BR_BOUNDS_MAX_Y		EQU		3t
; 
;  * Callback function invoked when an actor is
;  * rendered
;  
@proto_30		TYPEDEF		PROTO C :PTR br_actor, :PTR br_model, :PTR br_material, :PTR , :br_uint_8, :PTR br_matrix4, :PTR br_int_32
br_renderbounds_cbfn		TYPEDEF		@proto_30

; End of file ..\inc\zbproto.h
; Begin of file ..\inc\zsproto.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: zsproto.h 1.1 1995/08/31 16:37:20 sam Exp $
;  * $Locker:  $
;  *
;  * Function prototypes for bucket Z-sort renderer
;  
; 
;  * Sort types for primitive insertion
;  
BR_ZSORT_MIN		EQU		1t
BR_ZSORT_MAX		EQU		2t
BR_ZSORT_AVERAGE		EQU		4t

; 
;  * Callback function invoked when primitive is generated
;  
@proto_31		TYPEDEF		PROTO C :PTR , :PTR br_actor, :PTR br_model, :PTR br_material, :PTR br_order_table, :PTR br_scalar
br_primitive_cbfn		TYPEDEF		@proto_31

; End of file ..\inc\zsproto.h
; Begin of file ..\inc\fwpextra.h
; 
;  * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
;  *
;  * Extra glue for C<->ASM generated via gemasmpr.pl
;  
; End of file ..\inc\fwpextra.h
; 
; 	 * Inline functions
; 	 
; Begin of file ..\inc\compend.h
; 
;  * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: compend.h 1.1 1995/08/31 16:36:45 sam Exp $
;  * $Locker:  $
;  *
;  * Included at end of BRender includes - undoes any compiler specific pragmas
;  
; 
;  * WATCOM C/C++ 32
;  
; 
;  * Metaware High-C Version 1
;  
; End of file ..\inc\compend.h
; End of file ..\inc\brender.h
; Begin of file ..\fw\brlists.h
; 
;  * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: brlists.h 1.4 1995/02/22 21:41:25 sam Exp $
;  * $Locker:  $
;  *
;  * Doubly linked list supoprt (Based on Amiga EXEC!)
;  
; *
;  **	Full featued doubly linked lists
;  *
; 
;  * Anchor structure at front of list entries
;  
@t_76		TYPEDEF		PTR br_node
@t_77		TYPEDEF		PTR br_node
br_node		STRUCT 4t
next		@t_76		?
prev		@t_77		?
br_node		ENDS

; 
;  * Anchor structure for whole list - looks like two nodes overlapping.
;  * The _null memeber is always NULL, providing the termination at
;  * both ends of the list
;  
@t_78		TYPEDEF		PTR br_node
@t_79		TYPEDEF		PTR br_node
@t_80		TYPEDEF		PTR br_node
br_list		STRUCT 4t
head		@t_78		?
_null		@t_79		?
tail		@t_80		?
br_list		ENDS

; 
;  * Walk a pointer forwards along a list
;  *
;  * eg:
;  *	BR_FOR_LIST(sprites,sptr) {
;  *		sptr->x = 10;
;  *	}
;  
; 
;  * Walk a pointer backwards along a list
;  *
;  * eg:
;  *	BR_FOR_LIST_R(sprites,sptr) {
;  *		sptr->x = 10;
;  *	}
;  
; 
;  * Macros around exported functions that hide type casting
;  
; *
;  **	Simple lists that only use a single head pointer
;  *
@t_81		TYPEDEF		PTR br_simple_node
@t_82		TYPEDEF		PTR PTR br_simple_node
br_simple_node		STRUCT 4t
next		@t_81		?
prev		@t_82		?
br_simple_node		ENDS

; 
;  * Anchor structure for whole list - just a single head pointer
;  
@t_83		TYPEDEF		PTR br_simple_node
br_simple_list		STRUCT 4t
head		@t_83		?
br_simple_list		ENDS

; 
;  * Macros around exported functions that hide type casting
;  
; 
;  * Protoypes in lists.c
;  
; End of file ..\fw\brlists.h
; Begin of file ..\fw\brexcept.h
; 
;  * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: brexcept.h 1.2 1995/07/28 19:01:35 sam Exp $
;  * $Locker:  $
;  *
;  * Simple exception handling
;  
; 
;  * Exception types are the same as error types
;  *
;  * A value of zero is reserved
;  
br_exception		TYPEDEF		br_error

; 
;  * Useful macros for generating types
;  
; 
;  * Exception handler - allocated as a resource 
;  
@t_84		TYPEDEF		PTR br_exception_handler
; 
; 	 * setjmp/longjmp context to throw to
; 	 
br_exception_handler		STRUCT 4t
prev		@t_84		?
br_exception_handler		ENDS

; 
;  * Public macros
;  
; End of file ..\fw\brexcept.h
; Begin of file ..\fw\brimage.h
; 
;  * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: brimage.h 1.1 1995/07/28 19:03:11 sam Exp $
;  * $Locker:  $
;  *
;  
; 
;  * In-memory structure describing a loaded image
;  
@t_85		TYPEDEF		PTR SBYTE
@t_86		TYPEDEF		PTR 
br_image_section		STRUCT 4t
name		@t_85		?
base		@t_86		?
mem_offset		br_size_t		?
mem_size		br_size_t		?
data_offset		br_uint_32		?
data_size		br_uint_32		?
br_image_section		ENDS

; 
; 	 * Anchor block for list of images
; 	 
; 
; 	 * DLL name
; 	 
@t_87		TYPEDEF		PTR SBYTE
; 
; 	 * Flag, true if DLL is resident
; 	 
; 
; 	 * Number of references to this DLL
; 	 
; 
; 	 * Table of exported functions
; 	 
@t_88		TYPEDEF		PTR PTR 
; 
; 	 * Name -> ordinal lookup
; 	 
@t_89		TYPEDEF		PTR PTR SBYTE
@t_90		TYPEDEF		PTR br_uint_16
; 
; 	 * Table of imported image pointers
; 	 
@t_91		TYPEDEF		PTR PTR br_image
; 
; 	 * Image sections
; 	 
@t_92		TYPEDEF		PTR br_image_section
br_image		STRUCT 4t
node		br_node		<>
identifier		@t_87		?
resident		br_boolean		?
ref_count		SDWORD		?
ordinal_base		br_uint_32		?
n_functions		br_uint_32		?
functions		@t_88		?
n_names		br_uint_32		?
names		@t_89		?
name_ordinals		@t_90		?
n_imports		br_uint_16		?
imports		@t_91		?
n_sections		br_uint_16		?
sections		@t_92		?
br_image		ENDS

; End of file ..\fw\brimage.h
; Begin of file ..\fw\register.h
; 
;  * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: register.h 1.7 1995/03/16 11:57:08 sam Exp $
;  * $Locker:  $
;  *
;  * 
;  
; 
;  * An entry in a registry - doubly linked list and pointers to data
;  *
;  * It is assumed that the first thing in the data structure is a pointer
;  * to the item's name
;  
@t_93		TYPEDEF		PTR PTR SBYTE
br_registry_entry		STRUCT 4t
node		br_node		<>
item		@t_93		?
br_registry_entry		ENDS

@proto_32		TYPEDEF		PROTO C :PTR SBYTE
br_find_failed_cbfn		TYPEDEF		@proto_32

@proto_33		TYPEDEF		PROTO C :PTR , :PTR 
br_enum_cbfn		TYPEDEF		@proto_33

; 
;  * Base structure for registry
;  
; 
; 		 * Anchor structure
; 		 
; 
; 		 * Number of items in list
; 		 
; 
; 		 * Hook that is called when RegistryFind fails
; 		 
@t_94		TYPEDEF		PTR br_find_failed_cbfn
br_registery		STRUCT 4t
list		br_list		<>
count		SDWORD		?
find_failed_hook		@t_94		?
br_registery		ENDS

br_registry		TYPEDEF		br_registery

; End of file ..\fw\register.h
; Begin of file ..\fw\device.h
; 
;  * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
;  *
;  * $Id: device.h 1.5 1995/07/28 19:01:38 sam Exp $
;  * $Locker:  $
;  *
;  * Device structure - a function dispatch table for pixelmap operations
;  
; 
;  * function list for pixelmaps->context
;  
@proto_34		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_uint_32
ctx_fill_cbfn		TYPEDEF		@proto_34

@proto_35		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_32
ctx_rectangle_cbfn		TYPEDEF		@proto_35

@proto_36		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_32, :br_uint_32
ctx_rectangle2_cbfn		TYPEDEF		@proto_36

@proto_37		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_uint_16, :br_uint_16, :PTR br_pixelmap, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_16
ctx_rectangle_copy_cbfn		TYPEDEF		@proto_37

@proto_38		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_32
ctx_rectangle_fill_cbfn		TYPEDEF		@proto_38

@proto_39		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :PTR br_pixelmap, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_16
ctx_dirty_rectangle_copy_cbfn		TYPEDEF		@proto_39

@proto_40		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_32
ctx_dirty_rectangle_fill_cbfn		TYPEDEF		@proto_40

@proto_41		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_uint_16, :br_uint_16, :br_uint_32
ctx_pixel_set_cbfn		TYPEDEF		@proto_41

@proto_42		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_uint_16, :br_uint_16
ctx_pixel_get_cbfn		TYPEDEF		@proto_42

@proto_43		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :PTR br_pixelmap
ctx_copy_cbfn		TYPEDEF		@proto_43

@proto_44		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_int_16, :br_int_16, :br_int_16, :br_int_16, :br_uint_32
ctx_line_cbfn		TYPEDEF		@proto_44

@proto_45		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_int_16, :br_int_16, :PTR br_uint_8, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_16, :br_uint_32
ctx_copy_bits_cbfn		TYPEDEF		@proto_45

@proto_46		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :PTR br_pixelmap
ctx_double_buffer_cbfn		TYPEDEF		@proto_46

@proto_47		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap, :br_uint_8
ctx_match_cbfn		TYPEDEF		@proto_47

@proto_48		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap
ctx_clone_cbfn		TYPEDEF		@proto_48

@proto_49		TYPEDEF		PROTO C :PTR br_context, :PTR br_pixelmap
ctx_free_cbfn		TYPEDEF		@proto_49

; 
;  * Device context structure
;  
; 
; 	 * General ID for this structure
; 	 
@t_95		TYPEDEF		PTR SBYTE
; 
; 	 * Pointer to underlying device
; 	 
@t_96		TYPEDEF		PTR br_device
; 
; 	 * Font for context
; 	 
@t_97		TYPEDEF		PTR br_font
; 
; 	 * Default qualifier for display memory
; 	 
; *
; 	 ** Function pointers for all the context operations
; 	 *
; 
; 	 * Pixelmap management operations
; 	 
@t_98		TYPEDEF		PTR ctx_free_cbfn
@t_99		TYPEDEF		PTR ctx_match_cbfn
@t_100		TYPEDEF		PTR ctx_clone_cbfn
@t_101		TYPEDEF		PTR ctx_double_buffer_cbfn
; 
; 	 * Pixelmap copying
; 	 
@t_102		TYPEDEF		PTR ctx_copy_cbfn
@t_103		TYPEDEF		PTR ctx_copy_cbfn
@t_104		TYPEDEF		PTR ctx_copy_cbfn
; 
; 	 * Rendering operations
; 	 
@t_105		TYPEDEF		PTR ctx_fill_cbfn
@t_106		TYPEDEF		PTR ctx_rectangle_cbfn
@t_107		TYPEDEF		PTR ctx_rectangle2_cbfn
@t_108		TYPEDEF		PTR ctx_rectangle_copy_cbfn
@t_109		TYPEDEF		PTR ctx_rectangle_copy_cbfn
@t_110		TYPEDEF		PTR ctx_rectangle_copy_cbfn
@t_111		TYPEDEF		PTR ctx_rectangle_fill_cbfn
@t_112		TYPEDEF		PTR ctx_dirty_rectangle_copy_cbfn
@t_113		TYPEDEF		PTR ctx_dirty_rectangle_fill_cbfn
@t_114		TYPEDEF		PTR ctx_pixel_set_cbfn
@t_115		TYPEDEF		PTR ctx_pixel_get_cbfn
@t_116		TYPEDEF		PTR ctx_line_cbfn
@t_117		TYPEDEF		PTR ctx_copy_bits_cbfn
br_context		STRUCT 4t
identifier		@t_95		?
device		@t_96		?
font		@t_97		?
free		@t_98		?
match		@t_99		?
clone		@t_100		?
double_buffer		@t_101		?
copy		@t_102		?
copy_to		@t_103		?
copy_from		@t_104		?
fill		@t_105		?
rectangle		@t_106		?
rectangle2		@t_107		?
rectangle_copy		@t_108		?
rectangle_copy_to		@t_109		?
rectangle_copy_from		@t_110		?
rectangle_fill		@t_111		?
dirty_rectangle_copy		@t_112		?
dirty_rectangle_fill		@t_113		?
pixel_set		@t_114		?
pixel_get		@t_115		?
line		@t_116		?
copy_bits		@t_117		?
br_context		ENDS

; 
;  * Generic device class structure
;  
@proto_50		TYPEDEF		PROTO C :PTR br_device
dev_load_cbfn		TYPEDEF		@proto_50

@proto_51		TYPEDEF		PROTO C :PTR br_device
dev_unload_cbfn		TYPEDEF		@proto_51

@proto_52		TYPEDEF		PROTO C :PTR br_device
dev_enquire_cbfn		TYPEDEF		@proto_52

@proto_53		TYPEDEF		PROTO C :PTR br_device
dev_create_cbfn		TYPEDEF		@proto_53

; 
; 	 * Loaded devices are linked into a list
; 	 
@t_118		TYPEDEF		PTR SBYTE
@t_119		TYPEDEF		PTR dev_enquire_cbfn
@t_120		TYPEDEF		PTR dev_load_cbfn
@t_121		TYPEDEF		PTR dev_unload_cbfn
@t_122		TYPEDEF		PTR dev_create_cbfn
br_device		STRUCT 4t
node		br_node		<>
identifier		@t_118		?
enquire		@t_119		?
load		@t_120		?
unload		@t_121		?
create		@t_122		?
br_device		ENDS

; 
;  * Given a pixelmap, returns the context to use
;  *
;  * XXX Handles NULL context pointer, but that should be sorted out in
;  * PixelmapAllocate
;  
; End of file ..\fw\device.h
; 
;  * Per vertex components
;  
C_X		EQU		0t
C_Y		EQU		1t
C_Z		EQU		2t
C_W		EQU		3t
C_U		EQU		4t
C_V		EQU		5t
C_I		EQU		6t
C_R		EQU		7t
C_G		EQU		8t
C_B		EQU		9t
C_Q		EQU		10t
NUM_COMPONENTS		EQU		11t

; 
;  * Mask bits for selecting which components take part in various operations
;  
CM_X		EQU		1t
CM_Y		EQU		2t
CM_Z		EQU		4t
CM_W		EQU		8t
CM_U		EQU		16t
CM_V		EQU		32t
CM_I		EQU		64t
CM_R		EQU		128t
CM_G		EQU		256t
CM_B		EQU		512t
CM_Q		EQU		1024t

; 
;  * Limited set of components used for lighting sub functions
;  
L_I		EQU		0t
L_R		EQU		1t
L_G		EQU		2t
L_B		EQU		3t
NUM_L_COMPONENTS		EQU		4t

; 
;  * Private information about a light as it applies to the current model
;  
; 
; 	 * Light intensity
; 	 
; 
; 	 * position and direction of light in view space
; 	 
; 
; 	 * Position and direction of light in final space, view or model
; 	 
; 
; 	 * Half vector for specular effects
; 	 
; 
; 	 * Cosine of spot angles
; 	 
; 
; 	 * Function to evaluate this light
; 	 
@proto_54		TYPEDEF		PROTO C :PTR br_vector3, :PTR br_fvector3, :PTR br_active_light, :PTR br_scalar
@t_123		TYPEDEF		PTR @proto_54
; 
; 	 * Copy of light's type - may vary from original if a model's relationship
; 	 * to light allows optimisation, eg:
; 	 *  very far point light -> directional light
; 	 *  model compeletely within spot cone -> point light
; 	 
@t_124		TYPEDEF		PTR br_light
br_active_light		STRUCT 4t
intensity		br_scalar		?
view_position		br_vector3		<>
view_direction		br_vector3		<>
position		br_vector3		<>
direction		br_vector3		<>
half		br_vector3		<>
spot_cosine_outer		br_scalar		?
spot_cosine_inner		br_scalar		?
light_sub_function		@t_123		?
_type		SDWORD		?
light		@t_124		?
br_active_light		ENDS

; 
;  * Private information about an active clip plane
;  
; 
; 	 * Planer eqn. in screen space
; 	 
br_active_clip_plane		STRUCT 4t
screen_plane		br_vector4		<>
br_active_clip_plane		ENDS

; 
;  * Framework has a table of functions used for lighting - this
;  * will be initialised according to what sort of output is required
;  * (eg: indexed or RGB)
;  
@proto_55		TYPEDEF		PROTO C :PTR br_vertex, :PTR br_fvector3, :PTR br_scalar
br_surface_fn		TYPEDEF		@proto_55

@proto_56		TYPEDEF		PROTO C :PTR br_vertex, :PTR br_face, :SDWORD
br_face_surface_fn		TYPEDEF		@proto_56

@proto_57		TYPEDEF		PROTO C :PTR br_vector3, :PTR br_fvector3, :PTR br_active_light, :PTR br_scalar
br_light_sub_fn		TYPEDEF		@proto_57

@proto_58		TYPEDEF		PROTO C :PTR br_model, :br_uint_16
br_model_update_cbfn		TYPEDEF		@proto_58

@proto_59		TYPEDEF		PROTO C :PTR br_material, :br_uint_16
br_material_update_cbfn		TYPEDEF		@proto_59

@proto_60		TYPEDEF		PROTO C :PTR br_pixelmap, :br_uint_16
br_table_update_cbfn		TYPEDEF		@proto_60

@proto_61		TYPEDEF		PROTO C :PTR br_pixelmap, :br_uint_16
br_map_update_cbfn		TYPEDEF		@proto_61

; 
; 	 * Top level surface functions
; 	 
@t_125		TYPEDEF		PTR br_surface_fn
;  Evauluate lighting	
@t_126		TYPEDEF		PTR br_surface_fn
;  Copy material 		
@t_127		TYPEDEF		PTR br_surface_fn
;  Copy vertex 			
@t_128		TYPEDEF		PTR br_surface_fn
;  simple light plus simple texture mapping 
@t_129		TYPEDEF		PTR br_face_surface_fn
;  Evauluate surface for a face	
; 
; 	 * Lighting
; 	 
@t_130		TYPEDEF		PTR br_light_sub_fn
@t_131		TYPEDEF		PTR br_light_sub_fn
@t_132		TYPEDEF		PTR br_light_sub_fn
@t_133		TYPEDEF		PTR br_light_sub_fn
@t_134		TYPEDEF		PTR br_light_sub_fn
fw_fn_table		STRUCT 4t
light		@t_125		?
light_material		@t_126		?
light_vertex		@t_127		?
light_texture		@t_128		?
face_light		@t_129		?
direct		@t_130		?
point		@t_131		?
point_attn		@t_132		?
spot		@t_133		?
spot_attn		@t_134		?
fw_fn_table		ENDS

; 
;  * Private state of framework
;  
; 
; 	 * Main surface properties function
; 	 
@t_135		TYPEDEF		PTR br_surface_fn
; 
; 	 * Called after U,V generation
; 	 
@t_136		TYPEDEF		PTR br_surface_fn
; 
; 	 * Called after component copying
; 	 
@t_137		TYPEDEF		PTR br_surface_fn
; 
; 	 * Lighting a face - usually calls vertex function and converts result for faces
; 	 
@t_138		TYPEDEF		PTR br_face_surface_fn
; 
; 	 * Current mapping coordinate transform
; 	 
; 
; 	 * Base and scale for indexes
; 	 
; 
; 	 * Current Transforms
; 	 
; 
; 	 * List of transforms from camera to root with associated actor address
; 	 
@t_139		TYPEDEF		PTR br_actor
@tag_28		STRUCT 4t
m		br_matrix34		<>
_a		@t_139		?
@tag_28		ENDS

; 
; 	 * Type of view to screen transform
; 	 
; 
; 	 * COP in model space
; 	 
; 
; 	 * Material curently in use - used for lighting calcs.
; 	 
@t_140		TYPEDEF		PTR br_material
; 
; 	 * Information about each active light, the lights that need to be
; 	 * processed in model space are first, followed by those processed
; 	 * in view space
; 	 
; 
; 	 * numbers of active lights of each type
; 	 
; 
; 	 * Flag that is set if there is only 1 direct light in model space
; 	 
; 
; 	 * Eye vector to use for lighting, either in model or view space
; 	 
; 
; 	 * A table of active clip planes
; 	 
; 
; 	 * A table of pointers to enabled lights
; 	 *
; 	 * If an entry is not NULL then the referenced actor is an enabled light
; 	 
@t_141		TYPEDEF		PTR br_actor
; 
; 	 * A table of pointers to enabled clip planes
; 	 *
; 	 * If an entry is not NULL then the referenced actor is a clip plane
; 	 
@t_142		TYPEDEF		PTR br_actor
; 
; 	 * A pointer to the current environment - if NULL, then the model's
; 	 * local frame will be used
; 	 
@t_143		TYPEDEF		PTR br_actor
; 
; 	 * Output pixelmap
; 	 
@t_144		TYPEDEF		PTR br_pixelmap
; 
; 	 * Viewport parameters
; 	 
; 
; 	 * Flag that is true if rendering is going on
; 	 
; 
; 	 * Various lists of registered items
; 	 
; 
; 	 * An index of registered resources by class
; 	 
@t_145		TYPEDEF		PTR br_resource_class
; 
; 	 * Renderer functions for item preperation & update 
; 	 
@t_146		TYPEDEF		PTR br_model_update_cbfn
@t_147		TYPEDEF		PTR br_material_update_cbfn
@t_148		TYPEDEF		PTR br_table_update_cbfn
@t_149		TYPEDEF		PTR br_map_update_cbfn
; 
; 	 * Current filesystem, memory, and error handlers
; 	 
@t_150		TYPEDEF		PTR br_filesystem
@t_151		TYPEDEF		PTR br_allocator
@t_152		TYPEDEF		PTR br_diaghandler
; 
; 	 * File write mode
; 	 
; 
; 	 * Base resource of which everything else is
; 	 * a descendant
; 	 
@t_153		TYPEDEF		PTR 
; 
; 	 * Default model
; 	 
@t_154		TYPEDEF		PTR br_model
; 
; 	 * Default material
; 	 
@t_155		TYPEDEF		PTR br_material
; 
; 	 * Function pointers to current lighting ops.
; 	 
; 
; 	 * Global scratch space
; 	 
@t_156		TYPEDEF		PTR 
br_framework_state		STRUCT 4t
surface_fn		@t_135		?
surface_fn_after_map		@t_136		?
surface_fn_after_copy		@t_137		?
face_surface_fn		@t_138		?
map_transform		br_matrix23		<>
index_base		br_scalar		?
index_range		br_scalar		?
model_to_screen		br_matrix4		<>
view_to_screen		br_matrix4		<>
model_to_view		br_matrix34		<>
view_to_model		br_matrix34		<>
model_to_environment		br_matrix34		<>
camera_path		@tag_28		16t DUP (<>)
vtos_type		SDWORD		?
eye_m		br_vector3		<>
eye_m_normalised		br_vector3		<>
material		@t_140		?
active_lights_model		br_active_light		16t DUP (<>)
active_lights_view		br_active_light		16t DUP (<>)
nactive_lights_model		br_uint_16		?
nactive_lights_view		br_uint_16		?
light_is_1md		SDWORD		?
eye_l		br_vector3		<>
active_clip_planes		br_active_clip_plane		6t DUP (<>)
nactive_clip_planes		br_uint_16		?
enabled_lights		@t_141		16t DUP (?)
enabled_clip_planes		@t_142		6t DUP (?)
enabled_environment		@t_143		?
output		@t_144		?
vp_width		br_scalar		?
vp_height		br_scalar		?
vp_ox		br_scalar		?
vp_oy		br_scalar		?
rendering		SDWORD		?
reg_models		br_registry		<>
reg_materials		br_registry		<>
reg_textures		br_registry		<>
reg_tables		br_registry		<>
reg_resource_classes		br_registry		<>
resource_class_index		@t_145		256t DUP (?)
model_update		@t_146		?
material_update		@t_147		?
table_update		@t_148		?
map_update		@t_149		?
fsys		@t_150		?
mem		@t_151		?
diag		@t_152		?
open_mode		SDWORD		?
res		@t_153		?
default_model		@t_154		?
default_material		@t_155		?
fn_table		fw_fn_table		<>
scratch_ptr		@t_156		?
scratch_size		br_size_t		?
scratch_last		br_size_t		?
scratch_inuse		SDWORD		?
br_framework_state		ENDS

; 
;  * Global renderer state
;  
EXTERNDEF		C fw:br_framework_state

; 
;  * Minimum scratch space to allocate for render temps.
;  
MIN_WORKSPACE		EQU		8192t
; 
;  * Make sure NULL is defined
;  
NULL		EQU		0t
; 
;  * Class of view->screen transform
;  
BR_VTOS_PARALLEL		EQU		0t
BR_VTOS_PERSPECTIVE		EQU		1t
BR_VTOS_4X4		EQU		2t

; 
;  * Magic value for vertex->r to say that this vertex does not reuse a previous x,y,z
;  
NO_REUSE		EQU		00000ffffh
; 
;  * The level below which the fixed specular power function is zero
;  
; 
;  * Material flags generated during MaterialUpdate()
;  
MATUF_SURFACE_FACES		EQU		000000001h
MATUF_SURFACE_VERTICES		EQU		000000002h
MATUF_REGISTERED		EQU		000000004h
; 
;  * Model flags generated during ModelUpdate()
;  
;  Smoothing group is the same for the whole model 
MODUF_SIMPLE_GROUPS		EQU		000000001h
;  There are the same number of vertex groups, using the same materials 
MODUF_VERTEX_GROUPS_MATCH		EQU		000000002h
MODUF_REGISTERED		EQU		000000004h
;  Model has non-zero pivot point 
MODUF_HAS_PIVOT		EQU		000000008h
; 
;  * Perspective dive for Z component of vertices
;  
; 
;  * Generates outcodes for a homogenous point
;  
; 																				 * The 6 planes of the view volume...											 
; 																				 * Any user defined clip planes...												 
BR_RES_TRACKING		EQU		1t
BR_RES_TAGGING		EQU		1t
; 
;  * Pull in private prototypes
;  
; End of file ..\fw\fw.h
USER_CLIP		EQU		1t
; 
;  * Type for the on-screen vertex components
;  
SCREEN_FIXED		EQU		1t
screen_scalar		TYPEDEF		br_fixed_ls
; 
;  * Per vertex (in model) structure
;  *
;  * Some code relies on this structure being 64 bytes long
;  
;  Projected screen space point		
;  Outcodes for this vertex			
;  Shading components for vertex	
temp_vertex		STRUCT 4t
v		screen_scalar		3t DUP (?)
outcode		br_uint_32		?
direction		br_uint_32		?
comp		br_scalar		11t DUP (?)
temp_vertex		ENDS

;  Projected screen space point		
;  Outcodes for this vertex			
;  Shading components for vertex	
temp_vertex_fixed		STRUCT 4t
v		screen_scalar		3t DUP (?)
outcode		br_uint_32		?
direction		br_uint_32		?
comp		br_fixed_ls		11t DUP (?)
temp_vertex_fixed		ENDS

; 
;  * Per edge (in model) structure
;  
; 
; 	 * Vertices at either end of edge
; 	 
; 	br_uint_32 surface;
; 
; 	 * Edge material
; 	 
@t_157		TYPEDEF		PTR br_material
temp_edge		STRUCT 4t
vertices		br_uint_16		2t DUP (?)
codes		br_uint_16		?
flag		br_uint_8		?
_pad		br_uint_8		1t DUP (?)
material		@t_157		?
temp_edge		ENDS

; 
;  * Per face (in model) structure
;  
; 
; 	 * Result of face surface call
; 	 *
; 	 * Will either be an index or a true colour
; 	 
; 
; 	 * Combined outcodes of face
; 	 
; 
; 	 * Flag describing visiblity of face
; 	 
temp_face		STRUCT 4t
surface		br_uint_32		?
codes		br_uint_16		?
flag		br_uint_8		?
_pad		br_uint_8		1t DUP (?)
temp_face		ENDS

; 
;  * Values for temp_face.flag
;  
TFF_VISIBLE		EQU		4t
TFF_CLIPPED		EQU		2t
TFF_REVERSED		EQU		1t
; 
;  * Values for temp_vertex.direction
;  
TVDIR_FRONT		EQU		000000001h
TVDIR_BACK		EQU		000000002h
; 
; 	 * Components of vertex
; 	 
clip_vertex		STRUCT 4t
outcode		br_uint_16		?
base_vertex		br_uint_16		?
comp		br_scalar		11t DUP (?)
clip_vertex		ENDS

; 
;  * Scan convertion details for one edge of triangle
;  
;  delta x along edge				
;  delta y along edge				
;  gradient (x/y)					
;  Starting value		(fraction)  
; 						(integer)   
;  delta_x per scanline	(fraction)  
;              			(integer)   
;  starting scanline				
;  total scanlines				    
scan_edge		STRUCT 4t
x		br_fixed_ls		?
y		br_fixed_ls		?
grad		br_fixed_ls		?
f		br_int_32		?
i		br_int_32		?
d_f		br_int_32		?
d_i		br_int_32		?
start		br_int_32		?
count		br_int_32		?
scan_edge		ENDS

; 
;  * Scan convertion details for one parameter
;  
;  Parameter (16.16) value at pixel				
;  Parameter (16.16) value at start of scanline	
;  Increment per scanline if carry from bit 15	
;    ""              ""      no carry   ""    	
;  Gradient of parameter along X axis			
;  Gradient of parameter along Y axis			
scan_parameter		STRUCT 4t
currentpix		br_fixed_ls		?
current		br_fixed_ls		?
d_carry		br_fixed_ls		?
d_nocarry		br_fixed_ls		?
grad_x		br_fixed_ls		?
grad_y		br_fixed_ls		?
scan_parameter		ENDS

; 
;  * Arbitrary width scan line data
;  
@t_158		TYPEDEF		PTR scan_edge
@t_159		TYPEDEF		PTR SBYTE
@t_160		TYPEDEF		PTR SBYTE
@t_161		TYPEDEF		PTR SBYTE
@t_162		TYPEDEF		PTR SBYTE
@t_163		TYPEDEF		PTR SBYTE
arbitrary_width_scan		STRUCT 4t
edge		@t_158		?
start		@t_159		?
_end		@t_160		?
zstart		@t_161		?
source_current		@t_162		?
u_int_current		SWORD		?
pad		SWORD		?
u_current		DWORD		?
du		DWORD		?
du_carry		DWORD		?
du_nocarry		DWORD		?
du_int		SDWORD		?
du_int_nocarry		SDWORD		?
du_int_carry		SDWORD		?
v_current		DWORD		?
dv		DWORD		?
dv_carry		DWORD		?
dv_nocarry		DWORD		?
dsource		SDWORD		?
dsource_carry		SDWORD		?
dsource_nocarry		SDWORD		?
texture_start		@t_163		?
texture_size		SDWORD		?
texture_stride		SDWORD		?
texture_width		SDWORD		?
arbitrary_width_scan		ENDS

; 
;  * Perspective texture mapper data
;  
@t_164		TYPEDEF		PTR SBYTE
@t_165		TYPEDEF		PTR SBYTE
@t_166		TYPEDEF		PTR SBYTE
perspective_scan		STRUCT 4t
start		@t_164		?
_end		@t_165		?
zstart		@t_166		?
source		SDWORD		?
y		SDWORD		?
perspective_scan		ENDS

; 
;  * Function types for primitives
;  
@proto_62		TYPEDEF		PROTO C :PTR temp_vertex_fixed, :PTR temp_vertex_fixed, :PTR temp_vertex_fixed
br_triangle_fn		TYPEDEF		@proto_62

@proto_63		TYPEDEF		PROTO C :PTR temp_vertex_fixed, :PTR temp_vertex_fixed
br_line_fn		TYPEDEF		@proto_63

@proto_64		TYPEDEF		PROTO C :PTR temp_vertex_fixed
br_point_fn		TYPEDEF		@proto_64

; 
;  * Callback functions for generic trapezoid and pixel plot fns.
;  
@proto_65		TYPEDEF		PROTO C :PTR scan_edge
br_trapezoid_render_cb		TYPEDEF		@proto_65

@proto_66		TYPEDEF		PROTO C :br_int_32, :br_int_32
br_pixel_render_cb		TYPEDEF		@proto_66

; 
;  * Private state of renderer
;  
; 
; 	 * Local copy of model -> screen with viewport factored in
; 	 
; 
; 	 * Pointers into scratch area
; 	 
@t_167		TYPEDEF		PTR br_int_8
@t_168		TYPEDEF		PTR br_int_32
@t_169		TYPEDEF		PTR br_int_8
@t_170		TYPEDEF		PTR br_int_8
@t_171		TYPEDEF		PTR temp_face
@t_172		TYPEDEF		PTR temp_edge
@t_173		TYPEDEF		PTR temp_vertex
; 
; 	 * Flag to indicate that vertex directions
; 	 * have been initialised
; 	 
; 
; 	 * Model and material being rendered
; 	 
@t_174		TYPEDEF		PTR br_model
@t_175		TYPEDEF		PTR br_material
; 
; 	 * Current default material
; 	 
@t_176		TYPEDEF		PTR br_material
; 
; 	 * Misc. constant parameters for rendering
; 	 
@t_177		TYPEDEF		PTR br_uint_8
;  Colour buffer		
@t_178		TYPEDEF		PTR br_fixed_ls
;  Z buffer				
;  Stride (in bytes)	
;  Stride (in bytes)	
@t_179		TYPEDEF		PTR br_uint_8
;  Texture map to use				
@t_180		TYPEDEF		PTR br_uint_8
;  Bump map to use					
@t_181		TYPEDEF		PTR br_uint_8
;  Indirection table				
@t_182		TYPEDEF		PTR br_uint_8
;  Indirection table				
@t_183		TYPEDEF		PTR br_uint_8
;  Lighting for quantised normals	
@t_184		TYPEDEF		PTR br_uint_32
;  Screen							
; 
; 	 * Workspace for triangle scan converter
; 	 
;  Long edge of triangle				   
;  Top short edge						   
;  Bottom short edge					   
;  Depth				
;  Mapping				
;  Index				
;  Red					
;  Green				
;  Blue					
;  Perspective divisor	
;  Current texel 
; 
; 	 * Per-component mask for scan converter interpolation
; 	 
;  Components to interpolate 
;  Components to apply perspective correction 
;  Components to clip 
;  Components to convert scalar->float 
; 
; 	 * Global arbitrary width scan line data
; 	 
; 
; 	 * Perspective texture mapper globals
; 	 
; 
; 	 * Current triangle functions
; 	 
@t_185		TYPEDEF		PTR br_triangle_fn
; 
; 	 * Generic triangle rendering sub-functions
; 	 
@t_186		TYPEDEF		PTR br_trapezoid_render_cb
@t_187		TYPEDEF		PTR br_pixel_render_cb
; 
; 	 * Bounds of rendered actor
; 	 
; 
; 	 * Callback for visible actors
; 	 
@t_188		TYPEDEF		PTR br_renderbounds_cbfn
; 
; 	 * Current output type
; 	 
@t_189		TYPEDEF		PTR zb_render_type
; 
; 	 * Anchor resource for ZB renderer
; 	 
@t_190		TYPEDEF		PTR 
br_zbuffer_state		STRUCT 4t
os_model_to_screen		br_matrix4		<>
os_model_to_screen_hi		br_scalar		4t DUP (?)
vertex_counts		@t_167		?
face_group_counts		@t_168		?
face_group_clipped		@t_169		?
edge_counts		@t_170		?
temp_faces		@t_171		?
temp_edges		@t_172		?
temp_vertices		@t_173		?
directions_cleared		SDWORD		?
model		@t_174		?
material		@t_175		?
default_material		@t_176		?
colour_buffer		@t_177		?
depth_buffer		@t_178		?
row_width		br_int_32		?
depth_row_width		br_int_32		?
texture_buffer		@t_179		?
bump_buffer		@t_180		?
shade_table		@t_181		?
blend_table		@t_182		?
lighting_table		@t_183		?
screen_table		@t_184		?
main		scan_edge		<>
top		scan_edge		<>
bot		scan_edge		<>
pz		scan_parameter		<>
pu		scan_parameter		<>
pv		scan_parameter		<>
pi		scan_parameter		<>
pr		scan_parameter		<>
pg		scan_parameter		<>
pb		scan_parameter		<>
pq		scan_parameter		<>
source		scan_parameter		<>
component_mask		br_uint_32		?
correct_mask		br_uint_32		?
clip_mask		br_uint_32		?
convert_mask		br_uint_32		?
awsl		arbitrary_width_scan		<>
tsl		perspective_scan		<>
triangle_render		@t_185		?
trapezoid_render		@t_186		?
pixel_render		@t_187		?
bounds		screen_scalar		4t DUP (?)
bounds_call		@t_188		?
_type		@t_189		?
res		@t_190		?
br_zbuffer_state		ENDS

EXTERNDEF		C zb:br_zbuffer_state

; 
; 			 * Transform into screen space - Inline expanded BrMatrix4ApplyP()
; 			 
; 
;  * Generic macro for projecting a vertex from homogenous coordinates
;  
; 
;  * Alignment for block in scratch area
;  
SCRATCH_BOUNDARY		EQU		16t
; 
;  * New line and point clamps
;  
; 
;  * Include bounding rectangle callback
;  
BOUNDING_RECTANGLE_CALL		EQU		1t
; 
;  * Update MIN and MAX vertices' x,y
;  
@proto_67		TYPEDEF		PROTO C :PTR br_face_group, :PTR temp_face
br_face_group_fn		TYPEDEF		@proto_67

; 
;  * Structure describing one particular sort of material renderer
;  
; 
; 	 * Descriptive string
; 	 
@t_191		TYPEDEF		PTR SBYTE
; 
; 	 * If 
; 	 *		mat->flags & flags_mask) == flags_cmp
; 	 * 	    colour_map->map_type == map_type
; 	 *		width != 0 && colour_map->width == width
; 	 *		height != 0 && colour_map->height == height
; 	 *
; 	 *  material matches this type
; 	 
@t_192		TYPEDEF		PTR br_face_group_fn
@t_193		TYPEDEF		PTR br_triangle_fn
@t_194		TYPEDEF		PTR br_line_fn
@t_195		TYPEDEF		PTR br_point_fn
zb_material_type		STRUCT 4t
identifier		@t_191		?
flags_mask		br_uint_32		?
flags_cmp		br_uint_32		?
map_type		br_uint_8		?
_width		br_uint_16		?
height		br_uint_16		?
face_group		@t_192		?
triangle		@t_193		?
line		@t_194		?
point		@t_195		?
clip_mask		br_uint_32		?
convert_mask		br_uint_32		?
zb_material_type		ENDS

; 
;  * Flags used to augment material flags when specifiying
;  * material type
;  
ZB_MATF_HAS_MAP		EQU		080000000h
ZB_MATF_TRANSPARENT		EQU		040000000h
ZB_MATF_HAS_SCREEN		EQU		020000000h
ZB_MATF_SQUARE_POW2		EQU		010000000h
ZB_MATF_NO_SKIP		EQU		008000000h
ZB_MATF_MAP_OPACITY		EQU		004000000h
ZB_MATF_MAP_TRANSPARENT		EQU		002000000h
ZB_MATF_HAS_SHADE		EQU		001000000h
; 
;  * Structure describing one particular class of renderer
;  
; 
; 	 * Descriptive string
; 	 
@t_196		TYPEDEF		PTR SBYTE
; 
; 	 * Type of colour buffer
; 	 
; 
; 	 * Type of depth buffer
; 	 
;  XXX unused at the moment 
; 
; 	 * Used to check if the row_width's of the two pixelmaps
; 	 * match -
; 	 *	if colour row_width * depth_row_size == depth row_width * colour_row_size
; 	 *	then this set of functions match
; 	 
; 
; 	 * Flag to indicate true-colour
; 	 
; 
; 	 * A table of supported material types
; 	 
@t_197		TYPEDEF		PTR zb_material_type
zb_render_type		STRUCT 4t
identifier		@t_196		?
colour_type		br_uint_8		?
depth_type		br_uint_8		?
user_type		br_uint_16		?
colour_row_size		br_uint_8		?
depth_row_size		br_uint_8		?
true_colour		br_uint_8		?
material_types		@t_197		?
nmaterial_types		SDWORD		?
zb_render_type		ENDS

; 
;  * Pull in private prototypes
;  
; 
; #ifndef _ZBIPXTRA_H_
; #include "zbipxtra.h"
; #endif
; 
; End of file zb.h
