========================================================================
    CONSOLE APPLICATION : INET2PT Project Overview
========================================================================

This is a console application to convert INET transit files (used in 
TranPlan transportation modeling software) to Cube Voyager PT transit
LIN files.

Programmed in C++ using Microsoft Visual C++ Express Edition

Compile and run without options for program arguments.

NOTE: in the source code, the FARESYSTEM attribute is hard coded.  This
is generally a bad practice, but this is something that should be 
manually fixed.  If you have a faresystem to use, change this code (it
is noted!) prior to compiling.  If you need multiple fares, open the 
resulting file in Cube and fix as necessary.

Author: Andrew Rohne - arohne@oki.org - www.oki.org
License: 2012 GPLv3

Tags: Cube, Cube-Voyager, C++, TDM, travel modeling, transportation, transportation modeling

