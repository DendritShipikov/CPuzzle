call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
cl /O2 /EHsc /c /W3 /sdl /permissive- /Fo"./build/" "./src/main.c" "./src/puzzle.c"
link /out:"./build/puzzle.exe" "./build/main.obj" "./build/puzzle.obj"