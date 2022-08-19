#pragma once
#include "Core.h"

namespace Toshi
{
	class TDebugConsole
	{
	public:
		virtual ~TDebugConsole() { };

		// Returns true if the console was previously visible
		virtual bool Show(bool state) = 0;

		// Returns true if the console became visible
		virtual bool Toggle() = 0;
	};

	class TDebugConsoleFactory
	{
	public:
		static Scope<TDebugConsole> Create();
	};
}