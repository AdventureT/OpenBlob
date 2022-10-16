#pragma once
namespace Toshi2
{
	template<typename T, int Capacity>
	class T2Vector
	{
		int m_iNumElements = 0;

		T m_array[Capacity];

		template<typename I>
		void push_back(I item)
		{
			m_array[m_iNumElements++] = item;
		}
	};
}

