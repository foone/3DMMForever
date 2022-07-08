// clang-format off
	// Setup
	long cbTot;
	byte *pbLimDst = (byte *)pvDst + cbDst;
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
	if (!tst) goto LLiteral1;
	tst = eax & 2;
	if (!tst) goto L6Bit2;
	tst = eax & 4;
	if (!tst) goto L9Bit3;
	tst = eax & 8;
	if (!tst) goto L12Bit4;
	goto L20Bit4;

	// Literal 1
LLiteral1:
#ifdef SAFETY
	if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock1;

L6Bit2:
	esi_u = (eax >> 2) & 63;
	ecx = 2;
	esi_u += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 1;
	if (!tst) goto LLen1_0;
	tst = eax & 2;
	if (!tst) goto LLen1_1;
	tst = eax & 4;
	if (!tst) goto LLen1_2;
	tst = eax & 8;
	if (!tst) goto LLen1_3;
	tst = eax & 16;
	if (!tst) goto LLen1_4;
	tst = eax & 32;
	if (!tst) goto LLen1_5;
	tst = eax & 64;
	if (!tst) goto LLen1_6;
	tst = eax & 128;
	if (!tst) goto LLen1_7;
	tst = eax & 256;
	if (!tst) goto LLen1_8;
	tst = eax & 512;
	if (!tst) goto LLen1_9;
	tst = eax & 1024;
	if (!tst) goto LLen1_10;
	tst = eax & 2048;
	if (!tst) goto LLen1_11;
	goto LFail;
LLen1_0:
goto LCopyByte1
;LLen1_1:
	edx = (eax >> 2) & 1;
	ecx += 1 + edx;
goto LCopyByte3
;LLen1_2:
	edx = (eax >> 3) & 3;
	ecx += 3 + edx;
goto LCopyByte5
;LLen1_3:
	edx = (eax >> 4) & 7;
	ecx += 7 + edx;
goto LCopyByte7
;LLen1_4:
	edx = (eax >> 5) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen1_5:
	edx = (eax >> 6) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen1_6:
	edx = (eax >> 7) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen1_7:
	edx = (eax >> 8) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen1_8:
	edx = (eax >> 9) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen1_9:
	edx = (eax >> 10) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen1_10:
	edx = (eax >> 11) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen1_11:
	edx = (eax >> 12) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;
L9Bit3:
	esi_u = (eax >> 3) & 511;
	ecx = 2;
	esi_u += 65;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 16;
	if (!tst) goto LLen2_0;
	tst = eax & 32;
	if (!tst) goto LLen2_1;
	tst = eax & 64;
	if (!tst) goto LLen2_2;
	tst = eax & 128;
	if (!tst) goto LLen2_3;
	tst = eax & 256;
	if (!tst) goto LLen2_4;
	tst = eax & 512;
	if (!tst) goto LLen2_5;
	tst = eax & 1024;
	if (!tst) goto LLen2_6;
	tst = eax & 2048;
	if (!tst) goto LLen2_7;
	tst = eax & 4096;
	if (!tst) goto LLen2_8;
	tst = eax & 8192;
	if (!tst) goto LLen2_9;
	tst = eax & 16384;
	if (!tst) goto LLen2_10;
	tst = eax & 32768;
	if (!tst) goto LLen2_11;
	goto LFail;
LLen2_0:
goto LCopyByte5
;LLen2_1:
	edx = (eax >> 6) & 1;
	ecx += 1 + edx;
goto LCopyByte7
;LLen2_2:
	edx = (eax >> 7) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen2_3:
	edx = (eax >> 8) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen2_4:
	edx = (eax >> 9) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen2_5:
	edx = (eax >> 10) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen2_6:
	edx = (eax >> 11) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen2_7:
	edx = (eax >> 12) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen2_8:
	edx = (eax >> 13) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen2_9:
	edx = (eax >> 14) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen2_10:
	edx = (eax >> 15) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;LLen2_11:
	edx = (eax >> 16) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte3
;
L12Bit4:
	esi_u = (eax >> 4) & 4095;
	ecx = 2;
	esi_u += 577;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 1;
	if (!tst) goto LLen3_0;
	tst = eax & 2;
	if (!tst) goto LLen3_1;
	tst = eax & 4;
	if (!tst) goto LLen3_2;
	tst = eax & 8;
	if (!tst) goto LLen3_3;
	tst = eax & 16;
	if (!tst) goto LLen3_4;
	tst = eax & 32;
	if (!tst) goto LLen3_5;
	tst = eax & 64;
	if (!tst) goto LLen3_6;
	tst = eax & 128;
	if (!tst) goto LLen3_7;
	tst = eax & 256;
	if (!tst) goto LLen3_8;
	tst = eax & 512;
	if (!tst) goto LLen3_9;
	tst = eax & 1024;
	if (!tst) goto LLen3_10;
	tst = eax & 2048;
	if (!tst) goto LLen3_11;
	goto LFail;
LLen3_0:
goto LCopyByte1
;LLen3_1:
	edx = (eax >> 2) & 1;
	ecx += 1 + edx;
goto LCopyByte3
;LLen3_2:
	edx = (eax >> 3) & 3;
	ecx += 3 + edx;
goto LCopyByte5
;LLen3_3:
	edx = (eax >> 4) & 7;
	ecx += 7 + edx;
goto LCopyByte7
;LLen3_4:
	edx = (eax >> 5) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen3_5:
	edx = (eax >> 6) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen3_6:
	edx = (eax >> 7) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen3_7:
	edx = (eax >> 8) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen3_8:
	edx = (eax >> 9) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen3_9:
	edx = (eax >> 10) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen3_10:
	edx = (eax >> 11) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen3_11:
	edx = (eax >> 12) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;
L20Bit4:
	esi_u = (eax >> 4) & 1048575;
	ecx = 3;
	if (esi_u == 1048575) goto LDone;
	esi_u += 4673;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 1;
	if (!tst) goto LLen4_0;
	tst = eax & 2;
	if (!tst) goto LLen4_1;
	tst = eax & 4;
	if (!tst) goto LLen4_2;
	tst = eax & 8;
	if (!tst) goto LLen4_3;
	tst = eax & 16;
	if (!tst) goto LLen4_4;
	tst = eax & 32;
	if (!tst) goto LLen4_5;
	tst = eax & 64;
	if (!tst) goto LLen4_6;
	tst = eax & 128;
	if (!tst) goto LLen4_7;
	tst = eax & 256;
	if (!tst) goto LLen4_8;
	tst = eax & 512;
	if (!tst) goto LLen4_9;
	tst = eax & 1024;
	if (!tst) goto LLen4_10;
	tst = eax & 2048;
	if (!tst) goto LLen4_11;
	goto LFail;
LLen4_0:
goto LCopyByte1
;LLen4_1:
	edx = (eax >> 2) & 1;
	ecx += 1 + edx;
goto LCopyByte3
;LLen4_2:
	edx = (eax >> 3) & 3;
	ecx += 3 + edx;
goto LCopyByte5
;LLen4_3:
	edx = (eax >> 4) & 7;
	ecx += 7 + edx;
goto LCopyByte7
;LLen4_4:
	edx = (eax >> 5) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen4_5:
	edx = (eax >> 6) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen4_6:
	edx = (eax >> 7) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen4_7:
	edx = (eax >> 8) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen4_8:
	edx = (eax >> 9) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen4_9:
	edx = (eax >> 10) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen4_10:
	edx = (eax >> 11) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen4_11:
	edx = (eax >> 12) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;
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
	if (!tst) goto LLiteral2;
	tst = eax & 4;
	if (!tst) goto L6Bit3;
	tst = eax & 8;
	if (!tst) goto L9Bit4;
	tst = eax & 16;
	if (!tst) goto L12Bit5;
	goto L20Bit5;

	// Literal 2
LLiteral2:
#ifdef SAFETY
	if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 2;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock2;

L6Bit3:
	esi_u = (eax >> 3) & 63;
	ecx = 2;
	esi_u += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 2;
	if (!tst) goto LLen5_0;
	tst = eax & 4;
	if (!tst) goto LLen5_1;
	tst = eax & 8;
	if (!tst) goto LLen5_2;
	tst = eax & 16;
	if (!tst) goto LLen5_3;
	tst = eax & 32;
	if (!tst) goto LLen5_4;
	tst = eax & 64;
	if (!tst) goto LLen5_5;
	tst = eax & 128;
	if (!tst) goto LLen5_6;
	tst = eax & 256;
	if (!tst) goto LLen5_7;
	tst = eax & 512;
	if (!tst) goto LLen5_8;
	tst = eax & 1024;
	if (!tst) goto LLen5_9;
	tst = eax & 2048;
	if (!tst) goto LLen5_10;
	tst = eax & 4096;
	if (!tst) goto LLen5_11;
	goto LFail;
LLen5_0:
goto LCopyByte2
;LLen5_1:
	edx = (eax >> 3) & 1;
	ecx += 1 + edx;
goto LCopyByte4
;LLen5_2:
	edx = (eax >> 4) & 3;
	ecx += 3 + edx;
goto LCopyByte6
;LLen5_3:
	edx = (eax >> 5) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen5_4:
	edx = (eax >> 6) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen5_5:
	edx = (eax >> 7) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen5_6:
	edx = (eax >> 8) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen5_7:
	edx = (eax >> 9) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen5_8:
	edx = (eax >> 10) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen5_9:
	edx = (eax >> 11) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen5_10:
	edx = (eax >> 12) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen5_11:
	edx = (eax >> 13) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;
L9Bit4:
	esi_u = (eax >> 4) & 511;
	ecx = 2;
	esi_u += 65;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 32;
	if (!tst) goto LLen6_0;
	tst = eax & 64;
	if (!tst) goto LLen6_1;
	tst = eax & 128;
	if (!tst) goto LLen6_2;
	tst = eax & 256;
	if (!tst) goto LLen6_3;
	tst = eax & 512;
	if (!tst) goto LLen6_4;
	tst = eax & 1024;
	if (!tst) goto LLen6_5;
	tst = eax & 2048;
	if (!tst) goto LLen6_6;
	tst = eax & 4096;
	if (!tst) goto LLen6_7;
	tst = eax & 8192;
	if (!tst) goto LLen6_8;
	tst = eax & 16384;
	if (!tst) goto LLen6_9;
	tst = eax & 32768;
	if (!tst) goto LLen6_10;
	tst = eax & 65536;
	if (!tst) goto LLen6_11;
	goto LFail;
LLen6_0:
goto LCopyByte6
;LLen6_1:
	edx = (eax >> 7) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen6_2:
	edx = (eax >> 8) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen6_3:
	edx = (eax >> 9) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen6_4:
	edx = (eax >> 10) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen6_5:
	edx = (eax >> 11) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen6_6:
	edx = (eax >> 12) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen6_7:
	edx = (eax >> 13) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen6_8:
	edx = (eax >> 14) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen6_9:
	edx = (eax >> 15) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen6_10:
	edx = (eax >> 16) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;LLen6_11:
	edx = (eax >> 17) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte4
;
L12Bit5:
	esi_u = (eax >> 5) & 4095;
	ecx = 2;
	esi_u += 577;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 2;
	if (!tst) goto LLen7_0;
	tst = eax & 4;
	if (!tst) goto LLen7_1;
	tst = eax & 8;
	if (!tst) goto LLen7_2;
	tst = eax & 16;
	if (!tst) goto LLen7_3;
	tst = eax & 32;
	if (!tst) goto LLen7_4;
	tst = eax & 64;
	if (!tst) goto LLen7_5;
	tst = eax & 128;
	if (!tst) goto LLen7_6;
	tst = eax & 256;
	if (!tst) goto LLen7_7;
	tst = eax & 512;
	if (!tst) goto LLen7_8;
	tst = eax & 1024;
	if (!tst) goto LLen7_9;
	tst = eax & 2048;
	if (!tst) goto LLen7_10;
	tst = eax & 4096;
	if (!tst) goto LLen7_11;
	goto LFail;
LLen7_0:
goto LCopyByte2
;LLen7_1:
	edx = (eax >> 3) & 1;
	ecx += 1 + edx;
goto LCopyByte4
;LLen7_2:
	edx = (eax >> 4) & 3;
	ecx += 3 + edx;
goto LCopyByte6
;LLen7_3:
	edx = (eax >> 5) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen7_4:
	edx = (eax >> 6) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen7_5:
	edx = (eax >> 7) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen7_6:
	edx = (eax >> 8) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen7_7:
	edx = (eax >> 9) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen7_8:
	edx = (eax >> 10) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen7_9:
	edx = (eax >> 11) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen7_10:
	edx = (eax >> 12) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen7_11:
	edx = (eax >> 13) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;
L20Bit5:
	esi_u = (eax >> 5) & 1048575;
	ecx = 3;
	if (esi_u == 1048575) goto LDone;
	esi_u += 4673;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 2;
	if (!tst) goto LLen8_0;
	tst = eax & 4;
	if (!tst) goto LLen8_1;
	tst = eax & 8;
	if (!tst) goto LLen8_2;
	tst = eax & 16;
	if (!tst) goto LLen8_3;
	tst = eax & 32;
	if (!tst) goto LLen8_4;
	tst = eax & 64;
	if (!tst) goto LLen8_5;
	tst = eax & 128;
	if (!tst) goto LLen8_6;
	tst = eax & 256;
	if (!tst) goto LLen8_7;
	tst = eax & 512;
	if (!tst) goto LLen8_8;
	tst = eax & 1024;
	if (!tst) goto LLen8_9;
	tst = eax & 2048;
	if (!tst) goto LLen8_10;
	tst = eax & 4096;
	if (!tst) goto LLen8_11;
	goto LFail;
LLen8_0:
goto LCopyByte2
;LLen8_1:
	edx = (eax >> 3) & 1;
	ecx += 1 + edx;
goto LCopyByte4
;LLen8_2:
	edx = (eax >> 4) & 3;
	ecx += 3 + edx;
goto LCopyByte6
;LLen8_3:
	edx = (eax >> 5) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen8_4:
	edx = (eax >> 6) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen8_5:
	edx = (eax >> 7) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen8_6:
	edx = (eax >> 8) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen8_7:
	edx = (eax >> 9) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen8_8:
	edx = (eax >> 10) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen8_9:
	edx = (eax >> 11) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen8_10:
	edx = (eax >> 12) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen8_11:
	edx = (eax >> 13) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;
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
	if (!tst) goto LLiteral3;
	tst = eax & 8;
	if (!tst) goto L6Bit4;
	tst = eax & 16;
	if (!tst) goto L9Bit5;
	tst = eax & 32;
	if (!tst) goto L12Bit6;
	goto L20Bit6;

	// Literal 3
LLiteral3:
#ifdef SAFETY
	if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 3;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock3;

L6Bit4:
	esi_u = (eax >> 4) & 63;
	ecx = 2;
	esi_u += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 4;
	if (!tst) goto LLen9_0;
	tst = eax & 8;
	if (!tst) goto LLen9_1;
	tst = eax & 16;
	if (!tst) goto LLen9_2;
	tst = eax & 32;
	if (!tst) goto LLen9_3;
	tst = eax & 64;
	if (!tst) goto LLen9_4;
	tst = eax & 128;
	if (!tst) goto LLen9_5;
	tst = eax & 256;
	if (!tst) goto LLen9_6;
	tst = eax & 512;
	if (!tst) goto LLen9_7;
	tst = eax & 1024;
	if (!tst) goto LLen9_8;
	tst = eax & 2048;
	if (!tst) goto LLen9_9;
	tst = eax & 4096;
	if (!tst) goto LLen9_10;
	tst = eax & 8192;
	if (!tst) goto LLen9_11;
	goto LFail;
LLen9_0:
goto LCopyByte3
;LLen9_1:
	edx = (eax >> 4) & 1;
	ecx += 1 + edx;
goto LCopyByte5
;LLen9_2:
	edx = (eax >> 5) & 3;
	ecx += 3 + edx;
goto LCopyByte7
;LLen9_3:
	edx = (eax >> 6) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen9_4:
	edx = (eax >> 7) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen9_5:
	edx = (eax >> 8) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen9_6:
	edx = (eax >> 9) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen9_7:
	edx = (eax >> 10) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen9_8:
	edx = (eax >> 11) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen9_9:
	edx = (eax >> 12) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen9_10:
	edx = (eax >> 13) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen9_11:
	edx = (eax >> 14) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;
L9Bit5:
	esi_u = (eax >> 5) & 511;
	ecx = 2;
	esi_u += 65;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 64;
	if (!tst) goto LLen10_0;
	tst = eax & 128;
	if (!tst) goto LLen10_1;
	tst = eax & 256;
	if (!tst) goto LLen10_2;
	tst = eax & 512;
	if (!tst) goto LLen10_3;
	tst = eax & 1024;
	if (!tst) goto LLen10_4;
	tst = eax & 2048;
	if (!tst) goto LLen10_5;
	tst = eax & 4096;
	if (!tst) goto LLen10_6;
	tst = eax & 8192;
	if (!tst) goto LLen10_7;
	tst = eax & 16384;
	if (!tst) goto LLen10_8;
	tst = eax & 32768;
	if (!tst) goto LLen10_9;
	tst = eax & 65536;
	if (!tst) goto LLen10_10;
	tst = eax & 131072;
	if (!tst) goto LLen10_11;
	goto LFail;
LLen10_0:
goto LCopyByte7
;LLen10_1:
	edx = (eax >> 8) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen10_2:
	edx = (eax >> 9) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen10_3:
	edx = (eax >> 10) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen10_4:
	edx = (eax >> 11) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen10_5:
	edx = (eax >> 12) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen10_6:
	edx = (eax >> 13) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen10_7:
	edx = (eax >> 14) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen10_8:
	edx = (eax >> 15) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen10_9:
	edx = (eax >> 16) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;LLen10_10:
	edx = (eax >> 17) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte3
;LLen10_11:
	edx = (eax >> 18) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte5
;
L12Bit6:
	esi_u = (eax >> 6) & 4095;
	ecx = 2;
	esi_u += 577;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 4;
	if (!tst) goto LLen11_0;
	tst = eax & 8;
	if (!tst) goto LLen11_1;
	tst = eax & 16;
	if (!tst) goto LLen11_2;
	tst = eax & 32;
	if (!tst) goto LLen11_3;
	tst = eax & 64;
	if (!tst) goto LLen11_4;
	tst = eax & 128;
	if (!tst) goto LLen11_5;
	tst = eax & 256;
	if (!tst) goto LLen11_6;
	tst = eax & 512;
	if (!tst) goto LLen11_7;
	tst = eax & 1024;
	if (!tst) goto LLen11_8;
	tst = eax & 2048;
	if (!tst) goto LLen11_9;
	tst = eax & 4096;
	if (!tst) goto LLen11_10;
	tst = eax & 8192;
	if (!tst) goto LLen11_11;
	goto LFail;
LLen11_0:
goto LCopyByte3
;LLen11_1:
	edx = (eax >> 4) & 1;
	ecx += 1 + edx;
goto LCopyByte5
;LLen11_2:
	edx = (eax >> 5) & 3;
	ecx += 3 + edx;
goto LCopyByte7
;LLen11_3:
	edx = (eax >> 6) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen11_4:
	edx = (eax >> 7) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen11_5:
	edx = (eax >> 8) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen11_6:
	edx = (eax >> 9) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen11_7:
	edx = (eax >> 10) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen11_8:
	edx = (eax >> 11) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen11_9:
	edx = (eax >> 12) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen11_10:
	edx = (eax >> 13) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen11_11:
	edx = (eax >> 14) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;
L20Bit6:
	esi_u = (eax >> 6) & 1048575;
	ecx = 3;
	if (esi_u == 1048575) goto LDone;
	esi_u += 4673;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 4;
	if (!tst) goto LLen12_0;
	tst = eax & 8;
	if (!tst) goto LLen12_1;
	tst = eax & 16;
	if (!tst) goto LLen12_2;
	tst = eax & 32;
	if (!tst) goto LLen12_3;
	tst = eax & 64;
	if (!tst) goto LLen12_4;
	tst = eax & 128;
	if (!tst) goto LLen12_5;
	tst = eax & 256;
	if (!tst) goto LLen12_6;
	tst = eax & 512;
	if (!tst) goto LLen12_7;
	tst = eax & 1024;
	if (!tst) goto LLen12_8;
	tst = eax & 2048;
	if (!tst) goto LLen12_9;
	tst = eax & 4096;
	if (!tst) goto LLen12_10;
	tst = eax & 8192;
	if (!tst) goto LLen12_11;
	goto LFail;
LLen12_0:
goto LCopyByte3
;LLen12_1:
	edx = (eax >> 4) & 1;
	ecx += 1 + edx;
goto LCopyByte5
;LLen12_2:
	edx = (eax >> 5) & 3;
	ecx += 3 + edx;
goto LCopyByte7
;LLen12_3:
	edx = (eax >> 6) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen12_4:
	edx = (eax >> 7) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen12_5:
	edx = (eax >> 8) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen12_6:
	edx = (eax >> 9) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen12_7:
	edx = (eax >> 10) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen12_8:
	edx = (eax >> 11) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen12_9:
	edx = (eax >> 12) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen12_10:
	edx = (eax >> 13) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen12_11:
	edx = (eax >> 14) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;
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
	if (!tst) goto LLiteral4;
	tst = eax & 16;
	if (!tst) goto L6Bit5;
	tst = eax & 32;
	if (!tst) goto L9Bit6;
	tst = eax & 64;
	if (!tst) goto L12Bit7;
	goto L20Bit7;

	// Literal 4
LLiteral4:
#ifdef SAFETY
	if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 4;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock4;

L6Bit5:
	esi_u = (eax >> 5) & 63;
	ecx = 2;
	esi_u += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 8;
	if (!tst) goto LLen13_0;
	tst = eax & 16;
	if (!tst) goto LLen13_1;
	tst = eax & 32;
	if (!tst) goto LLen13_2;
	tst = eax & 64;
	if (!tst) goto LLen13_3;
	tst = eax & 128;
	if (!tst) goto LLen13_4;
	tst = eax & 256;
	if (!tst) goto LLen13_5;
	tst = eax & 512;
	if (!tst) goto LLen13_6;
	tst = eax & 1024;
	if (!tst) goto LLen13_7;
	tst = eax & 2048;
	if (!tst) goto LLen13_8;
	tst = eax & 4096;
	if (!tst) goto LLen13_9;
	tst = eax & 8192;
	if (!tst) goto LLen13_10;
	tst = eax & 16384;
	if (!tst) goto LLen13_11;
	goto LFail;
LLen13_0:
goto LCopyByte4
;LLen13_1:
	edx = (eax >> 5) & 1;
	ecx += 1 + edx;
goto LCopyByte6
;LLen13_2:
	edx = (eax >> 6) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen13_3:
	edx = (eax >> 7) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen13_4:
	edx = (eax >> 8) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen13_5:
	edx = (eax >> 9) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen13_6:
	edx = (eax >> 10) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen13_7:
	edx = (eax >> 11) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen13_8:
	edx = (eax >> 12) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen13_9:
	edx = (eax >> 13) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen13_10:
	edx = (eax >> 14) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen13_11:
	edx = (eax >> 15) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;
L9Bit6:
	esi_u = (eax >> 6) & 511;
	ecx = 2;
	esi_u += 65;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 128;
	if (!tst) goto LLen14_0;
	tst = eax & 256;
	if (!tst) goto LLen14_1;
	tst = eax & 512;
	if (!tst) goto LLen14_2;
	tst = eax & 1024;
	if (!tst) goto LLen14_3;
	tst = eax & 2048;
	if (!tst) goto LLen14_4;
	tst = eax & 4096;
	if (!tst) goto LLen14_5;
	tst = eax & 8192;
	if (!tst) goto LLen14_6;
	tst = eax & 16384;
	if (!tst) goto LLen14_7;
	tst = eax & 32768;
	if (!tst) goto LLen14_8;
	tst = eax & 65536;
	if (!tst) goto LLen14_9;
	tst = eax & 131072;
	if (!tst) goto LLen14_10;
	tst = eax & 262144;
	if (!tst) goto LLen14_11;
	goto LFail;
LLen14_0:
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen14_1:
	edx = (eax >> 9) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen14_2:
	edx = (eax >> 10) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen14_3:
	edx = (eax >> 11) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen14_4:
	edx = (eax >> 12) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen14_5:
	edx = (eax >> 13) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen14_6:
	edx = (eax >> 14) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen14_7:
	edx = (eax >> 15) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen14_8:
	edx = (eax >> 16) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen14_9:
	edx = (eax >> 17) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;LLen14_10:
	edx = (eax >> 18) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte4
;LLen14_11:
	edx = (eax >> 19) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte6
;
L12Bit7:
	esi_u = (eax >> 7) & 4095;
	ecx = 2;
	esi_u += 577;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 8;
	if (!tst) goto LLen15_0;
	tst = eax & 16;
	if (!tst) goto LLen15_1;
	tst = eax & 32;
	if (!tst) goto LLen15_2;
	tst = eax & 64;
	if (!tst) goto LLen15_3;
	tst = eax & 128;
	if (!tst) goto LLen15_4;
	tst = eax & 256;
	if (!tst) goto LLen15_5;
	tst = eax & 512;
	if (!tst) goto LLen15_6;
	tst = eax & 1024;
	if (!tst) goto LLen15_7;
	tst = eax & 2048;
	if (!tst) goto LLen15_8;
	tst = eax & 4096;
	if (!tst) goto LLen15_9;
	tst = eax & 8192;
	if (!tst) goto LLen15_10;
	tst = eax & 16384;
	if (!tst) goto LLen15_11;
	goto LFail;
LLen15_0:
goto LCopyByte4
;LLen15_1:
	edx = (eax >> 5) & 1;
	ecx += 1 + edx;
goto LCopyByte6
;LLen15_2:
	edx = (eax >> 6) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen15_3:
	edx = (eax >> 7) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen15_4:
	edx = (eax >> 8) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen15_5:
	edx = (eax >> 9) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen15_6:
	edx = (eax >> 10) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen15_7:
	edx = (eax >> 11) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen15_8:
	edx = (eax >> 12) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen15_9:
	edx = (eax >> 13) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen15_10:
	edx = (eax >> 14) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen15_11:
	edx = (eax >> 15) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;
L20Bit7:
	esi_u = (eax >> 7) & 1048575;
	ecx = 3;
	if (esi_u == 1048575) goto LDone;
	esi_u += 4673;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 8;
	if (!tst) goto LLen16_0;
	tst = eax & 16;
	if (!tst) goto LLen16_1;
	tst = eax & 32;
	if (!tst) goto LLen16_2;
	tst = eax & 64;
	if (!tst) goto LLen16_3;
	tst = eax & 128;
	if (!tst) goto LLen16_4;
	tst = eax & 256;
	if (!tst) goto LLen16_5;
	tst = eax & 512;
	if (!tst) goto LLen16_6;
	tst = eax & 1024;
	if (!tst) goto LLen16_7;
	tst = eax & 2048;
	if (!tst) goto LLen16_8;
	tst = eax & 4096;
	if (!tst) goto LLen16_9;
	tst = eax & 8192;
	if (!tst) goto LLen16_10;
	tst = eax & 16384;
	if (!tst) goto LLen16_11;
	goto LFail;
LLen16_0:
goto LCopyByte4
;LLen16_1:
	edx = (eax >> 5) & 1;
	ecx += 1 + edx;
goto LCopyByte6
;LLen16_2:
	edx = (eax >> 6) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen16_3:
	edx = (eax >> 7) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen16_4:
	edx = (eax >> 8) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen16_5:
	edx = (eax >> 9) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen16_6:
	edx = (eax >> 10) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen16_7:
	edx = (eax >> 11) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen16_8:
	edx = (eax >> 12) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen16_9:
	edx = (eax >> 13) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen16_10:
	edx = (eax >> 14) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen16_11:
	edx = (eax >> 15) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;
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
	if (!tst) goto LLiteral5;
	tst = eax & 32;
	if (!tst) goto L6Bit6;
	tst = eax & 64;
	if (!tst) goto L9Bit7;
	tst = eax & 128;
	if (!tst) goto L12Bit8;
	goto L20Bit8;

	// Literal 5
LLiteral5:
#ifdef SAFETY
	if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 5;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock5;

L6Bit6:
	esi_u = (eax >> 6) & 63;
	ecx = 2;
	esi_u += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 16;
	if (!tst) goto LLen17_0;
	tst = eax & 32;
	if (!tst) goto LLen17_1;
	tst = eax & 64;
	if (!tst) goto LLen17_2;
	tst = eax & 128;
	if (!tst) goto LLen17_3;
	tst = eax & 256;
	if (!tst) goto LLen17_4;
	tst = eax & 512;
	if (!tst) goto LLen17_5;
	tst = eax & 1024;
	if (!tst) goto LLen17_6;
	tst = eax & 2048;
	if (!tst) goto LLen17_7;
	tst = eax & 4096;
	if (!tst) goto LLen17_8;
	tst = eax & 8192;
	if (!tst) goto LLen17_9;
	tst = eax & 16384;
	if (!tst) goto LLen17_10;
	tst = eax & 32768;
	if (!tst) goto LLen17_11;
	goto LFail;
LLen17_0:
goto LCopyByte5
;LLen17_1:
	edx = (eax >> 6) & 1;
	ecx += 1 + edx;
goto LCopyByte7
;LLen17_2:
	edx = (eax >> 7) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen17_3:
	edx = (eax >> 8) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen17_4:
	edx = (eax >> 9) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen17_5:
	edx = (eax >> 10) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen17_6:
	edx = (eax >> 11) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen17_7:
	edx = (eax >> 12) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen17_8:
	edx = (eax >> 13) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen17_9:
	edx = (eax >> 14) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen17_10:
	edx = (eax >> 15) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;LLen17_11:
	edx = (eax >> 16) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte3
;
L9Bit7:
	esi_u = (eax >> 7) & 511;
	ecx = 2;
	esi_u += 65;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 1;
	if (!tst) goto LLen18_0;
	tst = eax & 2;
	if (!tst) goto LLen18_1;
	tst = eax & 4;
	if (!tst) goto LLen18_2;
	tst = eax & 8;
	if (!tst) goto LLen18_3;
	tst = eax & 16;
	if (!tst) goto LLen18_4;
	tst = eax & 32;
	if (!tst) goto LLen18_5;
	tst = eax & 64;
	if (!tst) goto LLen18_6;
	tst = eax & 128;
	if (!tst) goto LLen18_7;
	tst = eax & 256;
	if (!tst) goto LLen18_8;
	tst = eax & 512;
	if (!tst) goto LLen18_9;
	tst = eax & 1024;
	if (!tst) goto LLen18_10;
	tst = eax & 2048;
	if (!tst) goto LLen18_11;
	goto LFail;
LLen18_0:
goto LCopyByte1
;LLen18_1:
	edx = (eax >> 2) & 1;
	ecx += 1 + edx;
goto LCopyByte3
;LLen18_2:
	edx = (eax >> 3) & 3;
	ecx += 3 + edx;
goto LCopyByte5
;LLen18_3:
	edx = (eax >> 4) & 7;
	ecx += 7 + edx;
goto LCopyByte7
;LLen18_4:
	edx = (eax >> 5) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen18_5:
	edx = (eax >> 6) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen18_6:
	edx = (eax >> 7) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen18_7:
	edx = (eax >> 8) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen18_8:
	edx = (eax >> 9) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen18_9:
	edx = (eax >> 10) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen18_10:
	edx = (eax >> 11) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen18_11:
	edx = (eax >> 12) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;
L12Bit8:
	esi_u = (eax >> 8) & 4095;
	ecx = 2;
	esi_u += 577;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 16;
	if (!tst) goto LLen19_0;
	tst = eax & 32;
	if (!tst) goto LLen19_1;
	tst = eax & 64;
	if (!tst) goto LLen19_2;
	tst = eax & 128;
	if (!tst) goto LLen19_3;
	tst = eax & 256;
	if (!tst) goto LLen19_4;
	tst = eax & 512;
	if (!tst) goto LLen19_5;
	tst = eax & 1024;
	if (!tst) goto LLen19_6;
	tst = eax & 2048;
	if (!tst) goto LLen19_7;
	tst = eax & 4096;
	if (!tst) goto LLen19_8;
	tst = eax & 8192;
	if (!tst) goto LLen19_9;
	tst = eax & 16384;
	if (!tst) goto LLen19_10;
	tst = eax & 32768;
	if (!tst) goto LLen19_11;
	goto LFail;
LLen19_0:
goto LCopyByte5
;LLen19_1:
	edx = (eax >> 6) & 1;
	ecx += 1 + edx;
goto LCopyByte7
;LLen19_2:
	edx = (eax >> 7) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen19_3:
	edx = (eax >> 8) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen19_4:
	edx = (eax >> 9) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen19_5:
	edx = (eax >> 10) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen19_6:
	edx = (eax >> 11) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen19_7:
	edx = (eax >> 12) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen19_8:
	edx = (eax >> 13) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen19_9:
	edx = (eax >> 14) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen19_10:
	edx = (eax >> 15) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;LLen19_11:
	edx = (eax >> 16) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte3
;
L20Bit8:
	esi_u = (eax >> 8) & 1048575;
	ecx = 3;
	if (esi_u == 1048575) goto LDone;
	esi_u += 4673;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 16;
	if (!tst) goto LLen20_0;
	tst = eax & 32;
	if (!tst) goto LLen20_1;
	tst = eax & 64;
	if (!tst) goto LLen20_2;
	tst = eax & 128;
	if (!tst) goto LLen20_3;
	tst = eax & 256;
	if (!tst) goto LLen20_4;
	tst = eax & 512;
	if (!tst) goto LLen20_5;
	tst = eax & 1024;
	if (!tst) goto LLen20_6;
	tst = eax & 2048;
	if (!tst) goto LLen20_7;
	tst = eax & 4096;
	if (!tst) goto LLen20_8;
	tst = eax & 8192;
	if (!tst) goto LLen20_9;
	tst = eax & 16384;
	if (!tst) goto LLen20_10;
	tst = eax & 32768;
	if (!tst) goto LLen20_11;
	goto LFail;
LLen20_0:
goto LCopyByte5
;LLen20_1:
	edx = (eax >> 6) & 1;
	ecx += 1 + edx;
goto LCopyByte7
;LLen20_2:
	edx = (eax >> 7) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen20_3:
	edx = (eax >> 8) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen20_4:
	edx = (eax >> 9) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen20_5:
	edx = (eax >> 10) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen20_6:
	edx = (eax >> 11) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen20_7:
	edx = (eax >> 12) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen20_8:
	edx = (eax >> 13) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen20_9:
	edx = (eax >> 14) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen20_10:
	edx = (eax >> 15) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;LLen20_11:
	edx = (eax >> 16) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte3
;
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
	if (!tst) goto LLiteral6;
	tst = eax & 64;
	if (!tst) goto L6Bit7;
	tst = eax & 128;
	if (!tst) goto L9Bit8;
	tst = eax & 256;
	if (!tst) goto L12Bit9;
	goto L20Bit9;

	// Literal 6
LLiteral6:
#ifdef SAFETY
	if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 6;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock6;

L6Bit7:
	esi_u = (eax >> 7) & 63;
	ecx = 2;
	esi_u += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 32;
	if (!tst) goto LLen21_0;
	tst = eax & 64;
	if (!tst) goto LLen21_1;
	tst = eax & 128;
	if (!tst) goto LLen21_2;
	tst = eax & 256;
	if (!tst) goto LLen21_3;
	tst = eax & 512;
	if (!tst) goto LLen21_4;
	tst = eax & 1024;
	if (!tst) goto LLen21_5;
	tst = eax & 2048;
	if (!tst) goto LLen21_6;
	tst = eax & 4096;
	if (!tst) goto LLen21_7;
	tst = eax & 8192;
	if (!tst) goto LLen21_8;
	tst = eax & 16384;
	if (!tst) goto LLen21_9;
	tst = eax & 32768;
	if (!tst) goto LLen21_10;
	tst = eax & 65536;
	if (!tst) goto LLen21_11;
	goto LFail;
LLen21_0:
goto LCopyByte6
;LLen21_1:
	edx = (eax >> 7) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen21_2:
	edx = (eax >> 8) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen21_3:
	edx = (eax >> 9) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen21_4:
	edx = (eax >> 10) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen21_5:
	edx = (eax >> 11) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen21_6:
	edx = (eax >> 12) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen21_7:
	edx = (eax >> 13) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen21_8:
	edx = (eax >> 14) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen21_9:
	edx = (eax >> 15) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen21_10:
	edx = (eax >> 16) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;LLen21_11:
	edx = (eax >> 17) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte4
;
L9Bit8:
	esi_u = (eax >> 8) & 511;
	ecx = 2;
	esi_u += 65;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 2;
	if (!tst) goto LLen22_0;
	tst = eax & 4;
	if (!tst) goto LLen22_1;
	tst = eax & 8;
	if (!tst) goto LLen22_2;
	tst = eax & 16;
	if (!tst) goto LLen22_3;
	tst = eax & 32;
	if (!tst) goto LLen22_4;
	tst = eax & 64;
	if (!tst) goto LLen22_5;
	tst = eax & 128;
	if (!tst) goto LLen22_6;
	tst = eax & 256;
	if (!tst) goto LLen22_7;
	tst = eax & 512;
	if (!tst) goto LLen22_8;
	tst = eax & 1024;
	if (!tst) goto LLen22_9;
	tst = eax & 2048;
	if (!tst) goto LLen22_10;
	tst = eax & 4096;
	if (!tst) goto LLen22_11;
	goto LFail;
LLen22_0:
goto LCopyByte2
;LLen22_1:
	edx = (eax >> 3) & 1;
	ecx += 1 + edx;
goto LCopyByte4
;LLen22_2:
	edx = (eax >> 4) & 3;
	ecx += 3 + edx;
goto LCopyByte6
;LLen22_3:
	edx = (eax >> 5) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen22_4:
	edx = (eax >> 6) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen22_5:
	edx = (eax >> 7) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen22_6:
	edx = (eax >> 8) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen22_7:
	edx = (eax >> 9) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen22_8:
	edx = (eax >> 10) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen22_9:
	edx = (eax >> 11) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen22_10:
	edx = (eax >> 12) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen22_11:
	edx = (eax >> 13) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;
L12Bit9:
	esi_u = (eax >> 9) & 4095;
	ecx = 2;
	esi_u += 577;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 32;
	if (!tst) goto LLen23_0;
	tst = eax & 64;
	if (!tst) goto LLen23_1;
	tst = eax & 128;
	if (!tst) goto LLen23_2;
	tst = eax & 256;
	if (!tst) goto LLen23_3;
	tst = eax & 512;
	if (!tst) goto LLen23_4;
	tst = eax & 1024;
	if (!tst) goto LLen23_5;
	tst = eax & 2048;
	if (!tst) goto LLen23_6;
	tst = eax & 4096;
	if (!tst) goto LLen23_7;
	tst = eax & 8192;
	if (!tst) goto LLen23_8;
	tst = eax & 16384;
	if (!tst) goto LLen23_9;
	tst = eax & 32768;
	if (!tst) goto LLen23_10;
	tst = eax & 65536;
	if (!tst) goto LLen23_11;
	goto LFail;
LLen23_0:
goto LCopyByte6
;LLen23_1:
	edx = (eax >> 7) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen23_2:
	edx = (eax >> 8) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen23_3:
	edx = (eax >> 9) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen23_4:
	edx = (eax >> 10) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen23_5:
	edx = (eax >> 11) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen23_6:
	edx = (eax >> 12) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen23_7:
	edx = (eax >> 13) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen23_8:
	edx = (eax >> 14) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen23_9:
	edx = (eax >> 15) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen23_10:
	edx = (eax >> 16) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;LLen23_11:
	edx = (eax >> 17) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte4
;
L20Bit9:
	esi_u = (eax >> 9) & 1048575;
	ecx = 3;
	if (esi_u == 1048575) goto LDone;
	esi_u += 4673;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 32;
	if (!tst) goto LLen24_0;
	tst = eax & 64;
	if (!tst) goto LLen24_1;
	tst = eax & 128;
	if (!tst) goto LLen24_2;
	tst = eax & 256;
	if (!tst) goto LLen24_3;
	tst = eax & 512;
	if (!tst) goto LLen24_4;
	tst = eax & 1024;
	if (!tst) goto LLen24_5;
	tst = eax & 2048;
	if (!tst) goto LLen24_6;
	tst = eax & 4096;
	if (!tst) goto LLen24_7;
	tst = eax & 8192;
	if (!tst) goto LLen24_8;
	tst = eax & 16384;
	if (!tst) goto LLen24_9;
	tst = eax & 32768;
	if (!tst) goto LLen24_10;
	tst = eax & 65536;
	if (!tst) goto LLen24_11;
	goto LFail;
LLen24_0:
goto LCopyByte6
;LLen24_1:
	edx = (eax >> 7) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen24_2:
	edx = (eax >> 8) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen24_3:
	edx = (eax >> 9) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen24_4:
	edx = (eax >> 10) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen24_5:
	edx = (eax >> 11) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen24_6:
	edx = (eax >> 12) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen24_7:
	edx = (eax >> 13) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen24_8:
	edx = (eax >> 14) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen24_9:
	edx = (eax >> 15) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen24_10:
	edx = (eax >> 16) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;LLen24_11:
	edx = (eax >> 17) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte4
;
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
	if (!tst) goto LLiteral7;
	tst = eax & 128;
	if (!tst) goto L6Bit8;
	tst = eax & 256;
	if (!tst) goto L9Bit9;
	tst = eax & 512;
	if (!tst) goto L12Bit10;
	goto L20Bit10;

	// Literal 7
LLiteral7:
#ifdef SAFETY
	if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 7;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	goto LBlock7;

L6Bit8:
	esi_u = (eax >> 8) & 63;
	ecx = 2;
	esi_u += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 64;
	if (!tst) goto LLen25_0;
	tst = eax & 128;
	if (!tst) goto LLen25_1;
	tst = eax & 256;
	if (!tst) goto LLen25_2;
	tst = eax & 512;
	if (!tst) goto LLen25_3;
	tst = eax & 1024;
	if (!tst) goto LLen25_4;
	tst = eax & 2048;
	if (!tst) goto LLen25_5;
	tst = eax & 4096;
	if (!tst) goto LLen25_6;
	tst = eax & 8192;
	if (!tst) goto LLen25_7;
	tst = eax & 16384;
	if (!tst) goto LLen25_8;
	tst = eax & 32768;
	if (!tst) goto LLen25_9;
	tst = eax & 65536;
	if (!tst) goto LLen25_10;
	tst = eax & 131072;
	if (!tst) goto LLen25_11;
	goto LFail;
LLen25_0:
goto LCopyByte7
;LLen25_1:
	edx = (eax >> 8) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen25_2:
	edx = (eax >> 9) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen25_3:
	edx = (eax >> 10) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen25_4:
	edx = (eax >> 11) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen25_5:
	edx = (eax >> 12) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen25_6:
	edx = (eax >> 13) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen25_7:
	edx = (eax >> 14) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen25_8:
	edx = (eax >> 15) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen25_9:
	edx = (eax >> 16) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;LLen25_10:
	edx = (eax >> 17) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte3
;LLen25_11:
	edx = (eax >> 18) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte5
;
L9Bit9:
	esi_u = (eax >> 9) & 511;
	ecx = 2;
	esi_u += 65;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 4;
	if (!tst) goto LLen26_0;
	tst = eax & 8;
	if (!tst) goto LLen26_1;
	tst = eax & 16;
	if (!tst) goto LLen26_2;
	tst = eax & 32;
	if (!tst) goto LLen26_3;
	tst = eax & 64;
	if (!tst) goto LLen26_4;
	tst = eax & 128;
	if (!tst) goto LLen26_5;
	tst = eax & 256;
	if (!tst) goto LLen26_6;
	tst = eax & 512;
	if (!tst) goto LLen26_7;
	tst = eax & 1024;
	if (!tst) goto LLen26_8;
	tst = eax & 2048;
	if (!tst) goto LLen26_9;
	tst = eax & 4096;
	if (!tst) goto LLen26_10;
	tst = eax & 8192;
	if (!tst) goto LLen26_11;
	goto LFail;
LLen26_0:
goto LCopyByte3
;LLen26_1:
	edx = (eax >> 4) & 1;
	ecx += 1 + edx;
goto LCopyByte5
;LLen26_2:
	edx = (eax >> 5) & 3;
	ecx += 3 + edx;
goto LCopyByte7
;LLen26_3:
	edx = (eax >> 6) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen26_4:
	edx = (eax >> 7) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen26_5:
	edx = (eax >> 8) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen26_6:
	edx = (eax >> 9) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen26_7:
	edx = (eax >> 10) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen26_8:
	edx = (eax >> 11) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen26_9:
	edx = (eax >> 12) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen26_10:
	edx = (eax >> 13) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen26_11:
	edx = (eax >> 14) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;
L12Bit10:
	esi_u = (eax >> 10) & 4095;
	ecx = 2;
	esi_u += 577;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 64;
	if (!tst) goto LLen27_0;
	tst = eax & 128;
	if (!tst) goto LLen27_1;
	tst = eax & 256;
	if (!tst) goto LLen27_2;
	tst = eax & 512;
	if (!tst) goto LLen27_3;
	tst = eax & 1024;
	if (!tst) goto LLen27_4;
	tst = eax & 2048;
	if (!tst) goto LLen27_5;
	tst = eax & 4096;
	if (!tst) goto LLen27_6;
	tst = eax & 8192;
	if (!tst) goto LLen27_7;
	tst = eax & 16384;
	if (!tst) goto LLen27_8;
	tst = eax & 32768;
	if (!tst) goto LLen27_9;
	tst = eax & 65536;
	if (!tst) goto LLen27_10;
	tst = eax & 131072;
	if (!tst) goto LLen27_11;
	goto LFail;
LLen27_0:
goto LCopyByte7
;LLen27_1:
	edx = (eax >> 8) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen27_2:
	edx = (eax >> 9) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen27_3:
	edx = (eax >> 10) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen27_4:
	edx = (eax >> 11) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen27_5:
	edx = (eax >> 12) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen27_6:
	edx = (eax >> 13) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen27_7:
	edx = (eax >> 14) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen27_8:
	edx = (eax >> 15) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen27_9:
	edx = (eax >> 16) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;LLen27_10:
	edx = (eax >> 17) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte3
;LLen27_11:
	edx = (eax >> 18) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte5
;
L20Bit10:
	esi_u = (eax >> 10) & 1048575;
	ecx = 3;
	if (esi_u == 1048575) goto LDone;
	esi_u += 4673;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 64;
	if (!tst) goto LLen28_0;
	tst = eax & 128;
	if (!tst) goto LLen28_1;
	tst = eax & 256;
	if (!tst) goto LLen28_2;
	tst = eax & 512;
	if (!tst) goto LLen28_3;
	tst = eax & 1024;
	if (!tst) goto LLen28_4;
	tst = eax & 2048;
	if (!tst) goto LLen28_5;
	tst = eax & 4096;
	if (!tst) goto LLen28_6;
	tst = eax & 8192;
	if (!tst) goto LLen28_7;
	tst = eax & 16384;
	if (!tst) goto LLen28_8;
	tst = eax & 32768;
	if (!tst) goto LLen28_9;
	tst = eax & 65536;
	if (!tst) goto LLen28_10;
	tst = eax & 131072;
	if (!tst) goto LLen28_11;
	goto LFail;
LLen28_0:
goto LCopyByte7
;LLen28_1:
	edx = (eax >> 8) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte1
;LLen28_2:
	edx = (eax >> 9) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte3
;LLen28_3:
	edx = (eax >> 10) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte5
;LLen28_4:
	edx = (eax >> 11) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte7
;LLen28_5:
	edx = (eax >> 12) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte1
;LLen28_6:
	edx = (eax >> 13) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte3
;LLen28_7:
	edx = (eax >> 14) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte5
;LLen28_8:
	edx = (eax >> 15) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte7
;LLen28_9:
	edx = (eax >> 16) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte1
;LLen28_10:
	edx = (eax >> 17) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte3
;LLen28_11:
	edx = (eax >> 18) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte5
;
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
	if (!tst) goto LLiteral8;
	tst = eax & 256;
	if (!tst) goto L6Bit9;
	tst = eax & 512;
	if (!tst) goto L9Bit10;
	tst = eax & 1024;
	if (!tst) goto L12Bit11;
	goto L20Bit11;

	// Literal 8
LLiteral8:
#ifdef SAFETY
	if (edi >= pbLimDst) goto LFail;
#endif //SAFETY
	*edi++ = eax >> 8;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	goto LBlock0;

L6Bit9:
	esi_u = (eax >> 9) & 63;
	ecx = 2;
	esi_u += 1;
	eax = *(ulong*)(ebx - 3);
	ebx++;
	tst = eax & 128;
	if (!tst) goto LLen29_0;
	tst = eax & 256;
	if (!tst) goto LLen29_1;
	tst = eax & 512;
	if (!tst) goto LLen29_2;
	tst = eax & 1024;
	if (!tst) goto LLen29_3;
	tst = eax & 2048;
	if (!tst) goto LLen29_4;
	tst = eax & 4096;
	if (!tst) goto LLen29_5;
	tst = eax & 8192;
	if (!tst) goto LLen29_6;
	tst = eax & 16384;
	if (!tst) goto LLen29_7;
	tst = eax & 32768;
	if (!tst) goto LLen29_8;
	tst = eax & 65536;
	if (!tst) goto LLen29_9;
	tst = eax & 131072;
	if (!tst) goto LLen29_10;
	tst = eax & 262144;
	if (!tst) goto LLen29_11;
	goto LFail;
LLen29_0:
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen29_1:
	edx = (eax >> 9) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen29_2:
	edx = (eax >> 10) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen29_3:
	edx = (eax >> 11) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen29_4:
	edx = (eax >> 12) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen29_5:
	edx = (eax >> 13) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen29_6:
	edx = (eax >> 14) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen29_7:
	edx = (eax >> 15) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen29_8:
	edx = (eax >> 16) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen29_9:
	edx = (eax >> 17) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;LLen29_10:
	edx = (eax >> 18) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte4
;LLen29_11:
	edx = (eax >> 19) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte6
;
L9Bit10:
	esi_u = (eax >> 10) & 511;
	ecx = 2;
	esi_u += 65;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 8;
	if (!tst) goto LLen30_0;
	tst = eax & 16;
	if (!tst) goto LLen30_1;
	tst = eax & 32;
	if (!tst) goto LLen30_2;
	tst = eax & 64;
	if (!tst) goto LLen30_3;
	tst = eax & 128;
	if (!tst) goto LLen30_4;
	tst = eax & 256;
	if (!tst) goto LLen30_5;
	tst = eax & 512;
	if (!tst) goto LLen30_6;
	tst = eax & 1024;
	if (!tst) goto LLen30_7;
	tst = eax & 2048;
	if (!tst) goto LLen30_8;
	tst = eax & 4096;
	if (!tst) goto LLen30_9;
	tst = eax & 8192;
	if (!tst) goto LLen30_10;
	tst = eax & 16384;
	if (!tst) goto LLen30_11;
	goto LFail;
LLen30_0:
goto LCopyByte4
;LLen30_1:
	edx = (eax >> 5) & 1;
	ecx += 1 + edx;
goto LCopyByte6
;LLen30_2:
	edx = (eax >> 6) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen30_3:
	edx = (eax >> 7) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen30_4:
	edx = (eax >> 8) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen30_5:
	edx = (eax >> 9) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen30_6:
	edx = (eax >> 10) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen30_7:
	edx = (eax >> 11) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen30_8:
	edx = (eax >> 12) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen30_9:
	edx = (eax >> 13) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen30_10:
	edx = (eax >> 14) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen30_11:
	edx = (eax >> 15) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;
L12Bit11:
	esi_u = (eax >> 11) & 4095;
	ecx = 2;
	esi_u += 577;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
	tst = eax & 128;
	if (!tst) goto LLen31_0;
	tst = eax & 256;
	if (!tst) goto LLen31_1;
	tst = eax & 512;
	if (!tst) goto LLen31_2;
	tst = eax & 1024;
	if (!tst) goto LLen31_3;
	tst = eax & 2048;
	if (!tst) goto LLen31_4;
	tst = eax & 4096;
	if (!tst) goto LLen31_5;
	tst = eax & 8192;
	if (!tst) goto LLen31_6;
	tst = eax & 16384;
	if (!tst) goto LLen31_7;
	tst = eax & 32768;
	if (!tst) goto LLen31_8;
	tst = eax & 65536;
	if (!tst) goto LLen31_9;
	tst = eax & 131072;
	if (!tst) goto LLen31_10;
	tst = eax & 262144;
	if (!tst) goto LLen31_11;
	goto LFail;
LLen31_0:
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen31_1:
	edx = (eax >> 9) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen31_2:
	edx = (eax >> 10) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen31_3:
	edx = (eax >> 11) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen31_4:
	edx = (eax >> 12) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen31_5:
	edx = (eax >> 13) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen31_6:
	edx = (eax >> 14) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen31_7:
	edx = (eax >> 15) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen31_8:
	edx = (eax >> 16) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen31_9:
	edx = (eax >> 17) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;LLen31_10:
	edx = (eax >> 18) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte4
;LLen31_11:
	edx = (eax >> 19) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte6
;
L20Bit11:
	esi_u = (eax >> 11) & 1048575;
	ecx = 3;
	if (esi_u == 1048575) goto LDone;
	esi_u += 4673;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
	tst = eax & 128;
	if (!tst) goto LLen32_0;
	tst = eax & 256;
	if (!tst) goto LLen32_1;
	tst = eax & 512;
	if (!tst) goto LLen32_2;
	tst = eax & 1024;
	if (!tst) goto LLen32_3;
	tst = eax & 2048;
	if (!tst) goto LLen32_4;
	tst = eax & 4096;
	if (!tst) goto LLen32_5;
	tst = eax & 8192;
	if (!tst) goto LLen32_6;
	tst = eax & 16384;
	if (!tst) goto LLen32_7;
	tst = eax & 32768;
	if (!tst) goto LLen32_8;
	tst = eax & 65536;
	if (!tst) goto LLen32_9;
	tst = eax & 131072;
	if (!tst) goto LLen32_10;
	tst = eax & 262144;
	if (!tst) goto LLen32_11;
	goto LFail;
LLen32_0:
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte0
;LLen32_1:
	edx = (eax >> 9) & 1;
	ecx += 1 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte2
;LLen32_2:
	edx = (eax >> 10) & 3;
	ecx += 3 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte4
;LLen32_3:
	edx = (eax >> 11) & 7;
	ecx += 7 + edx;
	eax = *(ulong*)(ebx - 3);
	ebx++;
goto LCopyByte6
;LLen32_4:
	edx = (eax >> 12) & 15;
	ecx += 15 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte0
;LLen32_5:
	edx = (eax >> 13) & 31;
	ecx += 31 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte2
;LLen32_6:
	edx = (eax >> 14) & 63;
	ecx += 63 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte4
;LLen32_7:
	edx = (eax >> 15) & 127;
	ecx += 127 + edx;
	eax = *(ulong*)(ebx - 2);
	ebx += 2;
goto LCopyByte6
;LLen32_8:
	edx = (eax >> 16) & 255;
	ecx += 255 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte0
;LLen32_9:
	edx = (eax >> 17) & 511;
	ecx += 511 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte2
;LLen32_10:
	edx = (eax >> 18) & 1023;
	ecx += 1023 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte4
;LLen32_11:
	edx = (eax >> 19) & 2047;
	ecx += 2047 + edx;
	eax = *(ulong*)(ebx - 1);
	ebx += 3;
goto LCopyByte6
;
LDone:
	cbTot  = edi - pvDst;
// clang-format on
