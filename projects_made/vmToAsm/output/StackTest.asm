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
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@TRUE1
D;JLT
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
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
@TRUE2
D;JGT
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
@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M+D
@SP
M=M+1
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
