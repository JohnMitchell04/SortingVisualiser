-- premake5.lua
workspace "SortingVisualiser"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "SortingVisualiser"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "SortingVisualiser"