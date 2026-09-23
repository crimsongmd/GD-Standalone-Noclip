# IF YOU BEAT TOO MANY LEVELS IN LOW ATTEMPTS WITH NOCLIP, YOU WILL GET LEADERBOARD BANNED
# GD Standalone Noclip

Instead of geode, it just injects a dll into the process
( I kind of think geode is lame so I wanted to challenge myself )

This does flag as a virus on my system, this is because it injects into a process.
If you really don't feel safe downloading it, you can build it yourself and check the entire source code.

This only works on the windows version of gd
If you want to fix it

( THIS ALSO APPLIES IF THE GAME UPDATES)
1. You have to find the RVA, the most simple way is to just go to https://docs.geode-sdk.org/classes/PlayLayer#destroyPlayer
   Once you're there, find the destroyplayer method and click it, you'll see all of the devices.
   Choose the one you're on, then copy it.
2. Open the solution, open NoclipDLL, open dllmain.cpp, then find this line
   "uintptr_t targetAddress = base + 0x3b39d0;".
   The RVA is the numbers at the end, just replace those with what you copied.

# Building

If you want to build this for some reason you need:

Visual Studio
( With desktop development with C++ )

Thats it!

To actually build it, just open the solution, then press build solution.
Then go to the path it shows, and just open up the exe.

MAKE SURE THE DLL IS ALWAYS IN THE SAME FOLDER AS THE INJECTOR!!!!

# Using it

Make sure gd is open, then just open the exe.
