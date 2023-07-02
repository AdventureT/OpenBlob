#pragma once

#ifdef TOSHI_PLATFORM_WINDOWS
#define TOSHI_MULTIPLATFORM(FILENAME) STRINGIFY(STRCAT_3(Platform/Windows/, FILENAME, _Win.h))
#define TOSHI_MULTIRENDER(FILENAME) STRINGIFY(STRCAT_3(Platform/Windows/DX11/, FILENAME, _DX11.h))
#ifdef TOSHI_PLATFORM_REV_TO_Win
#define TOSHI_MULTIPLATFORM(FILENAME) STRINGIFY(STRCAT_3(Platform/Revolution/, FILENAME, _Rev.h))
#endif
#else TOSHI_PLATFORM_REVOLUTION
#error Unsupported platform
#endif

