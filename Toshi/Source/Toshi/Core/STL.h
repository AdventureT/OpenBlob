#pragma once
#include "Toshi/Core/TMemory.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <map>

namespace Toshi
{
	namespace STL
	{
		template <typename T>
		using Ref = std::shared_ptr<T>;

		template <typename T>
		using Scope = std::unique_ptr<T, Toshi::TMemoryDeleter>;

		template <typename T>
		using Vector = std::vector<T, Toshi::TAllocator<T>>;

		template <typename Key, typename T>
		using Map = std::map<Key, T, std::less<Key>, Toshi::TAllocator<std::pair<const Key, T>>>;

		template <typename Key, typename T>
		using UnorderedMap = std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, Toshi::TAllocator<std::pair<const Key, T>>>;

		template <typename T, typename ... Args>
		constexpr Ref<T> CreateRef(Args&& ... args)
		{
			return std::allocate_shared<T>(Toshi::TAllocator<T>(), std::forward<Args>(args)...);
		}

		template <typename T, typename ... Args>
		constexpr Scope<T> CreateScope(Args&& ... args)
		{
			Toshi::TAllocator<T> allocator;
			return Scope<T>(allocator.create(std::forward<Args>(args)...));
		}
	}
}