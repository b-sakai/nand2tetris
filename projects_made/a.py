dictm={
        "R0":format(0,'016b'),
        "R1":format(1,'016b'),
        "R2":format(2,'016b'),
        "R3":format(3,'016b'),
        "R4":format(4,'016b'),
        "R5":format(5,'016b'),
        "R6":format(6,'016b'),
        "R7":format(7,'016b'),
        "R8":format(8,'016b'),
        "R9":format(9,'016b'),
        "R10":format(10,'016b'),
        "R11":format(11,'016b'),
        "R12":format(12,'016b'),
        "R13":format(13,'016b'),
        "R14":format(14,'016b'),
        "R15":format(15,'016b'),
        "SP":format(0,'016b'),
        "LCL":format(1,'016b'),
        "ARG":format(2,'016b'),
        "THIS":format(3,'016b'),
        "THAT":format(4,'016b'),
        "SCREEN":format(16384,'016b'),
        "KBD":format(24576,'016b')
        }
dictcomp={
            "0"  :"0101010",
            "1"  :"0111111",
            "-1" :"0111010",
            "D"  :"0001100",
            "A"  :"0110000",
            "!D" :"0001101",
            "!A" :"0110011",
            "-D" :"0001111",
            "-A" :"0110011",
            "D+1":"0011111",
            "A+1":"0110111",
            "D-1":"0001110",
            "A-1":"0110010",
            "D+A":"0000010",
            "D-A":"0010011",
            "A-D":"0000111",
            "D&A":"0000000",
            "D|A":"0010101",

            "M"  :"1110000",
            "!M" :"1110001",
            "-M" :"1110011",
            "M+1":"1110111",
            "M-1":"1110010",
            "D+M":"1000010",
            "D-M":"1010011",
            "M-D":"1000111",
            "D&M":"1000000",
            "D|M":"1010101"
        }
dictdest={
            "null":"000",
            "M"   :"001",
            "D"   :"010",
            "MD"  :"011",
            "A"   :"100",
            "AM"  :"101",
            "AD"  :"110",
            "AMD" :"111"
        }
dictjump={
            "null":"000",
            "JGT" :"001",
            "JEQ" :"010",
            "JGE" :"011",
            "JLT" :"100",
            "JNE" :"101",
            "JLE" :"110",
            "JMP" :"111"
        }

def convert(line):
    for i in  range(len(line)):
            if line[i]==';':
             comp=dictcomp[line[0:i]]
             jump=dictjump[line[i+1:]]
             dest="000"
            if line[i]=='=':
             dest=dictdest[line[0:i]]
             comp=dictcomp[line[i+1:]]
             jump="000"
    wb="111"+comp+dest+jump
    fw.write(wb)
    fw.write("\n")
        
   

fr=open("06/pong/Pong.asm","r")
fw=open("06/pong/Pong1.hack","w")
minumber=16
rom=0
for line in fr:
    text0=line.strip()
    if len(text0)!=0:
        if text0[0]=='@':
            rom=rom+1
            if text0[1:] not in dictm:
                dictm[text0[1:]]=format(minumber,'016b')
                minumber=minumber+1
        if text0[0]=='(':
            dictm[text0[1:]]=format(rom,'016b')
        if text0[0]!='@'and text0[0]!='/' and text0[0]!="\n"and text0[0]!="(":
            rom=rom+1
mnumber=16
fr=open("06/pong/Pong.asm","r")
for line in fr:
    if line[0]!='//':
        if line[0]=='@':
            text=line[1:].rstrip("\r\n")
            print(1)
            if text[0:].isdigit():
                fw.write(format(int(line[1:]),'016b'))
                fw.write("\n")
            else:   
              # if text not in dictm:
               #     dictm[line[1:]]=format(mnumber,'016b')
               #     fw.write(format(mnumber,'016b'))
               #     fw.write("\n")
               #     mnumber=mnumber+1
              # else:
                fw.write(dictm[text])
                fw.write("\n")
    
        if line[0]!='@'and line[0]!='/' and line[0]!="\n"and line[0]!="(":
            convert(line.rstrip("\r\n"))


