@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
D=M-D
@TRUE0
D;JEQ
D=0
@SP
A=M
M=D
@FALSE0
0;JMP
(TRUE0)
D=-1
@SP
A=M
M=D
(FALSE0)
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
D=M-D
@TRUE1
D;JEQ
D=0
@SP
A=M
M=D
@FALSE1
0;JMP
(TRUE1)
D=-1
@SP
A=M
M=D
(FALSE1)
@SP
M=M+1
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
D=M-D
@TRUE2
D;JEQ
D=0
@SP
A=M
M=D
@FALSE2
0;JMP
(TRUE2)
D=-1
@SP
A=M
M=D
(FALSE2)
@SP
M=M+1
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
D=M-D
@TRUE3
D;JLT
D=0
@SP
A=M
M=D
@FALSE3
0;JMP
(TRUE3)
D=-1
@SP
A=M
M=D
(FALSE3)
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
D=M-D
@TRUE4
D;JLT
D=0
@SP
A=M
M=D
@FALSE4
0;JMP
(TRUE4)
D=-1
@SP
A=M
M=D
(FALSE4)
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
D=M-D
@TRUE5
D;JLT
D=0
@SP
A=M
M=D
@FALSE5
0;JMP
(TRUE5)
D=-1
@SP
A=M
M=D
(FALSE5)
@SP
M=M+1
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
D=M-D
@TRUE6
D;JGT
D=0
@SP
A=M
M=D
@FALSE6
0;JMP
(TRUE6)
D=-1
@SP
A=M
M=D
(FALSE6)
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
D=M-D
@TRUE7
D;JGT
D=0
@SP
A=M
M=D
@FALSE7
0;JMP
(TRUE7)
D=-1
@SP
A=M
M=D
(FALSE7)
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
D=M-D
@TRUE8
D;JGT
D=0
@SP
A=M
M=D
@FALSE8
0;JMP
(TRUE8)
D=-1
@SP
A=M
M=D
(FALSE8)
@SP
M=M+1
@57
D=A
@SP
A=M
M=D
@SP
M=M+1
@31
D=A
@SP
A=M
M=D
@SP
M=M+1
@53
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
M=M+D
@SP
M=M+1
@112
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
M=M-D
@SP
M=M+1
@SP
A=M-1
M=-M
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
M=M&D
@SP
M=M+1
@82
D=A
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
A=M
D=M
@SP
M=M-1
@SP
A=M
M=M|D
@SP
M=M+1
@SP
A=M-1
M=!M
