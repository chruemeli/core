import os
import binascii

scriptPath = os.path.dirname(os.path.realpath(__file__))
path = scriptPath + os.sep + '..' + os.sep + 'webapp' + os.sep + 'dist'

def toCamelCase(text):
    s = text.replace('-', " ").replace('_', ' ').replace('.', ' ')
    s = s.split()
    if len(text) == 0:
        return text
    return s[0] + ''.join(i.capitalize() for i in s[1:])

metaFilePath = scriptPath + os.sep + '..' + os.sep + 'src' + os.sep + 'generated' + os.sep + 'data.h'
metaCFilePath = scriptPath + os.sep + '..' + os.sep + 'src' + os.sep + 'generated' + os.sep + 'data.cpp'
processedFiles = []

with open(metaFilePath, 'w', encoding='utf-8') as metaFile:
    metaFile.write('#ifndef data_H\n')
    metaFile.write('#define data_H\n\n')
    metaFile.write('#include "../WebServer.h"\n\n')

    for subdir, dirs, files in os.walk(path):
        for filename in files:
            inFilePath = subdir + os.sep + filename

            if inFilePath.endswith(".html") or inFilePath.endswith(".css.gz") or inFilePath.endswith(".js.gz"):
                processedFiles.append(filename)
                inFileSize = os.path.getsize(inFilePath)

                identifier = toCamelCase(filename)
                outFilePath = scriptPath + os.sep + '..' + os.sep + 'src' + os.sep + 'generated' + os.sep + "data_" + identifier + ".h"

                with open(inFilePath, 'rb') as inFile, open(outFilePath, 'w', encoding='utf-8') as outFile:
                    outFile.write('#ifndef data_' + identifier + '_H\n')
                    outFile.write('#define data_' + identifier + '_H\n\n')
                    outFile.write('#include <Arduino.h>\n\n')
                    outFile.write('const size_t ' + identifier + 'Len = ' + str(inFileSize) + ';\n\n')

                    outFile.write('const uint8_t ' + identifier + '[] PROGMEM = {')

                    for i in range(inFileSize):
                        byte_s = inFile.read(1)

                        if i % 32 == 0:
                            outFile.write('\n')

                        outFile.write(' 0x' + byte_s.hex())
                        if i < inFileSize - 1:
                            outFile.write(',')

                    outFile.write('\n};\n')
                    outFile.write("\n#endif\n")

                    metaFile.write('#include "data_' + identifier + '.h"\n')
    
    metaFile.write("\n#endif\n")
