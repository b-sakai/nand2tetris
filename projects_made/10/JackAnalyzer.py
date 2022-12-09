import sys
print sys.argv[1][:-4]
fr=open(sys.argv[1], "r")
fw=open(sys.argv[1][:-4] +"xml", "w")
for line in fr:
    fw.write(line)

fr.close()
fw.close()
