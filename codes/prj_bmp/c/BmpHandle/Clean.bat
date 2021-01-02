@ECHO OFF

IF EXIST obj               RMDIR /Q/S  obj
IF EXIST bmp_extractor     DEL   /Q    bmp_extractor
IF EXIST bmp_gray          DEL   /Q    bmp_gray
IF EXIST bmp_extractor.exe DEL   /Q    bmp_extractor.exe
IF EXIST bmp_gray.exe      DEL   /Q    bmp_gray.exe
DEL /Q *.bmp
DEL /Q *.raw
