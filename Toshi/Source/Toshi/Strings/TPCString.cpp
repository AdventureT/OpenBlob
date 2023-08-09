#include "ToshiPCH.h"
#include "TPCString.h"

namespace Toshi {

	int Toshi::TPCString::Compare(const TPCString& other) const
	{
		TPooledCString* thisStringPool = GetPtr();
		TPooledCString* otherStringPool = other.GetPtr();

		if (!thisStringPool && otherStringPool) return -1;
		if (!otherStringPool) return 1;
		if (thisStringPool != otherStringPool)
		{
			return thisStringPool->m_oString.Compare(otherStringPool->m_oString, -1);
		}
		return 0;
	}

}
