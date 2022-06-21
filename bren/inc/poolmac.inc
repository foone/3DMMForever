;; Copyright (c) 1992,1993 Argonaut Software Ltd. All rights reserved.
;;
;; $Id: poolmac.inc 1.2 1994/07/01 18:55:27 sam Exp $
;; $Locker:  $
;;
;; ASM support for pool allcoation
;;

; Allocate a block form the given pool, leaving the pointer in a given
; register.
;
; Trashes AX
;
BR_POOL_ALLOCATE macro reg,pool
	local alloc_ok

		mov	edi,pool.free
		and	edi,edi
		jne	alloc_ok

		push	ecx
		push	ebx
		push	edx
		push	offset pool
		call	BrPoolAddChunk
		add	sp,4
		pop	edx
		pop	ebx
		pop	ecx

		mov	edi,pool.free

alloc_ok:	mov	eax,[edi].br_pool_block.next
		mov	pool.free,eax

		endm

