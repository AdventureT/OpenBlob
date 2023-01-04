include "Settings.lua"
include "Dependencies.lua"

workspace "Toshi"
	platforms "Windows"
	configurations { "Debug", "Release", "Dist" }
	
	debugdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	filter "options:arch=x86"
		architecture "x86"
		
	filter "options:arch=x64"
		architecture "x64"

include "Toshi"
include "OpenBlob"

group "Tools"
	include "Tools/TRBF"