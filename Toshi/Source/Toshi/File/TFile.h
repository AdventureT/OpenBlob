#pragma once
#include "Toshi/Utils/TSingleton.h"
#include "Toshi/Strings/TCString.h"

namespace Toshi
{
	class TFileManager : public TSingleton<TFileManager>
	{
	public:
		TFileManager() = default;

	private:
		void ValidateSystemPath();

	public:
		class TFile* CreateFile(const TCString&, unsigned int);
	};

	class TFile
	{
	public:

		enum TSEEK
		{
			TSEEK_SET,
			TSEEK_CUR,
			TSEEK_END
		};

		static TFile* Create(const TCString&, unsigned int);

	private:
		int unk;
		void* file;
	};
}


