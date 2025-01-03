---@diagnostic disable: undefined-global

-- solutuion
workspace "FME"
   -- build configurations
   configurations { "Debug", "Release" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "Speed"

   -- ending filter section
   filter {}

   -- platform configurations
   platforms { "Win64" }

   filter { "platforms:Win64" }
      system ("windows")
      architecture ("x86_64")
      defines { "WINDOWS" }

   -- ending filter section
   filter {}


-- engine project
ENGINENAME = "FME"
project (ENGINENAME)
   kind ("StaticLib")
   language ("C++")
   cppdialect ("C++20")
   location (ENGINENAME)
   libdirs (ENGINENAME .. "/external/lib")
   includedirs ({ENGINENAME .. "/external/include", ENGINENAME .. "/include", ENGINENAME .. "/src"})
   pchheader ("pch.h")
   pchsource(ENGINENAME .. "/src/pch.cpp")
   targetdir ("bin/%{cfg.buildcfg}")

   files { ENGINENAME .. "/**.h", ENGINENAME .. "/**.cpp" }
   removefiles {ENGINENAME .. "/src/platform/**.h", ENGINENAME .. "/src/platform/**.cpp"}

   filter {"system:windows"}
      files{ENGINENAME .. "/src/platform/windows/**.h", ENGINENAME .. "/src/platform/windows/**.cpp"}

   filter {}


-- game project
project ("Game")
   kind ("ConsoleApp")
   language ("C++")
   cppdialect ("C++20")
   location ("Game")
   targetdir ("bin/%{cfg.buildcfg}")
   links {ENGINENAME}
   includedirs {ENGINENAME .. "/include"}

   files { "Game/**.h", "Game/**.cpp" }
