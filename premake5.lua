solution "reflectos"
	location( "build/" .. _ACTION )
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	startproject "reflectos_unittest"

	include "reflectos_unittest.lua"
	include "../unitos/unitos.lua"