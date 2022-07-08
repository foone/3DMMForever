// clang-format off
	// Setup
	long cbTot;
	byte *pbLimDst = (byte *)pvDst + cbDst;
	byte *pbLimSrc = (byte *)pvSrc + cbSrc - kcbTailKcd2;

   ulong eax = 0;
   byte *ebx = 0;
   ulong ecx = 0;
   ulong edx = 0;
   byte *edi = 0;
   byte *esi = 0;
   ulong esi_u = 0;
   bool tst = false;

   edi = (byte*)pvDst;
   ebx = (byte*)pvSrc + 1;
	eax = *(ulong*)ebx;
	ebx += 4;
	goto LBlock0;

LLitSingle7:
#ifdef SAFETY
   if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 8;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LBlock0;

LLitByte7:
#ifdef SAFETY
	if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
#ifdef SAFETY
	esi = ebx - 3 + ecx;
	if (esi > pbLimSrc) goto LFail;
	esi -= ecx;
#else //!SAFETY
	esi = ebx - 3;
#endif //!SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;
	ebx = esi + 4;
	eax = *(ulong*)esi;
	goto LBlock0;

L6BitOffByte0:
	esi_u = ((eax >> 2) & 63) + 1;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte0;

L9BitOffByte4:
	esi_u = ((eax >> 7) & 511) + 65;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte0;

L12BitOffByte0:
	esi_u = ((eax >> 4) & 4095) + 577;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte0;

L20BitOffByte0:
	esi_u = ((eax >> 4) & 1048575) + 4673;
	ecx += 2;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	goto LCopyByte0;

	// Copy Byte 0
LCopyByte0:
#ifdef SAFETY
   if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	esi = edi - esi_u;
#ifdef SAFETY
	if (esi < pvDst) goto LFail;
#endif //SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;

	// Block 0
LBlock0:
	tst = eax & 1;
	if (!tst) goto LLen0_0;
	tst = eax & 2;
	if (!tst) goto LLen0_1;
	tst = eax & 4;
	if (!tst) goto LLen0_2;
	tst = eax & 8;
	if (!tst) goto LLen0_3;
	tst = eax & 16;
	if (!tst) goto LLen0_4;
	tst = eax & 32;
	if (!tst) goto LLen0_5;
	tst = eax & 64;
	if (!tst) goto LLen0_6;
	tst = eax & 128;
	if (!tst) goto LLen0_7;
	tst = eax & 256;
	if (!tst) goto LLen0_8;
	tst = eax & 512;
	if (!tst) goto LLen0_9;
	tst = eax & 1024;
	if (!tst) goto LLen0_10;
	tst = eax & 2048;
	if (!tst) goto LLen0_11;
	goto LDone;
LLen0_0:
	ecx = 1;
	tst = eax & 2;
	if (!tst) goto LLitSingle1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen0_1:
	ecx = ((eax >> 2) & 1) | 2;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen0_2:
	ecx = ((eax >> 3) & 3) | 4;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen0_3:
	ecx = ((eax >> 4) & 7) | 8;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen0_4:
	ecx = ((eax >> 5) & 15) | 16;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen0_5:
	ecx = ((eax >> 6) & 31) | 32;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen0_6:
	ecx = ((eax >> 7) & 63) | 64;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen0_7:
	ecx = ((eax >> 8) & 127) | 128;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen0_8:
	ecx = ((eax >> 9) & 255) | 256;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen0_9:
	ecx = ((eax >> 10) & 511) | 512;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen0_10:
	ecx = ((eax >> 11) & 1023) | 1024;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen0_11:
	ecx = ((eax >> 12) & 2047) | 2048;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;

LLitSingle0:
#ifdef SAFETY
   if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock1;

LLitByte0:
#ifdef SAFETY
	if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	ecx--;
	edx = (eax >> 1) & 127;
#ifdef SAFETY
	esi = ebx - 3 + ecx;
	if (esi > pbLimSrc) goto LFail;
	esi -= ecx;
#else //!SAFETY
	esi = ebx - 3;
#endif //!SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;
	ebx = esi + 4;
	eax = *(ulong*)esi;
	esi_u = eax << 7;
	edx |= esi_u;
	*edi++ = edx;
	goto LBlock1;

L6BitOffByte1:
	esi_u = ((eax >> 3) & 63) + 1;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte1;

L9BitOffByte5:
	esi_u = ((eax >> 8) & 511) + 65;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte1;

L12BitOffByte1:
	esi_u = ((eax >> 5) & 4095) + 577;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte1;

L20BitOffByte1:
	esi_u = ((eax >> 5) & 1048575) + 4673;
	ecx += 2;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	goto LCopyByte1;

	// Copy Byte 1
LCopyByte1:
#ifdef SAFETY
   if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	esi = edi - esi_u;
#ifdef SAFETY
	if (esi < pvDst) goto LFail;
#endif //SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;

	// Block 1
LBlock1:
	tst = eax & 2;
	if (!tst) goto LLen1_0;
	tst = eax & 4;
	if (!tst) goto LLen1_1;
	tst = eax & 8;
	if (!tst) goto LLen1_2;
	tst = eax & 16;
	if (!tst) goto LLen1_3;
	tst = eax & 32;
	if (!tst) goto LLen1_4;
	tst = eax & 64;
	if (!tst) goto LLen1_5;
	tst = eax & 128;
	if (!tst) goto LLen1_6;
	tst = eax & 256;
	if (!tst) goto LLen1_7;
	tst = eax & 512;
	if (!tst) goto LLen1_8;
	tst = eax & 1024;
	if (!tst) goto LLen1_9;
	tst = eax & 2048;
	if (!tst) goto LLen1_10;
	tst = eax & 4096;
	if (!tst) goto LLen1_11;
	goto LDone;
LLen1_0:
	ecx = 1;
	tst = eax & 4;
	if (!tst) goto LLitSingle2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen1_1:
	ecx = ((eax >> 3) & 1) | 2;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen1_2:
	ecx = ((eax >> 4) & 3) | 4;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen1_3:
	ecx = ((eax >> 5) & 7) | 8;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen1_4:
	ecx = ((eax >> 6) & 15) | 16;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen1_5:
	ecx = ((eax >> 7) & 31) | 32;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen1_6:
	ecx = ((eax >> 8) & 63) | 64;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen1_7:
	ecx = ((eax >> 9) & 127) | 128;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen1_8:
	ecx = ((eax >> 10) & 255) | 256;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen1_9:
	ecx = ((eax >> 11) & 511) | 512;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen1_10:
	ecx = ((eax >> 12) & 1023) | 1024;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen1_11:
	ecx = ((eax >> 13) & 2047) | 2048;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;

LLitSingle1:
#ifdef SAFETY
   if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 2;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock2;

LLitByte1:
#ifdef SAFETY
	if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	ecx--;
	edx = (eax >> 2) & 63;
#ifdef SAFETY
	esi = ebx - 3 + ecx;
	if (esi > pbLimSrc) goto LFail;
	esi -= ecx;
#else //!SAFETY
	esi = ebx - 3;
#endif //!SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;
	ebx = esi + 4;
	eax = *(ulong*)esi;
	esi_u = eax << 6;
	edx |= esi_u;
	*edi++ = edx;
	goto LBlock2;

L6BitOffByte2:
	esi_u = ((eax >> 4) & 63) + 1;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte2;

L9BitOffByte6:
	esi_u = ((eax >> 9) & 511) + 65;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte2;

L12BitOffByte2:
	esi_u = ((eax >> 6) & 4095) + 577;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte2;

L20BitOffByte2:
	esi_u = ((eax >> 6) & 1048575) + 4673;
	ecx += 2;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	goto LCopyByte2;

	// Copy Byte 2
LCopyByte2:
#ifdef SAFETY
   if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	esi = edi - esi_u;
#ifdef SAFETY
	if (esi < pvDst) goto LFail;
#endif //SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;

	// Block 2
LBlock2:
	tst = eax & 4;
	if (!tst) goto LLen2_0;
	tst = eax & 8;
	if (!tst) goto LLen2_1;
	tst = eax & 16;
	if (!tst) goto LLen2_2;
	tst = eax & 32;
	if (!tst) goto LLen2_3;
	tst = eax & 64;
	if (!tst) goto LLen2_4;
	tst = eax & 128;
	if (!tst) goto LLen2_5;
	tst = eax & 256;
	if (!tst) goto LLen2_6;
	tst = eax & 512;
	if (!tst) goto LLen2_7;
	tst = eax & 1024;
	if (!tst) goto LLen2_8;
	tst = eax & 2048;
	if (!tst) goto LLen2_9;
	tst = eax & 4096;
	if (!tst) goto LLen2_10;
	tst = eax & 8192;
	if (!tst) goto LLen2_11;
	goto LDone;
LLen2_0:
	ecx = 1;
	tst = eax & 8;
	if (!tst) goto LLitSingle3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen2_1:
	ecx = ((eax >> 4) & 1) | 2;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen2_2:
	ecx = ((eax >> 5) & 3) | 4;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen2_3:
	ecx = ((eax >> 6) & 7) | 8;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen2_4:
	ecx = ((eax >> 7) & 15) | 16;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen2_5:
	ecx = ((eax >> 8) & 31) | 32;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen2_6:
	ecx = ((eax >> 9) & 63) | 64;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen2_7:
	ecx = ((eax >> 10) & 127) | 128;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen2_8:
	ecx = ((eax >> 11) & 255) | 256;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen2_9:
	ecx = ((eax >> 12) & 511) | 512;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen2_10:
	ecx = ((eax >> 13) & 1023) | 1024;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen2_11:
	ecx = ((eax >> 14) & 2047) | 2048;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;

LLitSingle2:
#ifdef SAFETY
   if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 3;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock3;

LLitByte2:
#ifdef SAFETY
	if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	ecx--;
	edx = (eax >> 3) & 31;
#ifdef SAFETY
	esi = ebx - 3 + ecx;
	if (esi > pbLimSrc) goto LFail;
	esi -= ecx;
#else //!SAFETY
	esi = ebx - 3;
#endif //!SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;
	ebx = esi + 4;
	eax = *(ulong*)esi;
	esi_u = eax << 5;
	edx |= esi_u;
	*edi++ = edx;
	goto LBlock3;

L6BitOffByte3:
	esi_u = ((eax >> 5) & 63) + 1;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte3;

L9BitOffByte7:
	esi_u = ((eax >> 10) & 511) + 65;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte3;

L12BitOffByte3:
	esi_u = ((eax >> 7) & 4095) + 577;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte3;

L20BitOffByte3:
	esi_u = ((eax >> 7) & 1048575) + 4673;
	ecx += 2;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	goto LCopyByte3;

	// Copy Byte 3
LCopyByte3:
#ifdef SAFETY
   if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	esi = edi - esi_u;
#ifdef SAFETY
	if (esi < pvDst) goto LFail;
#endif //SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;

	// Block 3
LBlock3:
	tst = eax & 8;
	if (!tst) goto LLen3_0;
	tst = eax & 16;
	if (!tst) goto LLen3_1;
	tst = eax & 32;
	if (!tst) goto LLen3_2;
	tst = eax & 64;
	if (!tst) goto LLen3_3;
	tst = eax & 128;
	if (!tst) goto LLen3_4;
	tst = eax & 256;
	if (!tst) goto LLen3_5;
	tst = eax & 512;
	if (!tst) goto LLen3_6;
	tst = eax & 1024;
	if (!tst) goto LLen3_7;
	tst = eax & 2048;
	if (!tst) goto LLen3_8;
	tst = eax & 4096;
	if (!tst) goto LLen3_9;
	tst = eax & 8192;
	if (!tst) goto LLen3_10;
	tst = eax & 16384;
	if (!tst) goto LLen3_11;
	goto LDone;
LLen3_0:
	ecx = 1;
	tst = eax & 16;
	if (!tst) goto LLitSingle4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen3_1:
	ecx = ((eax >> 5) & 1) | 2;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen3_2:
	ecx = ((eax >> 6) & 3) | 4;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen3_3:
	ecx = ((eax >> 7) & 7) | 8;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen3_4:
	ecx = ((eax >> 8) & 15) | 16;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen3_5:
	ecx = ((eax >> 9) & 31) | 32;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen3_6:
	ecx = ((eax >> 10) & 63) | 64;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen3_7:
	ecx = ((eax >> 11) & 127) | 128;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen3_8:
	ecx = ((eax >> 12) & 255) | 256;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen3_9:
	ecx = ((eax >> 13) & 511) | 512;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen3_10:
	ecx = ((eax >> 14) & 1023) | 1024;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen3_11:
	ecx = ((eax >> 15) & 2047) | 2048;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;

LLitSingle3:
#ifdef SAFETY
   if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 4;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock4;

LLitByte3:
#ifdef SAFETY
	if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	ecx--;
	edx = (eax >> 4) & 15;
#ifdef SAFETY
	esi = ebx - 3 + ecx;
	if (esi > pbLimSrc) goto LFail;
	esi -= ecx;
#else //!SAFETY
	esi = ebx - 3;
#endif //!SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;
	ebx = esi + 4;
	eax = *(ulong*)esi;
	esi_u = eax << 4;
	edx |= esi_u;
	*edi++ = edx;
	goto LBlock4;

L6BitOffByte4:
	esi_u = ((eax >> 6) & 63) + 1;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte4;

L9BitOffByte0:
	esi_u = ((eax >> 3) & 511) + 65;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte4;

L12BitOffByte4:
	esi_u = ((eax >> 8) & 4095) + 577;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte4;

L20BitOffByte4:
	esi_u = ((eax >> 8) & 1048575) + 4673;
	ecx += 2;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	goto LCopyByte4;

	// Copy Byte 4
LCopyByte4:
#ifdef SAFETY
   if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	esi = edi - esi_u;
#ifdef SAFETY
	if (esi < pvDst) goto LFail;
#endif //SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;

	// Block 4
LBlock4:
	tst = eax & 16;
	if (!tst) goto LLen4_0;
	tst = eax & 32;
	if (!tst) goto LLen4_1;
	tst = eax & 64;
	if (!tst) goto LLen4_2;
	tst = eax & 128;
	if (!tst) goto LLen4_3;
	tst = eax & 256;
	if (!tst) goto LLen4_4;
	tst = eax & 512;
	if (!tst) goto LLen4_5;
	tst = eax & 1024;
	if (!tst) goto LLen4_6;
	tst = eax & 2048;
	if (!tst) goto LLen4_7;
	tst = eax & 4096;
	if (!tst) goto LLen4_8;
	tst = eax & 8192;
	if (!tst) goto LLen4_9;
	tst = eax & 16384;
	if (!tst) goto LLen4_10;
	tst = eax & 32768;
	if (!tst) goto LLen4_11;
	goto LDone;
LLen4_0:
	ecx = 1;
	tst = eax & 32;
	if (!tst) goto LLitSingle5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen4_1:
	ecx = ((eax >> 6) & 1) | 2;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen4_2:
	ecx = ((eax >> 7) & 3) | 4;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen4_3:
	ecx = ((eax >> 8) & 7) | 8;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen4_4:
	ecx = ((eax >> 9) & 15) | 16;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen4_5:
	ecx = ((eax >> 10) & 31) | 32;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen4_6:
	ecx = ((eax >> 11) & 63) | 64;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen4_7:
	ecx = ((eax >> 12) & 127) | 128;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen4_8:
	ecx = ((eax >> 13) & 255) | 256;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen4_9:
	ecx = ((eax >> 14) & 511) | 512;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen4_10:
	ecx = ((eax >> 15) & 1023) | 1024;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen4_11:
	ecx = ((eax >> 16) & 2047) | 2048;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;

LLitSingle4:
#ifdef SAFETY
   if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 5;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock5;

LLitByte4:
#ifdef SAFETY
	if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	ecx--;
	edx = (eax >> 5) & 7;
#ifdef SAFETY
	esi = ebx - 3 + ecx;
	if (esi > pbLimSrc) goto LFail;
	esi -= ecx;
#else //!SAFETY
	esi = ebx - 3;
#endif //!SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;
	ebx = esi + 4;
	eax = *(ulong*)esi;
	esi_u = eax << 3;
	edx |= esi_u;
	*edi++ = edx;
	goto LBlock5;

L6BitOffByte5:
	esi_u = ((eax >> 7) & 63) + 1;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte5;

L9BitOffByte1:
	esi_u = ((eax >> 4) & 511) + 65;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte5;

L12BitOffByte5:
	esi_u = ((eax >> 9) & 4095) + 577;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte5;

L20BitOffByte5:
	esi_u = ((eax >> 9) & 1048575) + 4673;
	ecx += 2;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	goto LCopyByte5;

	// Copy Byte 5
LCopyByte5:
#ifdef SAFETY
   if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	esi = edi - esi_u;
#ifdef SAFETY
	if (esi < pvDst) goto LFail;
#endif //SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;

	// Block 5
LBlock5:
	tst = eax & 32;
	if (!tst) goto LLen5_0;
	tst = eax & 64;
	if (!tst) goto LLen5_1;
	tst = eax & 128;
	if (!tst) goto LLen5_2;
	tst = eax & 256;
	if (!tst) goto LLen5_3;
	tst = eax & 512;
	if (!tst) goto LLen5_4;
	tst = eax & 1024;
	if (!tst) goto LLen5_5;
	tst = eax & 2048;
	if (!tst) goto LLen5_6;
	tst = eax & 4096;
	if (!tst) goto LLen5_7;
	tst = eax & 8192;
	if (!tst) goto LLen5_8;
	tst = eax & 16384;
	if (!tst) goto LLen5_9;
	tst = eax & 32768;
	if (!tst) goto LLen5_10;
	tst = eax & 65536;
	if (!tst) goto LLen5_11;
	goto LDone;
LLen5_0:
	ecx = 1;
	tst = eax & 64;
	if (!tst) goto LLitSingle6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen5_1:
	ecx = ((eax >> 7) & 1) | 2;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen5_2:
	ecx = ((eax >> 8) & 3) | 4;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen5_3:
	ecx = ((eax >> 9) & 7) | 8;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen5_4:
	ecx = ((eax >> 10) & 15) | 16;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen5_5:
	ecx = ((eax >> 11) & 31) | 32;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen5_6:
	ecx = ((eax >> 12) & 63) | 64;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen5_7:
	ecx = ((eax >> 13) & 127) | 128;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen5_8:
	ecx = ((eax >> 14) & 255) | 256;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen5_9:
	ecx = ((eax >> 15) & 511) | 512;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen5_10:
	ecx = ((eax >> 16) & 1023) | 1024;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen5_11:
	ecx = ((eax >> 17) & 2047) | 2048;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;

LLitSingle5:
#ifdef SAFETY
   if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 6;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock6;

LLitByte5:
#ifdef SAFETY
	if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	ecx--;
	edx = (eax >> 6) & 3;
#ifdef SAFETY
	esi = ebx - 3 + ecx;
	if (esi > pbLimSrc) goto LFail;
	esi -= ecx;
#else //!SAFETY
	esi = ebx - 3;
#endif //!SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;
	ebx = esi + 4;
	eax = *(ulong*)esi;
	esi_u = eax << 2;
	edx |= esi_u;
	*edi++ = edx;
	goto LBlock6;

L6BitOffByte6:
	esi_u = ((eax >> 8) & 63) + 1;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte6;

L9BitOffByte2:
	esi_u = ((eax >> 5) & 511) + 65;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte6;

L12BitOffByte6:
	esi_u = ((eax >> 10) & 4095) + 577;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte6;

L20BitOffByte6:
	esi_u = ((eax >> 10) & 1048575) + 4673;
	ecx += 2;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	goto LCopyByte6;

	// Copy Byte 6
LCopyByte6:
#ifdef SAFETY
   if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	esi = edi - esi_u;
#ifdef SAFETY
	if (esi < pvDst) goto LFail;
#endif //SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;

	// Block 6
LBlock6:
	tst = eax & 64;
	if (!tst) goto LLen6_0;
	tst = eax & 128;
	if (!tst) goto LLen6_1;
	tst = eax & 256;
	if (!tst) goto LLen6_2;
	tst = eax & 512;
	if (!tst) goto LLen6_3;
	tst = eax & 1024;
	if (!tst) goto LLen6_4;
	tst = eax & 2048;
	if (!tst) goto LLen6_5;
	tst = eax & 4096;
	if (!tst) goto LLen6_6;
	tst = eax & 8192;
	if (!tst) goto LLen6_7;
	tst = eax & 16384;
	if (!tst) goto LLen6_8;
	tst = eax & 32768;
	if (!tst) goto LLen6_9;
	tst = eax & 65536;
	if (!tst) goto LLen6_10;
	tst = eax & 131072;
	if (!tst) goto LLen6_11;
	goto LDone;
LLen6_0:
	ecx = 1;
	tst = eax & 128;
	if (!tst) goto LLitSingle7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen6_1:
	ecx = ((eax >> 8) & 1) | 2;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen6_2:
	ecx = ((eax >> 9) & 3) | 4;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen6_3:
	ecx = ((eax >> 10) & 7) | 8;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen6_4:
	ecx = ((eax >> 11) & 15) | 16;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen6_5:
	ecx = ((eax >> 12) & 31) | 32;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen6_6:
	ecx = ((eax >> 13) & 63) | 64;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen6_7:
	ecx = ((eax >> 14) & 127) | 128;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;
LLen6_8:
	ecx = ((eax >> 15) & 255) | 256;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 128;
	if (!tst) goto LLitByte7;
	tst = eax & 256;
	if (!tst) goto L6BitOffByte7;
	tst = eax & 512;
	if (!tst) goto L9BitOffByte7;
	tst = eax & 1024;
	if (!tst) goto L12BitOffByte7;
	goto L20BitOffByte7;
LLen6_9:
	ecx = ((eax >> 16) & 511) | 512;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 2;
	if (!tst) goto LLitByte1;
	tst = eax & 4;
	if (!tst) goto L6BitOffByte1;
	tst = eax & 8;
	if (!tst) goto L9BitOffByte1;
	tst = eax & 16;
	if (!tst) goto L12BitOffByte1;
	goto L20BitOffByte1;
LLen6_10:
	ecx = ((eax >> 17) & 1023) | 1024;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 8;
	if (!tst) goto LLitByte3;
	tst = eax & 16;
	if (!tst) goto L6BitOffByte3;
	tst = eax & 32;
	if (!tst) goto L9BitOffByte3;
	tst = eax & 64;
	if (!tst) goto L12BitOffByte3;
	goto L20BitOffByte3;
LLen6_11:
	ecx = ((eax >> 18) & 2047) | 2048;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 32;
	if (!tst) goto LLitByte5;
	tst = eax & 64;
	if (!tst) goto L6BitOffByte5;
	tst = eax & 128;
	if (!tst) goto L9BitOffByte5;
	tst = eax & 256;
	if (!tst) goto L12BitOffByte5;
	goto L20BitOffByte5;

LLitSingle6:
#ifdef SAFETY
   if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 7;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock7;

LLitByte6:
#ifdef SAFETY
	if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	ecx--;
	edx = (eax >> 7) & 1;
#ifdef SAFETY
	esi = ebx - 3 + ecx;
	if (esi > pbLimSrc) goto LFail;
	esi -= ecx;
#else //!SAFETY
	esi = ebx - 3;
#endif //!SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;
	ebx = esi + 4;
	eax = *(ulong*)esi;
	esi_u = eax << 1;
	edx |= esi_u;
	*edi++ = edx;
	goto LBlock7;

L6BitOffByte7:
	esi_u = ((eax >> 9) & 63) + 1;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte7;

L9BitOffByte3:
	esi_u = ((eax >> 6) & 511) + 65;
	ecx += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LCopyByte7;

L12BitOffByte7:
	esi_u = ((eax >> 11) & 4095) + 577;
	ecx += 1;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LCopyByte7;

L20BitOffByte7:
	esi_u = ((eax >> 11) & 1048575) + 4673;
	ecx += 2;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	goto LCopyByte7;

	// Copy Byte 7
LCopyByte7:
#ifdef SAFETY
   if ((edi + ecx) > pbLimDst) goto LFail;
#endif //SAFETY
	esi = edi - esi_u;
#ifdef SAFETY
	if (esi < pvDst) goto LFail;
#endif //SAFETY
	for (; ecx > 0; ecx--) *edi++ = *esi++;

	// Block 7
LBlock7:
	tst = eax & 128;
	if (!tst) goto LLen7_0;
	tst = eax & 256;
	if (!tst) goto LLen7_1;
	tst = eax & 512;
	if (!tst) goto LLen7_2;
	tst = eax & 1024;
	if (!tst) goto LLen7_3;
	tst = eax & 2048;
	if (!tst) goto LLen7_4;
	tst = eax & 4096;
	if (!tst) goto LLen7_5;
	tst = eax & 8192;
	if (!tst) goto LLen7_6;
	tst = eax & 16384;
	if (!tst) goto LLen7_7;
	tst = eax & 32768;
	if (!tst) goto LLen7_8;
	tst = eax & 65536;
	if (!tst) goto LLen7_9;
	tst = eax & 131072;
	if (!tst) goto LLen7_10;
	tst = eax & 262144;
	if (!tst) goto LLen7_11;
	goto LDone;
LLen7_0:
	ecx = 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 1;
	if (!tst) goto LLitSingle0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen7_1:
	ecx = ((eax >> 9) & 1) | 2;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen7_2:
	ecx = ((eax >> 10) & 3) | 4;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen7_3:
	ecx = ((eax >> 11) & 7) | 8;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen7_4:
	ecx = ((eax >> 12) & 15) | 16;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen7_5:
	ecx = ((eax >> 13) & 31) | 32;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen7_6:
	ecx = ((eax >> 14) & 63) | 64;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen7_7:
	ecx = ((eax >> 15) & 127) | 128;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;
LLen7_8:
	ecx = ((eax >> 16) & 255) | 256;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 1;
	if (!tst) goto LLitByte0;
	tst = eax & 2;
	if (!tst) goto L6BitOffByte0;
	tst = eax & 4;
	if (!tst) goto L9BitOffByte0;
	tst = eax & 8;
	if (!tst) goto L12BitOffByte0;
	goto L20BitOffByte0;
LLen7_9:
	ecx = ((eax >> 17) & 511) | 512;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 4;
	if (!tst) goto LLitByte2;
	tst = eax & 8;
	if (!tst) goto L6BitOffByte2;
	tst = eax & 16;
	if (!tst) goto L9BitOffByte2;
	tst = eax & 32;
	if (!tst) goto L12BitOffByte2;
	goto L20BitOffByte2;
LLen7_10:
	ecx = ((eax >> 18) & 1023) | 1024;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 16;
	if (!tst) goto LLitByte4;
	tst = eax & 32;
	if (!tst) goto L6BitOffByte4;
	tst = eax & 64;
	if (!tst) goto L9BitOffByte4;
	tst = eax & 128;
	if (!tst) goto L12BitOffByte4;
	goto L20BitOffByte4;
LLen7_11:
	ecx = ((eax >> 19) & 2047) | 2048;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 64;
	if (!tst) goto LLitByte6;
	tst = eax & 128;
	if (!tst) goto L6BitOffByte6;
	tst = eax & 256;
	if (!tst) goto L9BitOffByte6;
	tst = eax & 512;
	if (!tst) goto L12BitOffByte6;
	goto L20BitOffByte6;

LDone:
	cbTot = edi - pvDst;
// clang-format on
