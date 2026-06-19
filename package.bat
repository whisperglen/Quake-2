set VAR="%1"
mkdir .\bin\%VAR%\
mkdir .\bin\%VAR%\baseq2
copy .\%VAR%\gamex86.dll .\bin\%VAR%\baseq2\
copy .\%VAR%\quake2.exe .\bin\%VAR%\quake2dx.exe
copy .\%VAR%\ref_gl.dll .\bin\%VAR%\
copy .\%VAR%\ref_gl.pdb .\bin\%VAR%\
copy .\%VAR%\ref_soft.dll .\bin\%VAR%\
7z a -tzip quake2_game_binaries.zip .\bin\%VAR%\*