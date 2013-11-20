import sys
import os

def getFileList(strTop = sys.path[0], bTopDown = False):
    rList = []
    for root, dirs, files in os.walk(strTop, bTopDown):
        root = root.replace(strTop, "", 1)
        root = root.lstrip("\\/")
        for name in files:
            rList.append((os.path.join(root, name)))
    return rList


fileList = getFileList()
codeLines = 0
commentLines = 0
blankLines = 0
noFiles = 0

for f in fileList:
    do = False
    if f.endswith(".cpp") == True:
        do = True
    if f.endswith(".h") == True:
        do = True
        
        
    if do == True:

        noFiles += 1
        
        ifs = open(f)
        lines = ifs.readlines()
        ifs.close()

        inComment = 0
        for l in lines:
            if l.strip() == "":
                blankLines += 1
                continue
            
            if l.find("/*") != -1:                  
                inComment = 1

            if l.find("*/") != -1:
                inComment = 0
                commentLines += 1
                continue

            if inComment == 1:
                 commentLines += 1
                 continue

            if l.find("//") != -1:
                commentLines += 1
                continue

            codeLines += 1

print "Source Files: %d" % noFiles
print "Total Lines: %d" % (codeLines + commentLines + blankLines)
print "Lines of Code: %d" % codeLines
print "Lines of Comment: %d" % commentLines
print "Blank Lines: %d" % blankLines
print ""
print "About %d A4 pages" % ((codeLines + commentLines + blankLines) / 46)

        
i = input("Press Any Key to Continue...")
