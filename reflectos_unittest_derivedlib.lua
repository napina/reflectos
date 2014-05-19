project "reflectos_unittest_derivedlib"
	uuid "3DB6DA16-99A6-45DF-AA85-4194E0535243"
	kind "SharedLib"
	language "C++"
	files {
		"tests/derivedlib/*.h",
		"tests/derivedlib/*.cpp",
	}
	includedirs { "include", "tests" }
	links { "reflectos_unittest_baselib" }
	objdir( "build/" .. _ACTION )
	location( "build/" .. _ACTION )
	targetname "%{prj.name}_%{cfg.platform}"

	flags { "FatalWarnings", "NoBufferSecurityCheck", "NoEditAndContinue", "NoIncrementalLink", "NoPCH", "NoRTTI" }
	warnings "Extra"

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		targetsuffix "_d"

	configuration "Release"
		defines { "NDEBUG" }
		flags { "NoRuntimeChecks" }
		optimize "Speed"
		targetsuffix "_r"
