CMSC435Final
============

Graphics Project for CMSC435

Description:
This program is a data visualizer, that shows information about tensor fields between different areas of the brain. The tensor data was gathered by tdMRIs. The tensor data can be used to determine information about the brain, and diagnose diseases.

Compilation:
make

Execution:
FinalProject <COG.txt> <subject file>

Code Also Availible At:
https://github.com/dfilipp1/CMSC435Final/

Executable on the GL server at:
/afs/umbc.edu/users/d/f/dfilipp1/pub/graphics/FinalProject COG.txt subject01.txt

Further Work:
One piece of work that we would have liked to do is fix the major known bug, that is discussed below. If you are able to figure it out, PLEASE let us know, it is really bugging me(Dustin), and I would love to be able to show this off to people on my computer, and have it run fast.
Also, we would have liked to comment the code better, to make it clear what everything was doing.

Known Bugs:
This program does not work well with Mac OS X. The reason for this is a General Protection fault is generated, by OS X's memory security code. The General Protection fault is caused by a pointer's scope being invalidated. We were able to find out a lot of information about what the issue was and where it was happening. However, because OpenGL code requires synchronicity, we could only debug to a point, due to the fact that OpenGL would seg fault if a delagate thread can't send a message to the main thread. Because of this we were unable to get the project working on Macs, but it works on windows machines, and the GL server, because they don't have much memory protection.
A note if you are running this on the GL server, it can be very slow if you are running it off campus, or during a very congested time for the server. If you run it on the server when it is not very congested, and when you are on the campus network it runs a lot smoother.
