;; Copyright (c) 1992,1993-1995 Argonaut Technologies Limited. All rights reserved.
;;
;; $Id: ti8_pizp.asm 1.1 1995/08/31 16:52:50 sam Exp $
;; $Locker:  $
;;
;; Scanline loop for perpsective correct texture mapping
;;
		.386p
		.model	c,flat
		include	zb.inc
		.data

z		dd	?
i		dd	?
du_numerator	dd	?
dv_numerator	dd	?
u_numerator	dd	?
v_numerator	dd	?
zdest		dd	?
dest		dd	?
dest2		dd	?
source		dd	?
denominator	dd	?
dz		dd	?

		align 4

		.code


; Smoothly lit perspective texture mapping

ScanLinePIZ2TIP	macro	name,pre,incu,decu,incv,decv,post1,post2
name		proc

		pushad

	; Copy various parameters into registers

                mov     esi,zb.pu.current
                mov     ebx,zb.pv.current
                mov     eax,zb.pu.grad_x
                mov     edi,zb.pv.grad_x

		mov	dv_numerator,edi
                mov     ecx,zb.pi.current
                mov     edx,zb.pq.current
		mov	v_numerator,ebx
		mov	denominator,edx
                mov     i,ecx
		

                mov     ecx,zb.pz.current
                mov     z,ecx
                mov     ecx,zb.tsl.zstart
                mov     zdest,ecx

                mov     ebp,zb.tsl._end
                mov     du_numerator,eax
		
                mov     edi,zb.tsl.start
                mov     eax,zb.tsl.source
		
                cmp     edi,ebp
		mov	ebp,zdest
                ja      rtol		; Jump if right to left	
		
	    ; Left to right scan
		
                mov     dest,edi
next_pixelb:
		jae     L50		; Jump if end of scan
		
                mov     source,eax	; Free up eax

	    ; Texel fetch and store section

	    ; z buffer test

		mov	dl,[ebp]
		mov	ebx,z		; Get new z value
		mov	dh,1[ebp]
		ror	ebx,16
                cmp     bx,dx
                ja      L34		; Jump if new pixel behind old
		
                mov     edx,zb.texture_buffer
		xor	ecx,ecx

		;; Get texel and test for transparency

		mov	cl,[eax+edx]
if 1
		test	cl,cl
		jz	L34
endif

		;; Store texel and z

		mov	eax,zb.shade_table
		mov	ch,byte ptr i+2
                mov     [ebp],bx	; Store new z
                mov     cl,[ecx+eax]	; Get lit texel
                mov     [edi],cl	; Store texel

L34:
		ror	ebx,16
                mov     edx,zb.pz.grad_x	; Get z step
                add     ebp,2		; zdest++
                add     ebx,edx		; z += dz
		mov	eax,zb.pi.grad_x
	        mov	edx,i		; Get lighting
		add	edx,eax		; i += di
		mov	i,edx
		
L34a:
		inc	edi
if	1
		cmp	edi,zb.tsl._end
		jae	L50
endif
	        mov	edx,denominator
                mov     dest,edi
		mov	edi,dv_numerator
                mov     z,ebx
                mov     zdest,ebp
                mov     ecx,zb.pq.grad_x
                mov     ebp,du_numerator
		mov	ebx,v_numerator
                mov     eax,source	; Restore eax

	; Perspective calculation section

		pre
                sub     esi,ebp
                add     edx,ecx
                cmp     esi,edx
                jge     incuf		; Jump if u not too large
		
L36:
		test    esi,esi
		jl	decuf
L37:
		sub     ebx,edi
		jl	decvf
                cmp     ebx,edx
                jge     incvf		; Jump if v not too small
L40:
		post1
		post2
		mov	denominator,edx
		mov	v_numerator,ebx
		mov	edi,dest
                cmp     edi,zb.tsl._end
		mov     ebp,zdest
                jmp     next_pixelb

	    ; Right to left
rtol:
                mov     dest,edi
next_pixelbr:
		jbe     L50		; Jump if end of scan
		
                mov     source,eax	; Free up eax

	    ; z buffer test

		mov	dl,[ebp]
		mov	ebx,z		; Get new z value
		mov	dh,1[ebp]
		ror	ebx,16
                cmp     bx,dx
                ja      R34		; Jump if new pixel behind old
		
                mov     edx,zb.texture_buffer
		xor	ecx,ecx

		;; Get texel and test for transparency

		mov	cl,[edx+eax]
if 1
		test	cl,cl		; Transparent?
		jz	R34
endif

		;; Store texel and z

		mov	eax,zb.shade_table
		mov	ch,byte ptr i+2
                mov     [ebp],bx	; Store new z
                mov     cl,[ecx+eax]	; Get lit texel
                mov     [edi],cl	; Store texel
R34:
                sub     ebp,2		; zdest++

		;; Update lighting and z

		ror	ebx,16
                mov     edx,zb.pz.grad_x	; Get z step
		mov	eax,zb.pi.grad_x
                sub     ebx,edx		; z += dz
	        mov	edx,i		; Get lighting
		sub	edx,eax		; i += di
		mov	i,edx
		
R34a:
		dec	edi
if	1
		cmp	edi,zb.tsl._end
		jbe	L50
endif
	        mov	edx,denominator
                mov     dest,edi
		mov	edi,dv_numerator
                mov     z,ebx
                mov     zdest,ebp
                mov     ecx,zb.pq.grad_x
                mov     ebp,du_numerator
		mov	ebx,v_numerator
                mov     eax,source	; Restore eax

	; Core perspective calculations

		pre
                add     esi,ebp
                sub     edx,ecx
                cmp     esi,edx
                jge     incub		; Jump if u not too large
		
R36:
		test    esi,esi
		jl	decub
R37:
		add     ebx,edi
		jl	decvb
                cmp     ebx,edx
                jge     incvb		; Jump if v not too small
R40:
		post1
		post2
		mov	denominator,edx
		mov	v_numerator,ebx
		mov	edi,dest
                cmp     edi,zb.tsl._end
		mov     ebp,zdest
                jmp     next_pixelbr

L50:
		popad
		ret
		
decuf:
                mov     ebp,du_numerator
decufloop:
                decu
                sub     ebp,zb.pq.grad_x
                add     esi,edx
                jnc     decufloop
		
                mov     du_numerator,ebp
		jmp	L37
		
incuf:
                mov     ecx,zb.pq.grad_x
		mov     ebp,du_numerator
                sub     esi,edx
		
incufloop:
                incu     		; Increment u
                add     ebp,ecx
                sub     esi,edx
                jge     incufloop
		add	esi,edx
		
                mov     du_numerator,ebp
		jmp	L37

decvf:
                mov     ebp,zb.pq.grad_x
decvfloop:       
                decv     		; Decrement v
                sub     edi,ebp
                add     ebx,edx
                jnc     decvfloop
		mov	dv_numerator,edi
		jmp	L40
		
incvf:
                mov     ecx,zb.pq.grad_x
                sub     ebx,edx
		
incvfloop:
		incv     		; Increment v
                add     edi,ecx
                sub     ebx,edx
                jge     incvfloop
		
		add	ebx,edx
		mov	dv_numerator,edi
		jmp	L40

		; Back
decub:
                mov     ebp,du_numerator
decubloop:
                decu
                sub     ebp,zb.pq.grad_x
                add     esi,edx
                jnc     decubloop
		
                mov     du_numerator,ebp
		jmp	R37
		
incub:
                mov     ecx,zb.pq.grad_x
		mov     ebp,du_numerator
                sub     esi,edx
		
incubloop:
                incu     		; Increment u
                add     ebp,ecx
                sub     esi,edx
                jge     incubloop
		add	esi,edx
		
                mov     du_numerator,ebp
		jmp	R37

decvb:
                mov     ebp,zb.pq.grad_x
decvbloop:       
                decv     		; Decrement v
                sub     edi,ebp
                add     ebx,edx
                jnc     decvbloop
		mov	dv_numerator,edi
		jmp	R40
		
incvb:
                mov     ecx,zb.pq.grad_x
                sub     ebx,edx
		
incvbloop:
		incv     		; Increment v
                add     edi,ecx
                sub     ebx,edx
                jge     incvbloop
		
		add	ebx,edx
		mov	dv_numerator,edi
		jmp	R40
		
name		endp

		endm

; Unlit version

ScanLinePIZ2TP	macro	name,pre,incu,decu,incv,decv,post1,post2
name		proc

		pushad

                mov     esi,zb.pu.current
                mov     ebx,zb.pv.current
                mov     eax,zb.pu.grad_x
                mov     edi,zb.pv.grad_x
                mov     edx,zb.pq.current
                mov     ecx,zb.pz.current
                mov     ebp,zb.tsl._end
		
		mov	dv_numerator,edi
		mov	denominator,edx
		mov	v_numerator,ebx
                mov     z,ecx
                mov     du_numerator,eax
		
                mov     ecx,zb.tsl.zstart
                mov     zdest,ecx
		
                mov     edi,zb.tsl.start
                mov     eax,zb.tsl.source
		
                cmp     edi,ebp
		mov	ebp,zdest
                ja      rtol		; Jump if right to left	
		
	        ; Left to right scan
		
                mov     dest,edi
L33:
		jae     L50		; Jump if end of scan
		
                mov     source,eax	; Free up eax
		mov	ebx,z		; Get new z value
		mov	dx,[ebp]
		ror	ebx,16
                cmp     bx,dx
                ja      L34		; Jump if new pixel behind old
		
                mov     edx,zb.texture_buffer
		add	eax,edx

		;; Get and test texel for transparency

		mov	cl,[eax]
if 1
		test	cl,cl		; Transparent?
		jz	L34
endif

		;; Store texel and z value

                mov     [edi],cl	; Store texel
                mov     [ebp],bx	; Store new z
L34:
                add     ebp,2		; zdest++
		ror	ebx,16
                mov     ecx,zb.pz.grad_x
                add     ebx,ecx
L34a:
		inc	edi
		cmp	edi,zb.tsl._end
		jae	L50
	        mov	edx,denominator
                mov     dest,edi
		mov	edi,dv_numerator
                mov     z,ebx
                mov     zdest,ebp
                mov     ecx,zb.pq.grad_x
                mov     ebp,du_numerator
		mov	ebx,v_numerator
                mov     eax,source	; Restore eax
		pre
                sub     esi,ebp
                add     edx,ecx
                cmp     esi,edx
                jl	L36

                mov     ecx,zb.pq.grad_x
		mov     ebp,du_numerator
                sub     esi,edx
incufloop:
                incu
                add     ebp,ecx
                sub     esi,edx
                jge     incufloop
		add	esi,edx
		
                mov     du_numerator,ebp
		jmp	L37
		
L36:            test    esi,esi
		jge	L37

                mov     ebp,du_numerator
		mov	ecx,zb.pq.grad_x
decufloop:
                decu
                sub     ebp,ecx
                add     esi,edx
                jnc     decufloop
		
                mov     du_numerator,ebp
		
L37:
		sub     ebx,edi
		jge	skipdecvf

                mov     ebp,zb.pq.grad_x
decvfloop:       
                decv
                sub     edi,ebp
                add     ebx,edx
                jnc     decvfloop
		mov	dv_numerator,edi
		jmp	L40
skipdecvf:
                cmp     ebx,edx
                jl      L40		; Jump if v not too small

                mov     ecx,zb.pq.grad_x
                sub     ebx,edx
		
incvfloop:
		incv
                add     edi,ecx
                sub     ebx,edx
                jge     incvfloop
		
		add	ebx,edx
		mov	dv_numerator,edi
L40:
		post1
		post2
		mov	denominator,edx
		mov	v_numerator,ebx
		mov	edi,dest
                cmp     edi,zb.tsl._end
		mov     ebp,zdest
                jmp     L33

rtol:
                mov     dest,edi
R33:
		jbe     L50		; Jump if end of scan
		
                mov     source,eax	; Free up eax
		mov	ebx,z		; Get new z value
		mov	dx,[ebp]
		ror	ebx,16
                cmp     bx,dx
                ja      R34		; Jump if new pixel behind old
		
                mov     edx,zb.texture_buffer
		add	eax,edx

		;; Get texel and test for transparency

		mov	cl,[eax]
if 1
		test	cl,cl		; Transparent?
		jz	R34
endif

		;; Store texel and z value

                mov     [edi],cl	; Store texel
                mov     [ebp],bx	; Store new z

R34:
		ror	ebx,16
                mov     ecx,zb.pz.grad_x
                sub     ebp,2
                sub     ebx,ecx
R34a:
		dec	edi
		cmp	edi,zb.tsl._end
		jbe	L50
	        mov	edx,denominator
                mov     dest,edi
		mov	edi,dv_numerator
                mov     z,ebx
                mov     zdest,ebp
                mov     ecx,zb.pq.grad_x
                mov     ebp,du_numerator
		mov	ebx,v_numerator
                mov     eax,source	; Restore eax
		pre
                add     esi,ebp
                sub     edx,ecx
                cmp     esi,edx
                jl      R36

                mov     ecx,zb.pq.grad_x
		mov     ebp,du_numerator
                sub     esi,edx
incubloop:
                incu
                add     ebp,ecx
                sub     esi,edx
                jge     incubloop

		add	esi,edx
                mov     du_numerator,ebp
		jmp	R37
		
R36:            test    esi,esi
		jge	R37

                mov     ebp,du_numerator
		mov	ecx,zb.pq.grad_x
decubloop:
                decu
                sub     ebp,ecx
                add     esi,edx
                jnc     decubloop
		
                mov     du_numerator,ebp
R37:
		add     ebx,edi
		jge	skipdecvb

                mov     ebp,zb.pq.grad_x
decvbloop:       
                decv
                sub     edi,ebp
                add     ebx,edx
                jnc     decvbloop
		mov	dv_numerator,edi
		jmp	R40
skipdecvb:
                cmp     ebx,edx
                jl      R40		; Jump if v not too small

                mov     ecx,zb.pq.grad_x
                sub     ebx,edx
		
incvbloop:
		incv
                add     edi,ecx
                sub     ebx,edx
                jge     incvbloop
		
		add	ebx,edx
		mov	dv_numerator,edi
		
R40:
		post1
		post2
		mov	denominator,edx
		mov	v_numerator,ebx
		mov	edi,dest
                cmp     edi,zb.tsl._end
		mov     ebp,zdest
                jmp     R33
		
L50:
		popad
		ret
		
name		endp
		endm


; a += b*frac/16

dither		macro	a,b,frac

		push	b

		ifidni	<frac>,<0>
		elseifidni	<frac>,<1>	
		sar	b,4
		add	a,b
		elseifidni	<frac>,<2>	
		sar	b,3
		add	a,b
		elseifidni	<frac>,<3>	
		sar	b,3
		add	a,b
		sar	b,1
		add	a,b
		elseifidni	<frac>,<4>	
		sar	b,2
		add	a,b
		elseifidni	<frac>,<5>	
		sar	b,2
		add	a,b
		sar	b,2
		add	a,b
		elseifidni	<frac>,<6>	
		sar	b,2
		add	a,b
		sar	b,1
		add	a,b
		elseifidni	<frac>,<7>	
		sar	b,2
		add	a,b
		sar	b,1
		add	a,b
		sar	b,1
		add	a,b
		elseifidni	<frac>,<8>	
		sar	b,1
		add	a,b
		elseifidni	<frac>,<9>	
		sar	b,1
		add	a,b
		sar	b,3
		add	a,b
		elseifidni	<frac>,<10>	
		sar	b,1
		add	a,b
		sar	b,2
		add	a,b
		elseifidni	<frac>,<11>	
		sar	b,1
		add	a,b
		sar	b,2
		add	a,b
		sar	b,1
		add	a,b
		elseifidni	<frac>,<12>	
		sar	b,1
		add	a,b
		sar	b,1
		add	a,b
		elseifidni	<frac>,<13>	
		sar	b,1
		add	a,b
		sar	b,1
		add	a,b
		sar	b,2
		add	a,b
		elseifidni	<frac>,<14>	
		sar	b,1
		add	a,b
		sar	b,1
		add	a,b
		sar	b,1
		add	a,b
		elseifidni	<frac>,<15>	
		sar	b,1
		add	a,b
		sar	b,1
		add	a,b
		sar	b,1
		add	a,b
		sar	b,1
		add	a,b
		elseifidni	<frac>,<16>
		add	a,b
		elseifidni	<frac>,<-1>	
		sar	b,4
		sub	a,b
		elseifidni	<frac>,<-2>	
		sar	b,3
		sub	a,b
		elseifidni	<frac>,<-3>	
		sar	b,3
		sub	a,b
		sar	b,1
		sub	a,b
		elseifidni	<frac>,<-4>	
		sar	b,2
		sub	a,b
		elseifidni	<frac>,<-5>	
		sar	b,2
		sub	a,b
		sar	b,2
		sub	a,b
		elseifidni	<frac>,<-6>	
		sar	b,2
		sub	a,b
		sar	b,1
		sub	a,b
		elseifidni	<frac>,<-7>	
		sar	b,2
		sub	a,b
		sar	b,1
		sub	a,b
		sar	b,1
		sub	a,b
		elseifidni	<frac>,<-8>	
		sar	b,1
		sub	a,b
		elseifidni	<frac>,<-9>	
		sar	b,1
		sub	a,b
		sar	b,3
		sub	a,b
		elseifidni	<frac>,<-10>	
		sar	b,1
		sub	a,b
		sar	b,2
		sub	a,b
		elseifidni	<frac>,<-11>	
		sar	b,1
		sub	a,b
		sar	b,2
		sub	a,b
		sar	b,1
		sub	a,b
		elseifidni	<frac>,<-12>	
		sar	b,1
		sub	a,b
		sar	b,1
		sub	a,b
		elseifidni	<frac>,<-13>	
		sar	b,1
		sub	a,b
		sar	b,1
		sub	a,b
		sar	b,2
		sub	a,b
		elseifidni	<frac>,<-14>	
		sar	b,1
		sub	a,b
		sar	b,1
		sub	a,b
		sar	b,1
		sub	a,b
		elseifidni	<frac>,<-15>	
		sar	b,1
		sub	a,b
		sar	b,1
		sub	a,b
		sar	b,1
		sub	a,b
		sar	b,1
		sub	a,b
		endif

		pop	b

		endm

; Dithered perspective

ScanLinePIZ2TPDn	macro	name,pre,incu,decu,incv,decv,post1,post2,\
			d0,d1,d2,d3
name		proc

		pushad

                mov     esi,zb.pu.current
                mov     ebx,zb.pv.current
                mov     eax,zb.pu.grad_x
                mov     edi,zb.pv.grad_x
                mov     edx,zb.pq.current

		mov	v_numerator,ebx
                mov     du_numerator,eax
		mov	dv_numerator,edi
		mov	denominator,edx

                mov     ecx,zb.pz.current
                mov     edx,zb.tsl.zstart

                mov     z,ecx
                mov     zdest,edx

                mov     eax,zb.tsl.source
                mov     edi,zb.tsl.start
                mov     ebp,zb.tsl._end

		mov	source,eax
                mov     dest,edi

                cmp     edi,ebp
		je	L50
                ja      rtol		; Jump if right to left	
		
	        ; Left to right scan
		
		and	edi,3
		jmp	table_f_&name&[edi*4]

		.data
table_f_&name&	label	ptr dword
		dd	init_forward0
		dd	init_forward1
		dd	init_forward2
		dd	init_forward3
		.code

		for	count,<0,1,2,3>
init_forward&count&:
	        mov	edx,denominator
		mov	ebx,v_numerator

		if	1		; Dithering?

; Initialise forward dither
; For dithering need esi += edx*fraction
;                and ebx += edx*fraction

		ifidni	<count>,<0>
		dither	esi,edx,d0
		dither	ebx,edx,d0
		elseifidni	<count>,<1>
		dither	esi,edx,d1
		dither	ebx,edx,d1
		elseifidni	<count>,<2>
		dither	esi,edx,d2
		dither	ebx,edx,d2
		elseifidni	<count>,<3>
		dither	esi,edx,d3
		dither	ebx,edx,d3
		endif

		endif

		jmp	forward&count&
		endm

L33:
		for	count,<0,1,2,3>
		local	transp2,L34,L34a,L36,L37
		local	skipdecvf,L40

	        mov	edx,denominator
		mov	ebx,v_numerator

forward&count&:
                mov     ecx,zb.pq.grad_x
                mov     ebp,du_numerator
		mov	edi,dv_numerator
                mov     eax,source

		pre
                cmp     esi,edx
                jl	L36

                sub     esi,edx		; u_numerator -= denominator
@@:
                incu
                add     ebp,ecx		; du_numerator += d_denominator
                sub     esi,edx		; u_numerator -= denominator
                jge     @B

		add	esi,edx		; u_numerator += denominator
		
                mov     du_numerator,ebp
		jmp	L37
		
L36:            test    esi,esi
		jge	L37

                mov     ebp,du_numerator
@@:
                decu
                sub     ebp,ecx		; du_numerator -= d_denominator
                add     esi,edx		; u_numerator -= denominator
                jnc     @B
		
                mov     du_numerator,ebp
L37:
		test	ebx,ebx		; test v_numerator
		jge	skipdecvf

@@:       
                decv
                sub     edi,ecx		; dv_numerator -= d_denominator
                add     ebx,edx		; v_numerator -= denominator
                jnc     @B

		mov	dv_numerator,edi
		jmp	L40
skipdecvf:
                cmp     ebx,edx		; Compare v_numerator and denominator
                jl      L40		; Jump if proper fraction

                sub     ebx,edx
		
@@:
		incv
                add     edi,ecx
                sub     ebx,edx
                jge     @B
		
		add	ebx,edx
		mov	dv_numerator,edi
L40:
		post1
		post2

;		U,V and W increments

                sub     esi,ebp		; u_numerator -= du_numerator
		sub     ebx,edi		; v_numerator -= dv_numerator
                add     edx,ecx		; demominator += d_denominator

		if	1		; Dithering?

; Dither forwards
; For dithering need esi (u_numerator) += edx*fraction
;                and ebx (v_numerator) += edx*fraction

		ifidni	<count>,<0>
		dither	esi,edx,%d1-d0
		dither	ebx,edx,%d1-d0
		elseifidni	<count>,<1>
		dither	esi,edx,%d2-d1
		dither	ebx,edx,%d2-d1
		elseifidni	<count>,<2>
		dither	esi,edx,%d3-d2
		dither	ebx,edx,%d3-d2
		elseifidni	<count>,<3>
		dither	esi,edx,%d0-d3
		dither	ebx,edx,%d0-d3
		endif

		endif

		mov	denominator,edx
		mov	v_numerator,ebx

; End of calcs 

		mov	edi,dest
		mov	ebp,zb.tsl._end
                cmp     edi,ebp
		jae	L50

		mov     ebp,zdest
                mov     source,eax	; Save source
		mov	ebx,z		; Get new z value
		mov	dx,[ebp]
		ror	ebx,16
                cmp     bx,dx
                ja      @F		; Jump if new pixel behind old
		
                mov     edx,zb.texture_buffer
		add	eax,edx

		;; Fetch texel and test for transparency

		mov	cl,[eax]
if 1
		test	cl,cl		; Transparent?
		jz	@F
endif

		;; Store texel and z value

                mov     [edi],cl	; Store texel
                mov     [ebp],bx	; Store new z
@@:
                add     ebp,2		; zdest++
		ror	ebx,16
                mov     ecx,zb.pz.grad_x
                add     ebx,ecx		; z += dz
L34a:
		mov	zdest,ebp
		mov	z,ebx

		inc	edi
		cmp	edi,zb.tsl._end
		jae	L50
		mov	dest,edi

		endm

		jmp	L33

	; Right to left scan

rtol:
		and	edi,3
		jmp	table_b_&name&[edi*4]

		.data
table_b_&name&	label	ptr dword
		dd	init_backward0
		dd	init_backward1
		dd	init_backward2
		dd	init_backward3
		.code

		for	count,<0,1,2,3>
init_backward&count&:
	        mov	edx,denominator
		mov	ebx,v_numerator

		if	1		; Dithering?

; Initialise backward dither
; For dithering need esi += edx*fraction
;                and ebx += edx*fraction

		ifidni	<count>,<0>
		dither	esi,edx,d0
		dither	ebx,edx,d0
		elseifidni	<count>,<1>
		dither	esi,edx,d1
		dither	ebx,edx,d1
		elseifidni	<count>,<2>
		dither	esi,edx,d2
		dither	ebx,edx,d2
		elseifidni	<count>,<3>
		dither	esi,edx,d3
		dither	ebx,edx,d3
		endif

		endif

		jmp	backward&count&
		endm
R33:
		for	count,<3,2,1,0>
		local	R33,transp2r,R34,R34a
		local	R36,R37
		local	skipdecvb,R40

		mov	edx,denominator
		mov	ebx,v_numerator

backward&count&:
                mov     ecx,zb.pq.grad_x
                mov     ebp,du_numerator
		mov	edi,dv_numerator
                mov     eax,source	

		pre
                cmp     esi,edx
                jl      R36

                sub     esi,edx		; u_numerator -= denominator
@@:
                incu
                add     ebp,ecx
                sub     esi,edx
                jge     @B

		add	esi,edx

                mov     du_numerator,ebp
		jmp	R37
		
R36:            test    esi,esi
		jge	R37

                mov     ebp,du_numerator
@@:
                decu
                sub     ebp,ecx		; du_numerator -= d_denominator
                add     esi,edx		; u_numerator -= denominator
                jnc     @B
		
                mov     du_numerator,ebp
R37:
		test    ebx,ebx		; test v_numerator
		jge	skipdecvb

@@:       
                decv
                sub     edi,ecx
                add     ebx,edx
                jnc     @B

		mov	dv_numerator,edi
		jmp	R40
skipdecvb:
                cmp     ebx,edx
                jl      R40		; Jump if v not too small

                sub     ebx,edx
		
@@:
		incv
                add     edi,ecx
                sub     ebx,edx
                jge     @B
		
		add	ebx,edx
		mov	dv_numerator,edi
		
R40:
		post1
		post2

;		U,V and W increments

                add     esi,ebp		; u_numerator += du_numerator
		add     ebx,edi		; v_numerator += dv_numerator
                sub     edx,ecx		; demominator -= d_denominator

		if	1		; Dithering?

; Dither backwards
; For dithering need esi (u_numerator) += edx*fraction
;                and ebx (v_numerator) += edx*fraction

		ifidni	<count>,<0>
		dither	esi,edx,%d3-d0
		dither	ebx,edx,%d3-d0
		elseifidni	<count>,<1>
		dither	esi,edx,%d0-d1
		dither	ebx,edx,%d0-d1
		elseifidni	<count>,<2>
		dither	esi,edx,%d1-d2
		dither	ebx,edx,%d1-d2
		elseifidni	<count>,<3>
		dither	esi,edx,%d2-d3
		dither	ebx,edx,%d2-d3
		endif

		endif
		mov	denominator,edx
		mov	v_numerator,ebx

; End of calcs

		mov	edi,dest
		mov	ebp,zb.tsl._end
                cmp     edi,ebp
		jbe	L50

		mov     ebp,zdest
                mov     source,eax	; Save source
		mov	ebx,z		; Get new z value
		mov	dx,[ebp]
		ror	ebx,16
                cmp     bx,dx
                ja      @F		; Jump if new pixel behind old
		
                mov     edx,zb.texture_buffer
		add	eax,edx

		;; Fetch texel and text for transparency

		mov	cl,[eax]
if 1
		test	cl,cl		; Transparent?
		jz	@F
endif

                mov     [ebp],bx	; Store new z
                mov     [edi],cl	; Store texel

@@:
                sub     ebp,2		; zdest++
		ror	ebx,16
                mov     ecx,zb.pz.grad_x
                sub     ebx,ecx
R34a:
		mov	zdest,ebp
		mov	z,ebx

		dec	edi
		cmp	edi,zb.tsl._end
                mov     dest,edi
		jbe	L50
		
		endm

		jmp	R33
		
L50:
		popad
		ret
		
name		endp

		endm

    ; Dithered texture mapping 

ScanLinePIZ2TPD	macro	name,pre,incu,decu,incv,decv,post1,post2

ScanLinePIZ2TPDn	&name&_0,\
		pre,incu,decu,incv,decv,post1,post2,\
		13,1,4,16

ScanLinePIZ2TPDn	&name&_1,\
		pre,incu,decu,incv,decv,post1,post2,\
		8,12,9,5

ScanLinePIZ2TPDn	&name&_2,\
		pre,incu,decu,incv,decv,post1,post2,\
		10,6,7,11

ScanLinePIZ2TPDn	&name&_3,\
		pre,incu,decu,incv,decv,post1,post2,\
		3,15,14,2

		.data
&name&_table		label	ptr dword
		dd	&name&_0
		dd	&name&_1
		dd	&name&_2
		dd	&name&_3
		.code

name		proc

		mov	eax,zb.tsl.y
		and	eax,3
		jmp	&name&_table[4*eax]

name		endp

		endm

ScanLinePIZ2TIP	ScanLinePIZ2TIP256,\
		<>,\
		<inc al>,<dec al>,<inc ah>,<dec ah>,\
		<>,<>

ScanLinePIZ2TIP	ScanLinePIZ2TIP64,\
		<shl eax,2>,\
		<add al,100b>,<sub al,100b>,<inc ah>,<dec ah>,\
		<shr eax,2>,<and eax,0fffh>

ScanLinePIZ2TIP	ScanLinePIZ2TIP1024,\
		<shl eax,6>,\
		<add ax,40h>,<sub ax,40h>,<add eax,10000h>,<sub eax,10000h>,\
		<shr eax,6>,<and eax,0fffffh>

ScanLinePIZ2TP	ScanLinePIZ2TP64,\
		<shl eax,2>,\
		<add al,100b>,<sub al,100b>,<inc ah>,<dec ah>,\
		<shr eax,2>,<and eax,0fffh>

ScanLinePIZ2TP	ScanLinePIZ2TP1024,\
		<shl eax,6>,\
		<add ax,40h>,<sub ax,40h>,<add eax,10000h>,<sub eax,10000h>,\
		<shr eax,6>,<and eax,0fffffh>

ScanLinePIZ2TP	ScanLinePIZ2TP256,\
		<>,\
		<inc al>,<dec al>,<inc ah>,<dec ah>,\
		<>,<>

ScanLinePIZ2TPD	ScanLinePIZ2TPD1024,\
		<<shl eax,6>>,\
		<<add ax,40h>>,<<sub ax,40h>>,<<add eax,10000h>>,<<sub eax,10000h>>,\
		<<shr eax,6>>,<<and eax,0fffffh>>

		end
