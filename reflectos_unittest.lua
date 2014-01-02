project "reflectos_unittest"
	uuid "49C47015-CC76-4B69-8FA5-44EA5A5A0D8A"
	kind "ConsoleApp"
	language "C++"
	files {
		"tests/*.h",
		"tests/*.cpp",
	}
	includedirs { "include", "../unitos/include" }
	links { "unitos" }
	objdir( "build/" .. _ACTION )
	targetdir( "build/" .. _ACTION )
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

	configuration { "vs*"}
		postbuildcommands { "\"$(TargetPath)\"" }