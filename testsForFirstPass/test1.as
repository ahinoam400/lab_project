cmp #4,   r8
macro mac1
dec KOBY
clr r5
rts
endm
DAN: .string "dandan"
sub     YAIR[r13],       r1
mov #5, YONI
.entry DAN
mac1
     red r4
KOBY: .data 3,25,42,-16
not r10
.extern YONI
YAIR:   bne KOBY[r10]
.data 0
.entry SHIMON
prn r2 
SHIMON:     add #22,          r5
mac1
stop
