solution "reflectos"
	location( "build/" .. _ACTION )
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	startproject "reflectos_unittest"

	include "reflectos_unittest.lua"
	include "reflectos_unittest_baselib.lua"
	include "reflectos_unittest_derivedlib.lua"
	include "../unitos/unitos.lua"