// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.


// set maxAddr=24576
@24576  
D=A 
@maxADDR // SCREEN_MAX_ADDR
M=D 

// set blackDOT
@32767
D=A
@blackDOT
M=D

// set currentAddr
@SCREEN
D=A
@currentADDR
M=D 

// check keyboard input
(CHECK_KBD)
@KBD
D=M
@UPDATE_ADDR
D;JEQ // if RAM[KBD]=0 then goto update_addr

(DRAW)
@blackDOT 
D=M
@currentADDR 
A=M 
M=D 

// check if currentADDR < maxADDR
@currentADDR 
D=M 
@maxADDR
D=D-M  

@END 
D;JGE 

(UPDATE_ADDR)
@currentADDR 
D=M+1
M=D

@CHECK_KBD 
0;JMP // go to DRAW

(END)
@END
0;JMP