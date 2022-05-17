
/*
 * Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
 *
 * Extra glue for C<->ASM generated via gemasmpr.pl
 */

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef __IBMC__
#pragma map(BrFixedAbs,"_BrFixedAbs")
#pragma map(BrFixedMul,"_BrFixedMul")
#pragma map(BrFixedMac2,"_BrFixedMac2")
#pragma map(BrFixedMac3,"_BrFixedMac3")
#pragma map(BrFixedMac4,"_BrFixedMac4")
#pragma map(BrFixedSqr,"_BrFixedSqr")
#pragma map(BrFixedSqr2,"_BrFixedSqr2")
#pragma map(BrFixedSqr3,"_BrFixedSqr3")
#pragma map(BrFixedSqr4,"_BrFixedSqr4")
#pragma map(BrFixedLength2,"_BrFixedLength2")
#pragma map(BrFixedLength3,"_BrFixedLength3")
#pragma map(BrFixedLength4,"_BrFixedLength4")
#pragma map(BrFixedRLength2,"_BrFixedRLength2")
#pragma map(BrFixedRLength3,"_BrFixedRLength3")
#pragma map(BrFixedRLength4,"_BrFixedRLength4")
#pragma map(BrFixedDiv,"_BrFixedDiv")
#pragma map(BrFixedDivF,"_BrFixedDivF")
#pragma map(BrFixedDivR,"_BrFixedDivR")
#pragma map(BrFixedMulDiv,"_BrFixedMulDiv")
#pragma map(BrFixedMac2Div,"_BrFixedMac2Div")
#pragma map(BrFixedMac3Div,"_BrFixedMac3Div")
#pragma map(BrFixedMac4Div,"_BrFixedMac4Div")
#pragma map(BrFixedRcp,"_BrFixedRcp")
#pragma map(BrFixedFMac2,"_BrFixedFMac2")
#pragma map(BrFixedFMac3,"_BrFixedFMac3")
#pragma map(BrFixedFMac4,"_BrFixedFMac4")
#pragma map(BrFixedSin,"_BrFixedSin")
#pragma map(BrFixedCos,"_BrFixedCos")
#pragma map(BrFixedASin,"_BrFixedASin")
#pragma map(BrFixedACos,"_BrFixedACos")
#pragma map(BrFixedATan2,"_BrFixedATan2")
#pragma map(BrFixedATan2Fast,"_BrFixedATan2Fast")
#pragma map(BrSqrt,"_BrSqrt")
#pragma map(BrFastSqrt,"_BrFastSqrt")
#pragma map(BrFastRSqrt,"_BrFastRSqrt")
#pragma map(BrBlockFill,"_BrBlockFill")
#pragma map(BrBlockCopy,"_BrBlockCopy")
#endif /* __IBMC__ */

#ifdef __HIGHC__
#pragma Alias(BrFixedAbs,"_BrFixedAbs")
#pragma Alias(BrFixedMul,"_BrFixedMul")
#pragma Alias(BrFixedMac2,"_BrFixedMac2")
#pragma Alias(BrFixedMac3,"_BrFixedMac3")
#pragma Alias(BrFixedMac4,"_BrFixedMac4")
#pragma Alias(BrFixedSqr,"_BrFixedSqr")
#pragma Alias(BrFixedSqr2,"_BrFixedSqr2")
#pragma Alias(BrFixedSqr3,"_BrFixedSqr3")
#pragma Alias(BrFixedSqr4,"_BrFixedSqr4")
#pragma Alias(BrFixedLength2,"_BrFixedLength2")
#pragma Alias(BrFixedLength3,"_BrFixedLength3")
#pragma Alias(BrFixedLength4,"_BrFixedLength4")
#pragma Alias(BrFixedRLength2,"_BrFixedRLength2")
#pragma Alias(BrFixedRLength3,"_BrFixedRLength3")
#pragma Alias(BrFixedRLength4,"_BrFixedRLength4")
#pragma Alias(BrFixedDiv,"_BrFixedDiv")
#pragma Alias(BrFixedDivF,"_BrFixedDivF")
#pragma Alias(BrFixedDivR,"_BrFixedDivR")
#pragma Alias(BrFixedMulDiv,"_BrFixedMulDiv")
#pragma Alias(BrFixedMac2Div,"_BrFixedMac2Div")
#pragma Alias(BrFixedMac3Div,"_BrFixedMac3Div")
#pragma Alias(BrFixedMac4Div,"_BrFixedMac4Div")
#pragma Alias(BrFixedRcp,"_BrFixedRcp")
#pragma Alias(BrFixedFMac2,"_BrFixedFMac2")
#pragma Alias(BrFixedFMac3,"_BrFixedFMac3")
#pragma Alias(BrFixedFMac4,"_BrFixedFMac4")
#pragma Alias(BrFixedSin,"_BrFixedSin")
#pragma Alias(BrFixedCos,"_BrFixedCos")
#pragma Alias(BrFixedASin,"_BrFixedASin")
#pragma Alias(BrFixedACos,"_BrFixedACos")
#pragma Alias(BrFixedATan2,"_BrFixedATan2")
#pragma Alias(BrFixedATan2Fast,"_BrFixedATan2Fast")
#pragma Alias(BrSqrt,"_BrSqrt")
#pragma Alias(BrFastSqrt,"_BrFastSqrt")
#pragma Alias(BrFastRSqrt,"_BrFastRSqrt")
#pragma Alias(BrBlockFill,"_BrBlockFill")
#pragma Alias(BrBlockCopy,"_BrBlockCopy")
#endif /* __IBMC__ */

#ifdef __cplusplus
	};
#endif
