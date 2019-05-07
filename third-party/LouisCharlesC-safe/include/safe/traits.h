/**
 * @file traits.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2019-01-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <mutex>
#if __cplusplus >= 201402L
#include <shared_mutex>
#endif // __cplusplus >= 201402L

namespace safe
{
	enum class AccessMode
	{
		ReadOnly,
		ReadWrite
	};

	template<template<typename> class LockType>
	struct LockTraits
	{
		static constexpr bool IsReadOnly = false;
	};
	template<>
	struct LockTraits<std::lock_guard>
	{
		static constexpr bool IsReadOnly = false;
	};
	template<>
	struct LockTraits<std::unique_lock>
	{
		static constexpr bool IsReadOnly = false;
	};
#if __cplusplus >= 201402L
	template<>
	struct LockTraits<std::shared_lock>
	{
		static constexpr bool IsReadOnly = true;
	};
#endif // __cplusplus >= 201402L
} // namespace safe