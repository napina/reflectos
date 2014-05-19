project "reflectos_unittest"
	uuid "49C47015-CC76-4B69-8FA5-44EA5A5A0D8A"
	kind "ConsoleApp"
	language "C++"
	files {
		"tests/*.h",
		"tests/*.cpp",
	}
	includedirs { "include", "baselib", "derivedlib", "../unitos/include" }
	links { "reflectos_unittest_baselib", "reflectos_unittest_derivedlib", "unitos" }
	objdir( "build/" .. _ACTION )
	location( "build/" .. _ACTION )
	targetdir( "build/" .. _ACTION )
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

	configuration { "vs*"}
		postbuildcommands { "\"$(TargetPath)\"" }
