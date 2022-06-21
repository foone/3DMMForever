#include <brender.h>
#include <stddef.h>

#define FONT_WIDTH 3
#define FONT_HEIGHT 5

#pragma pack(push, 1)
typedef struct
{
    br_uint_8 glyph_space[FONT_HEIGHT];
    br_uint_8 glyph_exclam[FONT_HEIGHT];
    br_uint_8 glyph_dquote[FONT_HEIGHT];
    br_uint_8 glyph_pound[FONT_HEIGHT];
    br_uint_8 glyph_hash[FONT_HEIGHT];
    br_uint_8 glyph_percent[FONT_HEIGHT];
    br_uint_8 glyph_ampersand[FONT_HEIGHT];
    br_uint_8 glyph_quote[FONT_HEIGHT];
    br_uint_8 glyph_lparen[FONT_HEIGHT];
    br_uint_8 glyph_rparen[FONT_HEIGHT];
    br_uint_8 glyph_asterisk[FONT_HEIGHT];
    br_uint_8 glyph_plus[FONT_HEIGHT];
    br_uint_8 glyph_comma[FONT_HEIGHT];
    br_uint_8 glyph_minus[FONT_HEIGHT];
    br_uint_8 glyph_period[FONT_HEIGHT];
    br_uint_8 glyph_slash[FONT_HEIGHT];
    br_uint_8 glyph_0[FONT_HEIGHT];
    br_uint_8 glyph_1[FONT_HEIGHT];
    br_uint_8 glyph_2[FONT_HEIGHT];
    br_uint_8 glyph_3[FONT_HEIGHT];
    br_uint_8 glyph_4[FONT_HEIGHT];
    br_uint_8 glyph_5[FONT_HEIGHT];
    br_uint_8 glyph_6[FONT_HEIGHT];
    br_uint_8 glyph_7[FONT_HEIGHT];
    br_uint_8 glyph_8[FONT_HEIGHT];
    br_uint_8 glyph_9[FONT_HEIGHT];
    br_uint_8 glyph_colon[FONT_HEIGHT];
    br_uint_8 glyph_semicolon[FONT_HEIGHT];
    br_uint_8 glyph_langle[FONT_HEIGHT];
    br_uint_8 glyph_equal[FONT_HEIGHT];
    br_uint_8 glyph_rangle[FONT_HEIGHT];
    br_uint_8 glyph_query[FONT_HEIGHT];
    br_uint_8 glyph_comat[FONT_HEIGHT];
    br_uint_8 glyph_A[FONT_HEIGHT];
    br_uint_8 glyph_B[FONT_HEIGHT];
    br_uint_8 glyph_C[FONT_HEIGHT];
    br_uint_8 glyph_D[FONT_HEIGHT];
    br_uint_8 glyph_E[FONT_HEIGHT];
    br_uint_8 glyph_F[FONT_HEIGHT];
    br_uint_8 glyph_G[FONT_HEIGHT];
    br_uint_8 glyph_H[FONT_HEIGHT];
    br_uint_8 glyph_I[FONT_HEIGHT];
    br_uint_8 glyph_J[FONT_HEIGHT];
    br_uint_8 glyph_K[FONT_HEIGHT];
    br_uint_8 glyph_L[FONT_HEIGHT];
    br_uint_8 glyph_M[FONT_HEIGHT];
    br_uint_8 glyph_N[FONT_HEIGHT];
    br_uint_8 glyph_O[FONT_HEIGHT];
    br_uint_8 glyph_P[FONT_HEIGHT];
    br_uint_8 glyph_Q[FONT_HEIGHT];
    br_uint_8 glyph_R[FONT_HEIGHT];
    br_uint_8 glyph_S[FONT_HEIGHT];
    br_uint_8 glyph_T[FONT_HEIGHT];
    br_uint_8 glyph_U[FONT_HEIGHT];
    br_uint_8 glyph_V[FONT_HEIGHT];
    br_uint_8 glyph_W[FONT_HEIGHT];
    br_uint_8 glyph_X[FONT_HEIGHT];
    br_uint_8 glyph_Y[FONT_HEIGHT];
    br_uint_8 glyph_Z[FONT_HEIGHT];
    br_uint_8 glyph_lsquare[FONT_HEIGHT];
    br_uint_8 glyph_bslash[FONT_HEIGHT];
    br_uint_8 glyph_rsquare[FONT_HEIGHT];
    br_uint_8 glyph_caret[FONT_HEIGHT];
    br_uint_8 glyph_underbar[FONT_HEIGHT];
    br_uint_8 glyph_bquote[FONT_HEIGHT];
    br_uint_8 glyph_lbrace[FONT_HEIGHT];
    br_uint_8 glyph_vbar[FONT_HEIGHT];
    br_uint_8 glyph_rbrace[FONT_HEIGHT];
    br_uint_8 glyph_tilde[FONT_HEIGHT];
    br_uint_8 glyph_openbox[FONT_HEIGHT];
    br_uint_8 glyph_alpha[FONT_HEIGHT];
    br_uint_8 glyph_sup2[FONT_HEIGHT];
    br_uint_8 glyph_degree[FONT_HEIGHT];
    br_uint_8 glyph_a[FONT_HEIGHT];
    br_uint_8 glyph_b[FONT_HEIGHT];
    br_uint_8 glyph_c[FONT_HEIGHT];
    br_uint_8 glyph_d[FONT_HEIGHT];
    br_uint_8 glyph_e[FONT_HEIGHT];
    br_uint_8 glyph_f[FONT_HEIGHT];
    br_uint_8 glyph_g[FONT_HEIGHT];
    br_uint_8 glyph_h[FONT_HEIGHT];
    br_uint_8 glyph_i[FONT_HEIGHT];
    br_uint_8 glyph_j[FONT_HEIGHT];
    br_uint_8 glyph_k[FONT_HEIGHT];
    br_uint_8 glyph_l[FONT_HEIGHT];
    br_uint_8 glyph_m[FONT_HEIGHT];
    br_uint_8 glyph_n[FONT_HEIGHT];
    br_uint_8 glyph_o[FONT_HEIGHT];
    br_uint_8 glyph_p[FONT_HEIGHT];
    br_uint_8 glyph_q[FONT_HEIGHT];
    br_uint_8 glyph_r[FONT_HEIGHT];
    br_uint_8 glyph_s[FONT_HEIGHT];
    br_uint_8 glyph_t[FONT_HEIGHT];
    br_uint_8 glyph_u[FONT_HEIGHT];
    br_uint_8 glyph_v[FONT_HEIGHT];
    br_uint_8 glyph_w[FONT_HEIGHT];
    br_uint_8 glyph_x[FONT_HEIGHT];
    br_uint_8 glyph_y[FONT_HEIGHT];
    br_uint_8 glyph_z[FONT_HEIGHT];
    br_uint_8 glyph_ld_tb[FONT_HEIGHT];
    br_uint_8 glyph_ld_tbl[FONT_HEIGHT];
    br_uint_8 glyph_ld_tbL[FONT_HEIGHT];
    br_uint_8 glyph_ld_TBl[FONT_HEIGHT];
    br_uint_8 glyph_ld_Bl[FONT_HEIGHT];
    br_uint_8 glyph_ld_bL[FONT_HEIGHT];
    br_uint_8 glyph_ld_TBL[FONT_HEIGHT];
    br_uint_8 glyph_ld_TB[FONT_HEIGHT];
    br_uint_8 glyph_ld_BL[FONT_HEIGHT];
    br_uint_8 glyph_ld_TL[FONT_HEIGHT];
    br_uint_8 glyph_ld_Tl[FONT_HEIGHT];
    br_uint_8 glyph_ld_tL[FONT_HEIGHT];
    br_uint_8 glyph_ld_bl[FONT_HEIGHT];
    br_uint_8 glyph_ld_tr[FONT_HEIGHT];
    br_uint_8 glyph_ld_tlr[FONT_HEIGHT];
    br_uint_8 glyph_ld_blr[FONT_HEIGHT];
    br_uint_8 glyph_ld_tbr[FONT_HEIGHT];
    br_uint_8 glyph_ld_lr[FONT_HEIGHT];
    br_uint_8 glyph_ld_tblr[FONT_HEIGHT];
    br_uint_8 glyph_ld_tbR[FONT_HEIGHT];
    br_uint_8 glyph_ld_TBr[FONT_HEIGHT];
    br_uint_8 glyph_ld_TR[FONT_HEIGHT];
    br_uint_8 glyph_ld_BR[FONT_HEIGHT];
    br_uint_8 glyph_ld_TLR[FONT_HEIGHT];
    br_uint_8 glyph_ld_BLR[FONT_HEIGHT];
    br_uint_8 glyph_ld_TBR[FONT_HEIGHT];
    br_uint_8 glyph_ld_LR[FONT_HEIGHT];
    br_uint_8 glyph_ld_TBLR[FONT_HEIGHT];
    br_uint_8 glyph_ld_tLR[FONT_HEIGHT];
    br_uint_8 glyph_ld_Tlr[FONT_HEIGHT];
    br_uint_8 glyph_ld_bLR[FONT_HEIGHT];
    br_uint_8 glyph_ld_Blr[FONT_HEIGHT];
    br_uint_8 glyph_ld_Tr[FONT_HEIGHT];
    br_uint_8 glyph_ld_tR[FONT_HEIGHT];
    br_uint_8 glyph_ld_bR[FONT_HEIGHT];
    br_uint_8 glyph_ld_Br[FONT_HEIGHT];
    br_uint_8 glyph_ld_TBlr[FONT_HEIGHT];
    br_uint_8 glyph_ld_tbLR[FONT_HEIGHT];
    br_uint_8 glyph_ld_tl[FONT_HEIGHT];
    br_uint_8 glyph_ld_br[FONT_HEIGHT];
    br_uint_8 glyph_arrow[FONT_HEIGHT];
    br_uint_8 glyph_uparrow[FONT_HEIGHT];
    br_uint_8 glyph_downarrow[FONT_HEIGHT];
    br_uint_8 glyph_leftarrow[FONT_HEIGHT];
    br_uint_8 glyph_rightarrow[FONT_HEIGHT];
    br_uint_8 glyph_tick[FONT_HEIGHT];
    br_uint_8 glyph_bullet[FONT_HEIGHT];
} __font_glyphs;
#pragma pack(pop)

__font_glyphs _FontFixed3x5_Glyphs = {.glyph_space =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_dquote =
                                          {
                                              0b10100000,
                                              0b10100000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_pound =
                                          {
                                              0b01100000,
                                              0b01000000,
                                              0b11100000,
                                              0b01000000,
                                              0b11100000,
                                          },
                                      .glyph_hash =
                                          {
                                              0b01000000,
                                              0b11100000,
                                              0b01000000,
                                              0b11100000,
                                              0b01000000,
                                          },
                                      .glyph_percent =
                                          {
                                              0b10100000,
                                              0b00100000,
                                              0b01000000,
                                              0b10000000,
                                              0b10100000,
                                          },
                                      .glyph_ampersand =
                                          {
                                              0b01000000,
                                              0b10100000,
                                              0b10100000,
                                              0b01000000,
                                              0b10100000,
                                          },
                                      .glyph_quote =
                                          {
                                              0b01000000,
                                              0b10000000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_lparen =
                                          {
                                              0b00100000,
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                              0b00100000,
                                          },
                                      .glyph_rparen =
                                          {
                                              0b10000000,
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                              0b10000000,
                                          },
                                      .glyph_asterisk =
                                          {
                                              0b00000000,
                                              0b10100000,
                                              0b01000000,
                                              0b10100000,
                                              0b00000000,
                                          },
                                      .glyph_plus =
                                          {
                                              0b00000000,
                                              0b01000000,
                                              0b11100000,
                                              0b01000000,
                                              0b00000000,
                                          },
                                      .glyph_comma =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                              0b01000000,
                                              0b10000000,
                                          },
                                      .glyph_minus =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b11100000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_period =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                              0b01000000,
                                          },
                                      .glyph_slash =
                                          {
                                              0b00100000,
                                              0b00100000,
                                              0b01000000,
                                              0b10000000,
                                              0b10000000,
                                          },
                                      .glyph_0 =
                                          {
                                              0b11100000,
                                              0b10100000,
                                              0b10100000,
                                              0b10100000,
                                              0b11100000,
                                          },
                                      .glyph_1 =
                                          {
                                              0b11000000,
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                              0b11100000,
                                          },
                                      .glyph_2 =
                                          {
                                              0b11100000,
                                              0b00100000,
                                              0b11100000,
                                              0b10000000,
                                              0b11100000,
                                          },
                                      .glyph_3 =
                                          {
                                              0b11100000,
                                              0b00100000,
                                              0b01100000,
                                              0b00100000,
                                              0b11100000,
                                          },
                                      .glyph_4 =
                                          {
                                              0b10000000,
                                              0b10000000,
                                              0b10100000,
                                              0b11100000,
                                              0b00100000,
                                          },
                                      .glyph_5 =
                                          {
                                              0b11100000,
                                              0b10000000,
                                              0b11100000,
                                              0b00100000,
                                              0b11100000,
                                          },
                                      .glyph_6 =
                                          {
                                              0b11100000,
                                              0b10000000,
                                              0b11100000,
                                              0b10100000,
                                              0b11100000,
                                          },
                                      .glyph_7 =
                                          {
                                              0b11100000,
                                              0b00100000,
                                              0b00100000,
                                              0b00100000,
                                              0b00100000,
                                          },
                                      .glyph_8 =
                                          {
                                              0b11100000,
                                              0b10100000,
                                              0b11100000,
                                              0b10100000,
                                              0b11100000,
                                          },
                                      .glyph_9 =
                                          {
                                              0b11100000,
                                              0b10100000,
                                              0b11100000,
                                              0b00100000,
                                              0b11100000,
                                          },
                                      .glyph_colon =
                                          {
                                              0b00000000,
                                              0b01000000,
                                              0b00000000,
                                              0b01000000,
                                              0b00000000,
                                          },
                                      .glyph_semicolon =
                                          {
                                              0b00000000,
                                              0b01000000,
                                              0b00000000,
                                              0b01000000,
                                              0b10000000,
                                          },
                                      .glyph_langle =
                                          {
                                              0b00100000,
                                              0b01000000,
                                              0b10000000,
                                              0b01000000,
                                              0b00100000,
                                          },
                                      .glyph_equal =
                                          {
                                              0b00000000,
                                              0b11100000,
                                              0b00000000,
                                              0b11100000,
                                              0b00000000,
                                          },
                                      .glyph_rangle =
                                          {
                                              0b10000000,
                                              0b01000000,
                                              0b00100000,
                                              0b01000000,
                                              0b10000000,
                                          },
                                      .glyph_query =
                                          {
                                              0b11100000,
                                              0b00100000,
                                              0b01100000,
                                              0b00000000,
                                              0b01000000,
                                          },
                                      .glyph_comat =
                                          {
                                              0b11100000,
                                              0b10100000,
                                              0b10100000,
                                              0b10000000,
                                              0b11100000,
                                          },
                                      .glyph_A =
                                          {
                                              0b01000000,
                                              0b10100000,
                                              0b10100000,
                                              0b11100000,
                                              0b10100000,
                                          },
                                      .glyph_B =
                                          {
                                              0b11000000,
                                              0b10100000,
                                              0b11000000,
                                              0b10100000,
                                              0b11000000,
                                          },
                                      .glyph_C =
                                          {
                                              0b01100000,
                                              0b10000000,
                                              0b10000000,
                                              0b10000000,
                                              0b01100000,
                                          },
                                      .glyph_D =
                                          {
                                              0b11000000,
                                              0b10100000,
                                              0b10100000,
                                              0b10100000,
                                              0b11000000,
                                          },
                                      .glyph_E =
                                          {
                                              0b11100000,
                                              0b10000000,
                                              0b11000000,
                                              0b10000000,
                                              0b11100000,
                                          },
                                      .glyph_F =
                                          {
                                              0b11100000,
                                              0b10000000,
                                              0b11000000,
                                              0b10000000,
                                              0b10000000,
                                          },
                                      .glyph_G =
                                          {
                                              0b01100000,
                                              0b10000000,
                                              0b10100000,
                                              0b10100000,
                                              0b01100000,
                                          },
                                      .glyph_H =
                                          {
                                              0b10100000,
                                              0b10100000,
                                              0b11100000,
                                              0b10100000,
                                              0b10100000,
                                          },
                                      .glyph_I =
                                          {
                                              0b11100000,
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                              0b11100000,
                                          },
                                      .glyph_J =
                                          {
                                              0b11100000,
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                              0b11000000,
                                          },
                                      .glyph_K =
                                          {
                                              0b10100000,
                                              0b11100000,
                                              0b11000000,
                                              0b11100000,
                                              0b10100000,
                                          },
                                      .glyph_L =
                                          {
                                              0b10000000,
                                              0b10000000,
                                              0b10000000,
                                              0b10000000,
                                              0b11100000,
                                          },
                                      .glyph_M =
                                          {
                                              0b10100000,
                                              0b11100000,
                                              0b11100000,
                                              0b10100000,
                                              0b10100000,
                                          },
                                      .glyph_N =
                                          {
                                              0b10100000,
                                              0b11100000,
                                              0b11100000,
                                              0b11100000,
                                              0b10100000,
                                          },
                                      .glyph_O =
                                          {
                                              0b01000000,
                                              0b10100000,
                                              0b10100000,
                                              0b10100000,
                                              0b01000000,
                                          },
                                      .glyph_P =
                                          {
                                              0b11000000,
                                              0b10100000,
                                              0b11000000,
                                              0b10000000,
                                              0b10000000,
                                          },
                                      .glyph_Q =
                                          {
                                              0b01000000,
                                              0b10100000,
                                              0b10100000,
                                              0b10100000,
                                              0b01100000,
                                          },
                                      .glyph_R =
                                          {
                                              0b11000000,
                                              0b10100000,
                                              0b11000000,
                                              0b10100000,
                                              0b10100000,
                                          },
                                      .glyph_S =
                                          {
                                              0b01100000,
                                              0b10000000,
                                              0b01000000,
                                              0b00100000,
                                              0b11000000,
                                          },
                                      .glyph_T =
                                          {
                                              0b11100000,
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                          },
                                      .glyph_U =
                                          {
                                              0b10100000,
                                              0b10100000,
                                              0b10100000,
                                              0b10100000,
                                              0b11100000,
                                          },
                                      .glyph_V =
                                          {
                                              0b10100000,
                                              0b10100000,
                                              0b10100000,
                                              0b10100000,
                                              0b01000000,
                                          },
                                      .glyph_W =
                                          {
                                              0b10100000,
                                              0b10100000,
                                              0b11100000,
                                              0b11100000,
                                              0b10100000,
                                          },
                                      .glyph_X =
                                          {
                                              0b10100000,
                                              0b10100000,
                                              0b01000000,
                                              0b10100000,
                                              0b10100000,
                                          },
                                      .glyph_Y =
                                          {
                                              0b10100000,
                                              0b10100000,
                                              0b11100000,
                                              0b01000000,
                                              0b01000000,
                                          },
                                      .glyph_Z =
                                          {
                                              0b11100000,
                                              0b00100000,
                                              0b01000000,
                                              0b10000000,
                                              0b11100000,
                                          },
                                      .glyph_lsquare =
                                          {
                                              0b11100000,
                                              0b10000000,
                                              0b10000000,
                                              0b10000000,
                                              0b11100000,
                                          },
                                      .glyph_bslash =
                                          {
                                              0b10000000,
                                              0b10000000,
                                              0b01000000,
                                              0b00100000,
                                              0b00100000,
                                          },
                                      .glyph_rsquare =
                                          {
                                              0b11100000,
                                              0b00100000,
                                              0b00100000,
                                              0b00100000,
                                              0b11100000,
                                          },
                                      .glyph_caret =
                                          {
                                              0b01000000,
                                              0b10100000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_underbar =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                              0b11100000,
                                          },
                                      .glyph_bquote =
                                          {
                                              0b01000000,
                                              0b00100000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_lbrace =
                                          {
                                              0b01100000,
                                              0b01000000,
                                              0b11000000,
                                              0b01000000,
                                              0b01100000,
                                          },
                                      .glyph_vbar =
                                          {
                                              0b10000000,
                                              0b10000000,
                                              0b10000000,
                                              0b10000000,
                                              0b10000000,
                                          },
                                      .glyph_rbrace =
                                          {
                                              0b11000000,
                                              0b01000000,
                                              0b01100000,
                                              0b01000000,
                                              0b11000000,
                                          },
                                      .glyph_tilde =
                                          {
                                              0b01010000,
                                              0b10100000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_openbox =
                                          {
                                              0b11110000,
                                              0b10010000,
                                              0b10010000,
                                              0b10010000,
                                              0b11110000,
                                          },
                                      .glyph_alpha =
                                          {
                                              0b00000000,
                                              0b01010000,
                                              0b10100000,
                                              0b10100000,
                                              0b01010000,
                                          },
                                      .glyph_sup2 =
                                          {
                                              0b11000000,
                                              0b00100000,
                                              0b01000000,
                                              0b11100000,
                                              0b00000000,
                                          },
                                      .glyph_degree =
                                          {
                                              0b11100000,
                                              0b10100000,
                                              0b11100000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_a =
                                          {
                                              0b00000000,
                                              0b01000000,
                                              0b10100000,
                                              0b11100000,
                                              0b10100000,
                                          },
                                      .glyph_b =
                                          {
                                              0b00000000,
                                              0b11000000,
                                              0b11000000,
                                              0b10100000,
                                              0b11000000,
                                          },
                                      .glyph_c =
                                          {
                                              0b00000000,
                                              0b01100000,
                                              0b10000000,
                                              0b10000000,
                                              0b01100000,
                                          },
                                      .glyph_d =
                                          {
                                              0b00000000,
                                              0b11000000,
                                              0b10100000,
                                              0b10100000,
                                              0b11000000,
                                          },
                                      .glyph_e =
                                          {
                                              0b00000000,
                                              0b11100000,
                                              0b11000000,
                                              0b10000000,
                                              0b11100000,
                                          },
                                      .glyph_f =
                                          {
                                              0b00000000,
                                              0b11100000,
                                              0b10000000,
                                              0b11000000,
                                              0b10000000,
                                          },
                                      .glyph_g =
                                          {
                                              0b00000000,
                                              0b01100000,
                                              0b10000000,
                                              0b10100000,
                                              0b01100000,
                                          },
                                      .glyph_h =
                                          {
                                              0b00000000,
                                              0b10100000,
                                              0b10100000,
                                              0b11100000,
                                              0b10100000,
                                          },
                                      .glyph_i =
                                          {
                                              0b00000000,
                                              0b11100000,
                                              0b01000000,
                                              0b01000000,
                                              0b11100000,
                                          },
                                      .glyph_j =
                                          {
                                              0b00000000,
                                              0b11100000,
                                              0b01000000,
                                              0b01000000,
                                              0b11000000,
                                          },
                                      .glyph_k =
                                          {
                                              0b00000000,
                                              0b10100000,
                                              0b11000000,
                                              0b11000000,
                                              0b10100000,
                                          },
                                      .glyph_l =
                                          {
                                              0b00000000,
                                              0b10000000,
                                              0b10000000,
                                              0b10000000,
                                              0b11100000,
                                          },
                                      .glyph_m =
                                          {
                                              0b00000000,
                                              0b10100000,
                                              0b11100000,
                                              0b10100000,
                                              0b10100000,
                                          },
                                      .glyph_n =
                                          {
                                              0b00000000,
                                              0b10100000,
                                              0b11100000,
                                              0b11100000,
                                              0b10100000,
                                          },
                                      .glyph_o =
                                          {
                                              0b00000000,
                                              0b01000000,
                                              0b10100000,
                                              0b10100000,
                                              0b01000000,
                                          },
                                      .glyph_p =
                                          {
                                              0b00000000,
                                              0b11000000,
                                              0b10100000,
                                              0b11000000,
                                              0b10000000,
                                          },
                                      .glyph_q =
                                          {
                                              0b00000000,
                                              0b01000000,
                                              0b10100000,
                                              0b10100000,
                                              0b01100000,
                                          },
                                      .glyph_r =
                                          {
                                              0b00000000,
                                              0b11000000,
                                              0b10100000,
                                              0b11000000,
                                              0b10100000,
                                          },
                                      .glyph_s =
                                          {
                                              0b00000000,
                                              0b01100000,
                                              0b10000000,
                                              0b00100000,
                                              0b11000000,
                                          },
                                      .glyph_t =
                                          {
                                              0b00000000,
                                              0b11100000,
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                          },
                                      .glyph_u =
                                          {
                                              0b00000000,
                                              0b10100000,
                                              0b10100000,
                                              0b10100000,
                                              0b11100000,
                                          },
                                      .glyph_v =
                                          {
                                              0b00000000,
                                              0b10100000,
                                              0b10100000,
                                              0b10100000,
                                              0b01000000,
                                          },
                                      .glyph_w =
                                          {
                                              0b00000000,
                                              0b10100000,
                                              0b10100000,
                                              0b11100000,
                                              0b10100000,
                                          },
                                      .glyph_x =
                                          {
                                              0b00000000,
                                              0b10100000,
                                              0b01000000,
                                              0b01000000,
                                              0b10100000,
                                          },
                                      .glyph_y =
                                          {
                                              0b00000000,
                                              0b10100000,
                                              0b10100000,
                                              0b01000000,
                                              0b01000000,
                                          },
                                      .glyph_z =
                                          {
                                              0b00000000,
                                              0b11100000,
                                              0b00100000,
                                              0b11000000,
                                              0b11100000,
                                          },
                                      .glyph_ld_tb =
                                          {
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                              0b01000000,
                                          },
                                      .glyph_ld_tbl =
                                          {
                                              0b01000000,
                                              0b01000000,
                                              0b11000000,
                                              0b01000000,
                                              0b01000000,
                                          },
                                      .glyph_ld_tbL =
                                          {
                                              0b01000000,
                                              0b11000000,
                                              0b01000000,
                                              0b11000000,
                                              0b01000000,
                                          },
                                      .glyph_ld_TBl =
                                          {
                                              0b01100000,
                                              0b01100000,
                                              0b11100000,
                                              0b01100000,
                                              0b01100000,
                                          },
                                      .glyph_ld_Bl =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b11100000,
                                              0b01100000,
                                              0b01100000,
                                          },
                                      .glyph_ld_bL =
                                          {
                                              0b00000000,
                                              0b11100000,
                                              0b00100000,
                                              0b11100000,
                                              0b00100000,
                                          },
                                      .glyph_ld_TBL =
                                          {
                                              0b01100000,
                                              0b11100000,
                                              0b01100000,
                                              0b11100000,
                                              0b01100000,
                                          },
                                      .glyph_ld_TB =
                                          {
                                              0b01100000,
                                              0b01100000,
                                              0b01100000,
                                              0b01100000,
                                              0b01100000,
                                          },
                                      .glyph_ld_BL =
                                          {
                                              0b00000000,
                                              0b01110000,
                                              0b01100000,
                                              0b01110000,
                                              0b01100000,
                                          },
                                      .glyph_ld_TL =
                                          {
                                              0b01100000,
                                              0b01110000,
                                              0b01100000,
                                              0b01110000,
                                              0b00000000,
                                          },
                                      .glyph_ld_Tl =
                                          {
                                              0b01100000,
                                              0b01100000,
                                              0b11100000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_ld_tL =
                                          {
                                              0b01000000,
                                              0b11000000,
                                              0b01000000,
                                              0b11000000,
                                              0b00000000,
                                          },
                                      .glyph_ld_bl =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b11000000,
                                              0b01000000,
                                              0b01000000,
                                          },
                                      .glyph_ld_tr =
                                          {
                                              0b01000000,
                                              0b01000000,
                                              0b01110000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_ld_tlr =
                                          {
                                              0b01000000,
                                              0b01000000,
                                              0b11110000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_ld_blr =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b11110000,
                                              0b01000000,
                                              0b01000000,
                                          },
                                      .glyph_ld_tbr =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_ld_lr =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b11110000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_ld_tblr =
                                          {
                                              0b01000000,
                                              0b01000000,
                                              0b11110000,
                                              0b01000000,
                                              0b01000000,
                                          },
                                      .glyph_ld_tbR =
                                          {
                                              0b01000000,
                                              0b01110000,
                                              0b01000000,
                                              0b01110000,
                                              0b01000000,
                                          },
                                      .glyph_ld_TBr =
                                          {
                                              0b01100000,
                                              0b01100000,
                                              0b01110000,
                                              0b01100000,
                                              0b01100000,
                                          },
                                      .glyph_ld_TR =
                                          {
                                              0b01100000,
                                              0b01110000,
                                              0b01100000,
                                              0b01110000,
                                              0b00000000,
                                          },
                                      .glyph_ld_BR =
                                          {
                                              0b00000000,
                                              0b01110000,
                                              0b01100000,
                                              0b01110000,
                                              0b01100000,
                                          },
                                      .glyph_ld_TLR =
                                          {
                                              0b01100000,
                                              0b11110000,
                                              0b01100000,
                                              0b11110000,
                                              0b00000000,
                                          },
                                      .glyph_ld_BLR =
                                          {
                                              0b00000000,
                                              0b11110000,
                                              0b01100000,
                                              0b11110000,
                                              0b01100000,
                                          },
                                      .glyph_ld_TBR =
                                          {
                                              0b01100000,
                                              0b01110000,
                                              0b01100000,
                                              0b01110000,
                                              0b01100000,
                                          },
                                      .glyph_ld_LR =
                                          {
                                              0b00000000,
                                              0b11110000,
                                              0b00000000,
                                              0b11110000,
                                              0b00000000,
                                          },
                                      .glyph_ld_TBLR =
                                          {
                                              0b01100000,
                                              0b11110000,
                                              0b01100000,
                                              0b11110000,
                                              0b01100000,
                                          },
                                      .glyph_ld_tLR =
                                          {
                                              0b01000000,
                                              0b11110000,
                                              0b01000000,
                                              0b11110000,
                                              0b00000000,
                                          },
                                      .glyph_ld_Tlr =
                                          {
                                              0b01100000,
                                              0b01100000,
                                              0b11110000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_ld_bLR =
                                          {
                                              0b00000000,
                                              0b11110000,
                                              0b01000000,
                                              0b11110000,
                                              0b01000000,
                                          },
                                      .glyph_ld_Blr =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b11110000,
                                              0b01100000,
                                              0b01100000,
                                          },
                                      .glyph_ld_Tr =
                                          {
                                              0b01100000,
                                              0b01100000,
                                              0b01110000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_ld_tR =
                                          {
                                              0b01000000,
                                              0b01110000,
                                              0b01000000,
                                              0b01110000,
                                              0b00000000,
                                          },
                                      .glyph_ld_bR =
                                          {
                                              0b00000000,
                                              0b01110000,
                                              0b01000000,
                                              0b01110000,
                                              0b01000000,
                                          },
                                      .glyph_ld_Br =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b01110000,
                                              0b01100000,
                                              0b01100000,
                                          },
                                      .glyph_ld_TBlr =
                                          {
                                              0b01100000,
                                              0b01100000,
                                              0b11110000,
                                              0b01100000,
                                              0b01100000,
                                          },
                                      .glyph_ld_tbLR =
                                          {
                                              0b01000000,
                                              0b11110000,
                                              0b01000000,
                                              0b11110000,
                                              0b01000000,
                                          },
                                      .glyph_ld_tl =
                                          {
                                              0b01000000,
                                              0b01000000,
                                              0b11000000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_ld_br =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b01110000,
                                              0b01000000,
                                              0b01000000,
                                          },
                                      .glyph_arrow =
                                          {
                                              0b11111000,
                                              0b11110000,
                                              0b11110000,
                                              0b11001000,
                                              0b10000100,
                                          },
                                      .glyph_uparrow =
                                          {
                                              0b01000000,
                                              0b11100000,
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                          },
                                      .glyph_downarrow =
                                          {
                                              0b00000000,
                                              0b00000000,
                                              0b00000000,
                                              0b11100000,
                                              0b01000000,
                                          },
                                      .glyph_leftarrow =
                                          {
                                              0b00000000,
                                              0b01000000,
                                              0b11000000,
                                              0b01000000,
                                              0b00000000,
                                          },
                                      .glyph_rightarrow =
                                          {
                                              0b00000000,
                                              0b01000000,
                                              0b01100000,
                                              0b01000000,
                                              0b00000000,
                                          },
                                      .glyph_tick =
                                          {
                                              0b00000000,
                                              0b01100000,
                                              0b01000000,
                                              0b11000000,
                                              0b01000000,
                                          },
                                      .glyph_bullet = {
                                          0b00000000,
                                          0b00000000,
                                          0b01000000,
                                          0b00000000,
                                          0b00000000,
                                      }};

br_uint_16 _FontFixed3x5_Encoding[] = {
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_bullet),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_uparrow),    offsetof(__font_glyphs, glyph_downarrow),
    offsetof(__font_glyphs, glyph_rightarrow), offsetof(__font_glyphs, glyph_leftarrow),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_uparrow),    offsetof(__font_glyphs, glyph_downarrow),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_exclam),
    offsetof(__font_glyphs, glyph_dquote),     offsetof(__font_glyphs, glyph_hash),
    offsetof(__font_glyphs, glyph_pound),      offsetof(__font_glyphs, glyph_percent),
    offsetof(__font_glyphs, glyph_ampersand),  offsetof(__font_glyphs, glyph_quote),
    offsetof(__font_glyphs, glyph_lparen),     offsetof(__font_glyphs, glyph_rparen),
    offsetof(__font_glyphs, glyph_asterisk),   offsetof(__font_glyphs, glyph_plus),
    offsetof(__font_glyphs, glyph_comma),      offsetof(__font_glyphs, glyph_minus),
    offsetof(__font_glyphs, glyph_period),     offsetof(__font_glyphs, glyph_slash),
    offsetof(__font_glyphs, glyph_0),          offsetof(__font_glyphs, glyph_1),
    offsetof(__font_glyphs, glyph_2),          offsetof(__font_glyphs, glyph_3),
    offsetof(__font_glyphs, glyph_4),          offsetof(__font_glyphs, glyph_5),
    offsetof(__font_glyphs, glyph_6),          offsetof(__font_glyphs, glyph_7),
    offsetof(__font_glyphs, glyph_8),          offsetof(__font_glyphs, glyph_9),
    offsetof(__font_glyphs, glyph_colon),      offsetof(__font_glyphs, glyph_semicolon),
    offsetof(__font_glyphs, glyph_langle),     offsetof(__font_glyphs, glyph_equal),
    offsetof(__font_glyphs, glyph_rangle),     offsetof(__font_glyphs, glyph_query),
    offsetof(__font_glyphs, glyph_comat),      offsetof(__font_glyphs, glyph_A),
    offsetof(__font_glyphs, glyph_B),          offsetof(__font_glyphs, glyph_C),
    offsetof(__font_glyphs, glyph_D),          offsetof(__font_glyphs, glyph_E),
    offsetof(__font_glyphs, glyph_F),          offsetof(__font_glyphs, glyph_G),
    offsetof(__font_glyphs, glyph_H),          offsetof(__font_glyphs, glyph_I),
    offsetof(__font_glyphs, glyph_J),          offsetof(__font_glyphs, glyph_K),
    offsetof(__font_glyphs, glyph_L),          offsetof(__font_glyphs, glyph_M),
    offsetof(__font_glyphs, glyph_N),          offsetof(__font_glyphs, glyph_O),
    offsetof(__font_glyphs, glyph_P),          offsetof(__font_glyphs, glyph_Q),
    offsetof(__font_glyphs, glyph_R),          offsetof(__font_glyphs, glyph_S),
    offsetof(__font_glyphs, glyph_T),          offsetof(__font_glyphs, glyph_U),
    offsetof(__font_glyphs, glyph_V),          offsetof(__font_glyphs, glyph_W),
    offsetof(__font_glyphs, glyph_X),          offsetof(__font_glyphs, glyph_Y),
    offsetof(__font_glyphs, glyph_Z),          offsetof(__font_glyphs, glyph_lsquare),
    offsetof(__font_glyphs, glyph_bslash),     offsetof(__font_glyphs, glyph_rsquare),
    offsetof(__font_glyphs, glyph_caret),      offsetof(__font_glyphs, glyph_underbar),
    offsetof(__font_glyphs, glyph_bquote),     offsetof(__font_glyphs, glyph_a),
    offsetof(__font_glyphs, glyph_b),          offsetof(__font_glyphs, glyph_c),
    offsetof(__font_glyphs, glyph_d),          offsetof(__font_glyphs, glyph_e),
    offsetof(__font_glyphs, glyph_f),          offsetof(__font_glyphs, glyph_g),
    offsetof(__font_glyphs, glyph_h),          offsetof(__font_glyphs, glyph_i),
    offsetof(__font_glyphs, glyph_j),          offsetof(__font_glyphs, glyph_k),
    offsetof(__font_glyphs, glyph_l),          offsetof(__font_glyphs, glyph_m),
    offsetof(__font_glyphs, glyph_n),          offsetof(__font_glyphs, glyph_o),
    offsetof(__font_glyphs, glyph_p),          offsetof(__font_glyphs, glyph_q),
    offsetof(__font_glyphs, glyph_r),          offsetof(__font_glyphs, glyph_s),
    offsetof(__font_glyphs, glyph_t),          offsetof(__font_glyphs, glyph_u),
    offsetof(__font_glyphs, glyph_v),          offsetof(__font_glyphs, glyph_w),
    offsetof(__font_glyphs, glyph_x),          offsetof(__font_glyphs, glyph_y),
    offsetof(__font_glyphs, glyph_z),          offsetof(__font_glyphs, glyph_lbrace),
    offsetof(__font_glyphs, glyph_vbar),       offsetof(__font_glyphs, glyph_rbrace),
    offsetof(__font_glyphs, glyph_tilde),      offsetof(__font_glyphs, glyph_arrow),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_ld_tb),
    offsetof(__font_glyphs, glyph_ld_tbl),     offsetof(__font_glyphs, glyph_ld_tbL),
    offsetof(__font_glyphs, glyph_ld_TBl),     offsetof(__font_glyphs, glyph_ld_Bl),
    offsetof(__font_glyphs, glyph_ld_bL),      offsetof(__font_glyphs, glyph_ld_TBL),
    offsetof(__font_glyphs, glyph_ld_TB),      offsetof(__font_glyphs, glyph_ld_BL),
    offsetof(__font_glyphs, glyph_ld_TL),      offsetof(__font_glyphs, glyph_ld_Tl),
    offsetof(__font_glyphs, glyph_ld_tL),      offsetof(__font_glyphs, glyph_ld_bl),
    offsetof(__font_glyphs, glyph_ld_tr),      offsetof(__font_glyphs, glyph_ld_tlr),
    offsetof(__font_glyphs, glyph_ld_blr),     offsetof(__font_glyphs, glyph_ld_tbr),
    offsetof(__font_glyphs, glyph_ld_lr),      offsetof(__font_glyphs, glyph_ld_tblr),
    offsetof(__font_glyphs, glyph_ld_tbR),     offsetof(__font_glyphs, glyph_ld_TBr),
    offsetof(__font_glyphs, glyph_ld_TR),      offsetof(__font_glyphs, glyph_ld_BR),
    offsetof(__font_glyphs, glyph_ld_TLR),     offsetof(__font_glyphs, glyph_ld_BLR),
    offsetof(__font_glyphs, glyph_ld_TBR),     offsetof(__font_glyphs, glyph_ld_LR),
    offsetof(__font_glyphs, glyph_ld_TBLR),    offsetof(__font_glyphs, glyph_ld_tLR),
    offsetof(__font_glyphs, glyph_ld_Tlr),     offsetof(__font_glyphs, glyph_ld_bLR),
    offsetof(__font_glyphs, glyph_ld_Blr),     offsetof(__font_glyphs, glyph_ld_Tr),
    offsetof(__font_glyphs, glyph_ld_tR),      offsetof(__font_glyphs, glyph_ld_bR),
    offsetof(__font_glyphs, glyph_ld_Br),      offsetof(__font_glyphs, glyph_ld_TBlr),
    offsetof(__font_glyphs, glyph_ld_tbLR),    offsetof(__font_glyphs, glyph_ld_tl),
    offsetof(__font_glyphs, glyph_ld_br),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_alpha),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_degree),     offsetof(__font_glyphs, glyph_space),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_tick),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_sup2),
    offsetof(__font_glyphs, glyph_space),      offsetof(__font_glyphs, glyph_space),
};

struct br_font _FontFixed3x5 = {.flags = 0,
                                .glyph_x = FONT_WIDTH,
                                .glyph_y = FONT_HEIGHT,
                                .spacing_x = 4,
                                .spacing_y = 6,
                                .width = NULL,
                                .encoding = _FontFixed3x5_Encoding,
                                .glyphs = (br_uint_8 *)&_FontFixed3x5_Glyphs};