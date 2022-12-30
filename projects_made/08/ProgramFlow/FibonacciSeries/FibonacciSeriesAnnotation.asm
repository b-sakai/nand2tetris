@ARG // push argument 1
D=M
@1
A=D+A
D=M
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
@0 // push constant 0
D=A
@SP
A=M
M=D
@SP
M=M+1
@THAT // pop that 0
D=M
@0
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
@1 // push constant 1
D=A
@SP
A=M
M=D
@SP
M=M+1
@THAT // pop that 0
D=M
@1
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
@ARG // push argument 0
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@2 // push constant 2
D=A
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
@ARG // pop argument 0
D=M
@0
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
(MAIN_LOOP_START) // label MAIN_LOOP_START
@ARG // push argument 0
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP // if-goto COMPUTE_ELEMENT
AM=M-1
D=M
@COMPUTE_ELEMENT 
D;JNE
@END_PROGRAM // goto END_PROGRAM
0;JMP
(COMPUTE_ELEMENT) // label COMPUTE_ELEMENT
@THAT // push that 0
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT // push that 1
D=M
@1
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
@THAT // pop that 2
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
@R4 // push pointer 1
D=M
@SP
A=M
M=D
@SP
M=M+1
@1 // push constant 1
D=A
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
@R4 // push pointer 1
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
@ARG // push argument 0
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1
@1 // push constant 1
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
@SP // sub
M=M-1
@SP
A=M
M=M-D
@SP
M=M+1
@ARG // pop argument 0
D=M
@0
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
@MAIN_LOOP_START // goto MAIN_LOOP_START
0;JMP
(END_PROGRAM) // label END_PROGRAM
