/**
 * @file safe.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2018-10-23
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#pragma once

#include "lockable.h"

#include <memory>
#include <mutex>
#include <utility>

namespace safe {
	/**
	 * @brief Multi-threading utility class for variables that are meant
	 * to be modified and read by several threads.
	 * 
	 * To replace the content of a Safe variable, assign to it (using
	 * operator =). To get a copy of it, call copy(). To perform
	 * arbitrary operations of a Safe variable, call writeAccess(). Call
	 * readAccess() to perform arbitrary read-only operations.
	 * 
	 * @tparam ValueType Type of the value object.
	 * @tparam MutexType The type of mutex.
	 */
	template<typename ValueType, typename MutexType = DefaultMutex>
	class Safe
	{
		using LockableValue = Lockable<ValueType, MutexType>;

	public:
		template<typename... Args>
		Safe(Args&&... args):
			m_value(default_construct_mutex, std::forward<Args>(args)...)
		{}

		template<typename Other>
		Safe& operator=(Other&& other)
		{
			*WriteAccess<LockableValue>(m_value) = std::forward<Other>(other);
		}

		WriteAccess<LockableValue> writeAccess()
		{
			return {m_value};
		}

		template<template<typename> class LockType=DefaultReadOnlyLock>
		ReadAccess<LockableValue, LockType> readAccess() const
		{
			return {m_value};
		}

		template<template<typename> class LockType=DefaultReadOnlyLock>
		ValueType copy() const
		{
			return *ReadAccess<LockableValue, LockType>(m_value);
		}

	private:
		LockableValue m_value;
	};

	/**
	 * @brief Copy-on-write optimization for Safe objects of std::shared_ptr!
	 * 
	 * Assignemnt (operator =) replaces the existing Safe object if
	 * possible, otherwise they allocate a new one. writeAccess() also
	 * allocates a new object only if needed. readAccess() and copy()
	 * return a std::shared_ptr, they never make copies.
	 * 
	 * @tparam ValueType The type of std::shared_ptr's pointee.
	 * @tparam MutexType The type of mutex.
	 */
	template<typename ValueType, typename MutexType>
	class Safe<std::shared_ptr<ValueType>, MutexType>
	{
		using LockableValue = Lockable<std::shared_ptr<ValueType>, MutexType>;
		
	public:
		template<typename... Args>
		Safe(Args&&... args):
			m_value(default_construct_mutex, std::make_shared<ValueType>(std::forward<Args>(args)...))
		{}

		template<typename Other>
		Safe& operator=(Other&& other)
		{
			WriteAccess<LockableValue> valueAccess(m_value);

			// If views on the value do exist
			if (!valueAccess->unique())
			{
				// Construct a new shared_ptr
				*valueAccess = std::make_shared<ValueType>(std::forward<Other>(other));
			}
			else // no one owns a view on the value
			{
				// replace the contents of the shared_ptr
				**valueAccess = std::forward<Other>(other);
			}
		}

		WriteAccess<LockableValue> writeAccess()
		{
			WriteAccess<LockableValue, std::unique_lock> valueAccess(m_value);

			// If views on the value do exist
			if (!valueAccess->unique())
			{
				// Create a brand new copy of the value, it is now unique
				*valueAccess = std::make_shared<ValueType>(**valueAccess);
			}

			return {*valueAccess, *valueAccess.lock.release(), std::adopt_lock};
		}

		template<template<typename> class LockType=DefaultReadOnlyLock>
		ReadAccess<LockableValue, LockType> readAccess() const
		{
			return {m_value};
		}

		template<template<typename> class LockType=DefaultReadOnlyLock>
		std::shared_ptr<const ValueType> copy() const
		{
			return std::const_pointer_cast<const ValueType>(*ReadAccess<LockableValue, LockType>(m_value));
		}

	private:
		LockableValue m_value;
	};
}  // namespace safe
