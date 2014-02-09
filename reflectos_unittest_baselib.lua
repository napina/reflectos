project "reflectos_unittest_baselib"
	uuid "F2FC1697-E015-4C83-9BB0-15282D024CD3"
	kind "SharedLib"
	language "C++"
	files {
		"tests/baselib/*.h",
		"tests/baselib/*.cpp",
	}
	includedirs { "include" }
	links { }
	objdir( "build/" .. _ACTION )
	location( "build/" .. _ACTION )

	flags { "FatalWarnings", "NoBufferSecurityCheck", "NoEditAndContinue", "NoIncrementalLink", "NoPCH", "NoRTTI" }
	warnings "Extra"

	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		defines { "NDEBUG" }
		flags { "NoRuntimeChecks" }
		optimize "Speed"