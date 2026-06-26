set VAR="%1"
mkdir .\bin\%VAR%\
mkdir .\bin\%VAR%\baseq2
copy .\%VAR%\gamex86.dll .\bin\%VAR%\baseq2\
copy .\%VAR%\gamex86.pdb .\bin\%VAR%\baseq2\
copy .\%VAR%\quake2dx.exe .\bin\%VAR%\
copy .\%VAR%\quake2dx.pdb .\bin\%VAR%\
copy .\%VAR%\ref_gl.dll .\bin\%VAR%\
copy .\%VAR%\ref_gl.pdb .\bin\%VAR%\
copy .\%VAR%\ref_soft.dll .\bin\%VAR%\
rem 7z a -tzip quake2_x86_%VAR%.zip .\bin\%VAR%\*