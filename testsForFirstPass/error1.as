mov L, #20
lea L , #100
notcmd  1
mov 1
stop 1
 .data SYM: 123
 SYM$: 123
STR: .string "abcd"
STR: .string "efgh"
^mov 1 2
.data notanumber
.extern not*a*symbol
mov, 1,2
.data 1,
.data , 1
prn 2,
prn illegal*operand
mov 1 2
mov r20
mov 1,,2
