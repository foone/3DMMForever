;; Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
;;
;; $Id: 586_macs.inc 1.3 1995/02/22 21:53:24 sam Exp $
;; $Locker:  $
;;
;; Various macros to play with optimisation on pentium
;;
	; Swap two registers - xchg is not a 'simple' insn
	;
SWAP		macro	ra,rb
	if 0
		xchg	ra,rb
	else
;		xor	ra,rb
;		xor	rb,ra
;		xor	ra,rb
		push	ra
		push	rb
		pop		ra
		pop		rb
	endif
		endm

	; Fix result of a fixed point multiply in edx:eax => eax
	;
FIX_MUL		macro
	if 0
		shrd	eax,edx,16
	else
		shr	eax,16
		shl	edx,16
		or	eax,edx
	endif
		endm

	; Fraction
	;
FIX_FMUL	macro
	if 0
		shrd	eax,edx,15
	else
		shr	eax,15
		shl	edx,17
		or	eax,edx
	endif
		endm

	; Fix input to fixed point divide in edx => edx:eax
	;
FIX_DIV		macro

		mov	eax,edx
		sar	edx,16
		shl	eax,16

		endm


