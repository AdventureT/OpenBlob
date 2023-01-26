IncludeDir = {}
IncludeDir.spdlog = "%{wks.location}/Toshi/Vendor/spdlog/include"
IncludeDir.fmod = "%{wks.location}/Toshi/Vendor/fmod/include"
IncludeDir.trbf = "%{wks.location}/Tools/TRBF/Include"
IncludeDir.libogg = "%{wks.location}/Toshi/Vendor/libogg/include"
IncludeDir.libvorbis = "%{wks.location}/Toshi/Vendor/libvorbis/include"

LibDir = {}
LibDir.fmod = "%{wks.location}/Toshi/Vendor/fmod/lib"

-- content of these folders should be copied to any client application
ClientContentCommon = "%{wks.location}Content/Common/"
ClientContentArch   = "%{wks.location}Content"

-- modify paths based on architecture
LibDir.fmod = LibDir.fmod .. "/" .. _OPTIONS["arch"] .. "/"
ClientContentArch = ClientContentArch .. "/" .. _OPTIONS["arch"] .. "/"