# What is this?
I wanted to try the original idtech2 code with Remix, since it's open source, getting it in a working state means I could port changes to other games based on idtech2.

### Changes:
- static geometry is batched via texture id and drawn in a single drawcall; this allows setting r_novis 1 without causing severe slowdown in Remix
- each entity (npc, flags, ammo, health, barrels etc.) is assembled in a vertex buffer and drawn with very few drawcalls, one for each 
unique texture assigned to the model; with r_nocull 2 now affecting entities, again this prevents fps drop in Remix
- in case of static entities lerping has been disabled to make the model's hashes stable for Remix

### Notes:
- I'm using my QindieGL fork to run this with Remix
- executable must be renamed to quake2dx.exe
- r_nocull 1 is a must: I have modified it to render more faces from the BSP
- r_nocull 2 prevents entities from being frustrum culled (e.g. flags on walls are entities)
- gl_polyblend 0 OR cl_blend 0 is necessary to prevent being blinded by the white/orange plane when taking damage or a pickup

### *Original README text follows:*

This is the complete source code for Quake 2, version 3.19, buildable with
visual C++ 6.0.  The linux version should be buildable, but we haven't
tested it for the release.

The code is all licensed under the terms of the GPL (gnu public license).  
You should read the entire license, but the gist of it is that you can do 
anything you want with the code, including sell your new version.  The catch 
is that if you distribute new binary versions, you are required to make the 
entire source code available for free to everyone.

The primary intent of this release is for entertainment and educational 
purposes, but the GPL does allow commercial exploitation if you obey the 
full license.  If you want to do something commercial and you just can't bear 
to have your source changes released, we could still negotiate a separate 
license agreement (for $$$), but I would encourage you to just live with the 
GPL.

All of the Q2 data files remain copyrighted and licensed under the 
original terms, so you cannot redistribute data from the original game, but if 
you do a true total conversion, you can create a standalone game based on 
this code.

Thanks to Robert Duffy for doing the grunt work of building this release.

John Carmack
Id Software
