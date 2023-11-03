;; Copyright (c) 1991,1993-1995 Argonaut Technologies Limited. All rights reserved.
;;
;; $Id: t_piza.asm 1.1 1995/08/31 16:52:47 sam Exp $
;; $Locker:  $
;;
;; Trapezoid rendering for arbitary width texture mapping
;; Transparent Version.
;; See t_piza2.asm for non-transparent version
;;

	.386p
	.model	c,flat
	include zb.inc

WRAP	equ	1
WRAPROW	equ	1
TRANSP	equ	1

	.data
noffset	dd	?

	.code

	; Macro to increment those parts of the per scan line
	; increment that depend on whether there has been a carry
	; from the fractional part of the current screen x coordinate

scan_inc	macro	carry,LIGHT,BPP
	local	lab2

	if	LIGHT
	mov	ebx,zb.pi.current
	endif	; LIGHT

	if	LIGHT
	mov	ecx,zb.pi.d_&carry&
	endif	; LIGHT

	mov	esi,zb.pz.current
	mov	edi,zb.pz.d_&carry&
	add	esi,edi
	mov	zb.pz.current,esi

	add	ebx,ecx
	mov	zb.pi.current,ebx

	mov	ebx,zb.awsl.u_current
	mov	ecx,zb.awsl.du_&carry&
	mov	ebp,zb.awsl.source_current
	mov	edi,zb.awsl.dsource_&carry&
	add	ebx,ecx			; zb.awsl.u_current += zb.awsl.dsource_&carry&
	sbb	ecx,ecx			; Save -carry
	add	ebp,edi			; zb.awsl.source_current += zb.awsl.dsource_&carry&
	mov	zb.awsl.u_current,ebx
	
	if WRAP
	mov	ebx,dword ptr zb.awsl.u_int_current
	add	ebx,zb.awsl.du_int_&carry&
	sub	ebx,ecx			; zb.awsl.u_int_current += carry;
	mov	dword ptr zb.awsl.u_int_current,ebx
	endif	; WRAP

	ifidni	<BPP>,<2>
	shl	ecx,1
	elseifidni	<BPP>,<3>
	lea	ecx,[ecx+ecx*2]
	elseifidni	<BPP>,<4>
	shl	ecx,2
	endif	; <BPP>

	sub	ebp,ecx			; zb.awsl.source_current += carry*BPP

	mov	ebx,zb.awsl.v_current
	mov	ecx,zb.awsl.dv_&carry&
	add	ebx,ecx
	mov	zb.awsl.v_current,ebx
	jnc	lab2

	mov	edi,zb.awsl.texture_stride
	add	ebp,edi
lab2:

	mov	zb.awsl.source_current,ebp
	endm



TriangleRenderZ2	macro	fn,LIGHT,BPP,BUMP

fn	proc uses eax ebx ecx edx esi edi ebp

outer:
	mov	eax,zb.awsl.edge
	mov	ebx,[eax].scan_edge.count
	test	ebx,ebx
	jz	exit
	dec	ebx ; zb.awsl.edge->count--
	mov	[eax].scan_edge.count,ebx

	if	BUMP
	mov	ecx,zb.bump_buffer
	sub	ecx,zb.texture_buffer
	mov	noffset,ecx
	endif

	mov	edi,zb.awsl.start
	mov	ebx,zb.awsl._end
	cmp	edi,ebx
	mov	eax,zb.awsl.u_current
	mov	ebx,zb.awsl.v_current

	mov	al,byte ptr zb.awsl.u_int_current

	mov	ecx,zb.pz.current
	mov	zb.pz.currentpix,ecx

	if	LIGHT
	mov	ecx,zb.pi.current
	mov	zb.pi.currentpix,ecx
	endif	; LIGHT

	mov	ah,byte ptr zb.awsl.u_int_current+1

	mov	esi,zb.awsl.source_current
	mov	ebp,zb.awsl.zstart

	jge	scan_back

	; Forward scan
loopf:
	mov	ecx,zb.awsl._end
	cmp	edi,ecx
	jge	next

	if	TRANSP

	ifidni	<BPP>,<1>
	mov		dl, [esi]
	test	dl, dl
	jz		failf ; Skip if transparent
	elseifidni	<BPP>,<2>
	mov		dl, [esi]
	mov		dh, 1[esi]
	or		dl, dh
	jz	failf
	elseifidni	<BPP>,<3>
	mov	dl,[esi]
	mov	dh,1[esi]
	or	dl,dh
	mov	dh,2[esi]
	or	dl,dh
	jz	failf
	endif	; <BPP>

	endif	; TRANSP

	; 16 bit z test 

	mov	bl,byte ptr zb.pz.currentpix+2
	mov	dl,[ebp]
	mov	bh,byte ptr zb.pz.currentpix+3
	mov	dh,1[ebp]
	cmp	dl,bl
	sbb	dh,bh
	jb	failf			; Skip if pixel failed z test

	if	LIGHT

	xor	ecx,ecx
	mov	[ebp],bl		; Store low byte of z

	mov	ch,byte ptr zb.pi.currentpix+2	; Get lighting index
	mov	edx,zb.shade_table
	mov	cl,[esi]		; Get texel
	mov	1[ebp],bh		; Store high byte of z
	add	edx,ecx
	mov	ecx,zb.pz.currentpix
	add	ebp,2			; zptr += 2
	mov	bl,[edx]		; Light texel
	mov	edx,zb.pz.grad_x	; z += dz
	mov	[edi],bl		; Store texel

	add	ecx,edx

	ifidni	<BPP>,<1>
	inc	edi			; ptr++
	else	; <BPP>
	add	edi,BPP
	endif	; <BPP>

	mov	edx,zb.pi.currentpix
	mov	zb.pz.currentpix,ecx
	mov	ecx,zb.pi.grad_x	; i += di
	add	edx,ecx
	mov	zb.pi.currentpix,edx

	jmp	contf

failf:
	add	ebp,2			; zptr += 2

	ifidni	<BPP>,<1>
	inc	edi			; ptr++
	else	; <BPP>
	add	edi,BPP
	endif	; <BPP>

	mov	ecx,zb.pz.currentpix
	mov	edx,zb.pz.grad_x	; z += dz
	add	ecx,edx
	mov	edx,zb.pi.currentpix
	mov	zb.pz.currentpix,ecx
	mov	ecx,zb.pi.grad_x	; i += di
	add	edx,ecx
	mov	zb.pi.currentpix,edx

	elseif	BUMP	

	; Bump mapping

	mov	[ebp],bl		; Store low byte of z

	xor	ecx,ecx
	mov	cl,[esi]		; Get texel
	mov	edx,noffset
	mov	dl,[esi+edx]		; Get normal
	and	edx,0ffh
	add	edx,zb.lighting_table	; Calculate light level from normal
	mov	ch,[edx]
	mov	edx,zb.shade_table
	mov	cl,[edx+ecx]		; Light texel
	mov	1[ebp],bh		; Store high byte of z
	mov	[edi],cl

	mov	ecx,zb.pz.currentpix
	add	ebp,2			; zptr += 2
	mov	edx,zb.pz.grad_x	

	add	ecx,edx			; z += dz

	ifidni	<BPP>,<1>
	inc	edi			; ptr++
	else	; <BPP>
	add	edi,BPP
	endif	; <BPP>

	mov	zb.pz.currentpix,ecx
	jmp	contf

failf:
	add	ebp,2			; zptr += 2

	ifidni	<BPP>,<1>
	inc	edi			; ptr++
	else	; <BPP>
	add	edi,BPP
	endif	; <BPP>

	mov	ecx,zb.pz.currentpix
	mov	edx,zb.pz.grad_x	; z += dz
	add	ecx,edx
	mov	zb.pz.currentpix,ecx

	else	; LIGHT

	; No lighting

	mov	[ebp],bl		; Store low byte of z

	ifidni	<BPP>,<1>

	mov	cl,[esi]		; Get texel
	mov	1[ebp],bh		; Store high byte of z
	mov	[edi],cl

	elseifidni	<BPP>,<2>

	mov	cl,[esi]
	mov	1[ebp],bh		; Store high byte of z
	mov	ch,1[esi]
	mov	[edi],cl
	mov	1[edi],ch

	elseifidni	<BPP>,<3>

	mov	cl,[esi]		; Get texel
	mov	1[ebp],bh		; Store high byte of z
	mov	[edi],cl
	mov	ch,1[esi]
	mov	1[edi],ch		; Store green
	mov	cl,2[esi]
	mov	2[edi],cl		; Store red

	elseifidni	<BPP>,<4>

	mov	ecx,[esi]
	mov	1[ebp],bh
	mov	[edi],ecx

	endif	; <BPP>

	mov	ecx,zb.pz.currentpix
	add	ebp,2			; zptr += 2
	mov	edx,zb.pz.grad_x	

	add	ecx,edx			; z += dz

	ifidni	<BPP>,<1>
	inc	edi			; ptr++
	else	; <BPP>
	add	edi,BPP
	endif	; <BPP>

	mov	zb.pz.currentpix,ecx
	jmp	contf

failf:
	add	ebp,2			; zptr += 2

	ifidni	<BPP>,<1>
	inc	edi			; ptr++
	else	; <BPP>
	add	edi,BPP
	endif	; <BPP>

	mov	ecx,zb.pz.currentpix
	mov	edx,zb.pz.grad_x	; z += dz
	add	ecx,edx
	mov	zb.pz.currentpix,ecx

	endif	; LIGHT

contf:

	; Redo this with high and low halfwords of du swapped

	mov	ecx,zb.awsl.du
	add	eax,ecx			; u += du
	mov	ecx,zb.awsl.dsource

	sbb	edx,edx			; edx = -carry
	add	esi,ecx
	mov	ecx,zb.awsl.du_int
	sub	eax,edx			; zb.awsl.du += carry

	ifidni	<BPP>,<2>
	shl	edx,1
	elseifidni	<BPP>,<3>
	lea	edx,[edx+2*edx]
	elseifidni	<BPP>,<4>
	shl	edx,2
	endif	; <BPP>

	sub	esi,edx			; esi += carry
	add	eax,ecx			; u_int += du_int

	mov	ecx,zb.awsl.dv
	mov	edx,zb.awsl.texture_stride; 
	add	ebx,ecx
	jnc	nostridef

	add	esi,edx

nostridef:
	test	ah,ah
	jl	nowidthf

	mov	ecx,zb.awsl.texture_width
	add	eax,ecx

	ifidni	<BPP>,<1>
	add	esi,ecx
	elseifidni	<BPP>,<2>
	lea	esi,[esi+2*ecx]
	elseifidni	<BPP>,<3>
	lea	ecx,[ecx+2*ecx]
	add	esi,ecx
	elseifidni	<BPP>,<4>
	lea	esi,[esi+4*ecx]
	endif	; <BPP>

nowidthf:
	mov	ecx,zb.awsl.texture_start
	cmp	esi,ecx			; Have we underrun?
	jge	nosizef

	mov	ecx,zb.awsl.texture_size
	add	esi,ecx

nosizef:

	jmp	loopf

scan_back:
	; Backwards scan
loopb:
	mov	ecx, zb.awsl._end
	cmp	edi, ecx
	jle	next

	mov	ecx, zb.pz.currentpix
	mov	edx, zb.pz.grad_x	; z += dz
	sub	ecx, edx

	if	LIGHT
	mov	edx, zb.pi.currentpix
	endif

	mov	zb.pz.currentpix,ecx

	if	LIGHT
	mov	ecx,zb.pi.grad_x	; i += di
	sub	edx,ecx
	mov	zb.pi.currentpix,edx
	endif

	; Redo this with high and low halfwords of du swapped

	mov	ecx,zb.awsl.du
	add	eax,ecx			; u += du
	mov	ecx,zb.awsl.dsource

	sbb	edx,edx			; edx = -carry
	add	esi,ecx
	mov	ecx,zb.awsl.du_int
	sub	eax,edx
	add	eax,ecx			; u_int += du_int

	ifidni	<BPP>,<1>
	sub	esi,edx
	elseifidni	<BPP>,<2>
	shl	edx,1
	sub	esi,edx
	elseifidni	<BPP>,<3>
	lea	edx,[edx+edx*2]
	sub	esi,edx
	elseifidni	<BPP>,<4>
	shl	edx,2
	sub	esi,edx
	endif

	mov	ecx,zb.awsl.dv
	add	ebx,ecx
	jnc	nostrideb

	mov	ecx,zb.awsl.texture_stride
	add	esi,ecx

nostrideb:

	test	ah,ah
	jl	nowidthb

	mov	ecx,zb.awsl.texture_width
	add	eax,ecx

	ifidni	<BPP>,<1>
	add	esi,ecx
	elseifidni	<BPP>,<2>
	lea	esi,[esi+2*ecx]
	elseifidni	<BPP>,<3>
	lea	ecx,[ecx+2*ecx]
	add	esi,ecx
	elseifidni	<BPP>,<4>
	lea	esi,[esi+4*ecx]
	endif	; <BPP>

nowidthb:
	mov	ecx,zb.awsl.texture_start
	cmp	esi,ecx
	jge	nosizeb

	mov	ecx,zb.awsl.texture_size
	add	esi,ecx

nosizeb:
	sub	ebp,2			; zptr += 2

	ifidni	<BPP>,<1>
	dec	edi			; ptr += 1
	else	; <BPP>
	sub	edi,BPP
	endif	; <BPP>

	if TRANSP

	ifidni <BPP>,<1>
	mov	bl,[esi]
	test	bl,bl
	jz	loopb		; Skip if transparent
	elseifidni	<BPP>,<2>
	mov	bl,[esi]
	mov	bh,1[esi]
	or	bl,bh
	jz	failb
	elseifidni	<BPP>,<3>
	mov	bl,[esi]
	mov	bh,1[esi]
	or	bl,bh
	mov	bh,2[esi]
	or	bl,bh
	jz	failb
	endif	; <BPP>

	endif	; <TRANSP>

	mov	dl,byte ptr zb.pz.currentpix+2
	mov	bl,[ebp]
	mov	dh,byte ptr zb.pz.currentpix+3
	mov	bh,1[ebp]
	sub	bl,dl
	sbb	bh,dh
	jb	loopb			; Skip if pixel failed z test

	mov	[ebp],dl		; Store low byte of z

	if	LIGHT
	xor	ecx,ecx
	endif	; <LIGHT>

	mov	1[ebp],dh		; Store high byte of z

	if	LIGHT
	mov	ch,byte ptr zb.pi.currentpix+2
	mov	edx,zb.shade_table
	endif	; <LIGHT>

	ifidni	<BPP>,<1>

	mov	cl,[esi]		; Get texel

	if	LIGHT
	mov	cl,[ecx+edx]		; Light texel
	endif

	if	BUMP
	and	ecx,0ffh
	mov	edx,noffset
	mov	dl,[esi+edx]
	and	edx,0ffh
	add	edx,zb.lighting_table	; Calculate light level from normal
	mov	ch,[edx]
	mov	edx,zb.shade_table
	mov	cl,[edx+ecx]		; Light texel
	endif

	mov	[edi],cl

	elseifidni	<BPP>,<2>

	mov	cl,[esi]
	mov	[edi],cl		; Store texel
	mov	cl,1[esi]
	mov	1[edi],cl		; Store texel

	elseifidni	<BPP>,<3>

	mov	ecx,[esi]
	mov	2[edi],cl		; Store texel
	mov	1[edi],ch		; Store texel
	ror	ecx,16
	mov	[edi],cl		; Store texel

	elseifidni	<BPP>,<4>

	mov	ecx,[esi]
	mov	[edi],ecx		; Store texel

	endif	; <BPP>

failb:
	jmp	loopb

next:
	; Per scan line updates

	assume	eax:ptr scan_edge
	mov	eax,zb.awsl.edge

	mov	ebx,[eax].f
	mov	ecx,[eax].d_f
	add	ebx,ecx			; edge->f += edge->d_f
	mov	[eax].f,ebx
	mov	ecx,[eax].d_i
	mov	ebx,zb.awsl._end

	ifidni 	<BPP>,<1>
	adc	ebx,ecx			; zb.awsl._end += edge->d_i+carry
	elseifidni	<BPP>,<2>
	adc	ecx,0
	shl	ecx,1
	add	ebx,ecx
	elseifidni	<BPP>,<3>
	adc	ecx,0
	lea	ecx,[ecx+2*ecx]
	add	ebx,ecx			; zb.awsl._end += 3*(edge->d_i+carry)
	elseifidni	<BPP>,<3>
	adc	ecx,0
	shl	ecx,2
	add	ebx,ecx
	endif	; <BPP>

	mov	zb.awsl._end,ebx

	mov	ebx,zb.main.f
	mov	ecx,zb.main.d_f
	add	ebx,ecx
	mov	zb.main.f,ebx
	mov	ebx,zb.awsl.start
	mov	ecx,zb.main.d_i
	mov	esi,zb.awsl.zstart
	jnc	noscarry

	; if (carry) ...

	adc	ecx,0
	lea	esi,[esi+2*ecx]

	ifidni	<BPP>,<1>
	add	ebx,ecx
	elseifidni	<BPP>,<2>
	lea	ebx,[ebx+2*ecx]
	elseifidni	<BPP>,<3>
	lea	ecx,[ecx+2*ecx]
	add	ebx,ecx
	elseifidni	<BPP>,<4>
	lea	ebx,[ebx+4*ecx]
	endif

	mov	zb.awsl.start,ebx
	mov	zb.awsl.zstart,esi

	scan_inc	carry,LIGHT,BPP

	jmp	cont

	; ... else ...
noscarry:

	lea	esi,[esi+2*ecx]

	ifidni	<BPP>,<1>
	add	ebx,ecx
	elseifidni	<BPP>,<2>
	lea	ebx,[ebx+2*ecx]
	elseifidni	<BPP>,<3>
	lea	ecx,[ecx+2*ecx]
	add	ebx,ecx
	elseifidni	<BPP>,<4>
	lea	ebx,[ebx+4*ecx]
	endif

	mov	zb.awsl.start,ebx
	mov	zb.awsl.zstart,esi

	scan_inc	nocarry,LIGHT,BPP

	; ... endif

cont:
	mov	ebx,dword ptr zb.awsl.u_int_current
	test	bh,bh			; Test sign of low halfword
	jl	lab1

	mov	ecx,zb.awsl.texture_width
	add	ebx,ecx
	mov	dword ptr zb.awsl.u_int_current,ebx
	mov	ebx,zb.awsl.source_current

	ifidni	<BPP>,<1>
	add	ebx,ecx
	elseifidni	<BPP>,<2>
	lea	ebx,[ebx+2*ecx]
	elseifidni	<BPP>,<3>
	lea	ecx,[ecx+2*ecx]
	add	ebx,ecx
	elseifidni	<BPP>,<4>
	lea	ebx,[ebx+4*ecx]
	endif

	mov	zb.awsl.source_current,ebx

lab1:
	mov	ebx,zb.awsl.source_current
	mov	ecx,zb.awsl.texture_start
	cmp	ebx,ecx
	jge	outer

	mov	ecx,zb.awsl.texture_size
	add	ebx,ecx
	mov	zb.awsl.source_current,ebx

	jmp	outer

exit:
	ret

fn	endp

	endm

TriangleRenderZ2	TrapezoidRenderPIZ2TA,          0, 1, 0
TriangleRenderZ2	TrapezoidRenderPIZ2TA_RGB_555,  0, 2, 0
TriangleRenderZ2	TrapezoidRenderPIZ2TA_RGB_888,  0, 3, 0

TriangleRenderZ2	TrapezoidRenderPIZ2TAN,         0, 1, 1
TriangleRenderZ2	TrapezoidRenderPIZ2TAN_RGB_555, 0, 2, 1
TriangleRenderZ2	TrapezoidRenderPIZ2TAN_RGB_888, 0, 3, 1

TriangleRenderZ2	TrapezoidRenderPIZ2TIA,         1, 1, 0
TriangleRenderZ2	TrapezoidRenderPIZ2TIA_RGB_555, 1, 2, 0
TriangleRenderZ2	TrapezoidRenderPIZ2TIA_RGB_888, 1, 3, 0

	end