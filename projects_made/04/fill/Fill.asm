// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

(WHITE)
@KBD
D=M
@BLACK
D;JNE

(WLOOP)
@i
D=M
@8992
D=D-A
@WHITE
D;JGT
@i
D=M
@SCREEN
A=A+D
M=0
@i
M=M+1
@WLOOP
@WHITE
0;JMP


(BLACK)
@KBD
D=M
@WHITE
D;JEQ

(BLOOP)
@i
D=M
@8992
D=D-A
@BLACK
D;JGT
@i
D=M
@SCREEN
A=A+D
M=1
@i
M=M+1
@BLOOP

@BLACK
0;JMP