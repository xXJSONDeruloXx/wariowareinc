.section .text
.thumb
.syntax unified
.include "include/gba.inc"

glabel func_08002038
.thumb_func
/* 08002038 */ PUSH {LR}
/* 0800203A */ BL func_08002038_c
/* 0800203E */ POP {R0}
/* 08002040 */ BX R0

.ltorg