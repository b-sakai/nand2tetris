@3030 // push constant 3030
D=A
@SP
A=M
M=D
@SP
M=M+1
@R3 // pop pointer 0
D=M
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
@3040 // push constant 3040
D=A
@SP
A=M
M=D
@SP
M=M+1
@R4 // pop pointer 1
D=M
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
@32 // push constant 32
D=A
@SP
A=M
M=D
@SP
M=M+1
@THIS // pop this 2
D=M
@2
A=D+A
D=M
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
@46 // push constant 46
D=A
@SP
A=M
M=D
@SP
M=M+1
@THAT // pop that 6
D=M
@6
A=D+A
D=M
D=A
@R13
M=D
@SP
AM=M-1
D=M
@R13
A=M
M=D
@R3 // push pointer 0
D=M
@SP
A=M
M=D
@SP
M=M+1
@R4 // push pointer 1
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP // add
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
@THIS // push this 2
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP // sub
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
@THAT // push that 6
D=M
@6
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP // add
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
