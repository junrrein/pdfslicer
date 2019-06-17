/**
 * @file lockable.h
 * @author L.-C. C.
 * @brief 
 * @version 0.1
 * @date 2018-09-21
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#pragma once

#include "traits.h"

#include <mutex>
#include <type_traits>
#include <utility>

namespace safe
{
	using DefaultMutex = std::mutex;
	template<typename MutexType>
	using DefaultReadOnlyLock = std::lock_guard<MutexType>;
	template<typename MutexType>
	using DefaultReadWriteLock = std::lock_guard<MutexType>;

	/**
	 * @brief Tag to use the mutex's default constructor when arguments
	 * are provided to contruct the value object.
	 */
	struct DefaultConstructMutex {};
	static constexpr DefaultConstructMutex default_construct_mutex;
	
	namespace trick
	{
		/**
		 * @brief A helper class that defines a member variable of type
		 * Type. The variable is defined "mutable Type" if Type is not a
		 * reference, the variable is "Type&" if Type is a reference.
		 * 
		 * @tparam Type The type of the variable to define.
		 */
		template<typename Type>
		class MutableIfNotReference
		{
		public:
			MutableIfNotReference() = default;
			template<typename... Arg>
			MutableIfNotReference(Arg&&... arg):
				m_var(std::forward<Arg>(arg)...)
			{}

			Type& get() const
			{
				return m_var;
			}

		private:
			/// Mutable Type object.
			mutable Type m_var;
		};
		template<typename Type>
		class MutableIfNotReference<Type&>
		{
		public:
			MutableIfNotReference(Type& ref):
				m_ref(ref)
			{}

			Type& get() const
			{
				return m_ref;
			}

		private:
			/// Reference to a Type object.
			Type& m_ref;
		};
	} // namespace trick

	/**
	 * @brief Wraps a value together with the mutex that protects it for
	 * multi-threaded access.
	 * 
	 * @tparam ValueType The type of the value to protect.
	 * @tparam MutexType The type of the mutex. Use a shared mutex if
	 * you have access to one.
	 */
	template<typename ValueType, typename MutexType = DefaultMutex>
	class Lockable
	{
	private:
		/// Type ValueType with reference removed, if present
		using RemoveRefValueType = typename std::remove_reference<ValueType>::type;
		/// Type MutexType with reference removed, if present
		using RemoveRefMutexType = typename std::remove_reference<MutexType>::type;

		/**
		 * @brief Manages a mutex and gives pointer-like access to a value
		 * object.
		 * 
		 * @tparam LockType The type of the lock object that manages the
		 * mutex, example: std::lock_guard.
		 * @tparam Mode Determines the access mode of the Access
		 * object. Can be either AccessMode::ReadOnly or
		 * AccessMode::ReadWrite.
		 */
		template<template<typename> class LockType, AccessMode Mode>
		class Access
		{
			static_assert(!(LockTraits<LockType>::IsReadOnly && Mode==AccessMode::ReadWrite), "Cannot have ReadWrite access mode with ReadOnly lock. Check the value of LockTraits<LockType>::IsReadOnly if it exists.");

		private:
			/// ValueType with const qualifier if AccessMode is ReadOnly.
			using ConstIfReadOnlyValueType = typename std::conditional<Mode==AccessMode::ReadOnly, const RemoveRefValueType, RemoveRefValueType>::type;

		public:
			/// Pointer-to-const ValueType
			using ConstPointerType = const ConstIfReadOnlyValueType*;
			/// Pointer-to-const ValueType if Mode is ReadOnly, pointer to ValueType otherwise.
			using PointerType = ConstIfReadOnlyValueType*;
			/// Reference-to-const ValueType
			using ConstReferenceType = const ConstIfReadOnlyValueType&;
			/// Reference-to-const ValueType if Mode is ReadOnly, reference to ValueType otherwise.
			using ReferenceType = ConstIfReadOnlyValueType&;

			/**
			 * @brief Construct an Access object from a possibly const
			 * reference to the value object and any additionnal argument
			 * needed to construct the Lock object.
			 * 
			 * @tparam LockArgs Deduced from lockArgs.
			 * @param value Reference to the value.
			 * @param lockArgs Arguments needed to construct the lock object.
			 */
			template<typename... OtherLockArgs>
			Access(ReferenceType value, MutexType& mutex, OtherLockArgs&&... otherLockArgs):
				lock(mutex, std::forward<OtherLockArgs>(otherLockArgs)...),
				m_value(value)
			{}

			/**
			 * @brief Construct a read-only Access object from a const
			 * safe::Lockable object and any additionnal argument needed to
			 * construct the Lock object.
			 * 
			 * If needed, you can provide additionnal arguments to construct
			 * the lock object (such as std::adopt_lock). The mutex from the
			 * safe::Locakble object is already passed to the lock object's
			 * constructor though, you must not provide it.
			 * 
			 * @tparam OtherLockArgs Deduced from otherLockArgs.
			 * @param safe The const Lockable object to give protected access to.
			 * @param otherLockArgs Other arguments needed to construct the lock
			 * object.
			 */
			template<typename... OtherLockArgs>
			Access(const Lockable& lockable, OtherLockArgs&&... otherLockArgs):
				Access(lockable.m_value, lockable.m_mutex.get(), std::forward<OtherLockArgs>(otherLockArgs)...)
			{}

			/**
			 * @brief Construct a read-write Access object from a
			 * safe::Lockable object and any additionnal argument needed to
			 * construct the Lock object.
			 * 
			 * If needed, you can provide additionnal arguments to construct
			 * the lock object (such as std::adopt_lock). The mutex from the
			 * safe object is already passed to the lock object's constructor
			 * though, you must not provide it.
			 * 
			 * @tparam OtherLockArgs Deduced from otherLockArgs.
			 * @param safe The Lockable object to give protected access to.
			 * @param otherLockArgs Other arguments needed to construct the lock
			 * object.
			 */
			template<typename... OtherLockArgs>
			Access(Lockable& lockable, OtherLockArgs&&... otherLockArgs):
				Access(lockable.m_value, lockable.m_mutex.get(), std::forward<OtherLockArgs>(otherLockArgs)...)
			{}

			/**
			 * @brief Const accessor to the value.
			 * @return ConstPointerType Const pointer to the protected value.
			 */
			ConstPointerType operator->() const noexcept
			{
				return &m_value;
			}

			/**
			 * @brief Accessor to the value.
			 * @return ValuePointerType Pointer to the protected value.
			 */
			PointerType operator->() noexcept
			{
				return &m_value;
			}

			/**
			 * @brief Const accessor to the value.
			 * @return ConstValueReferenceType Const reference to the protected
			 * value.
			 */
			ConstReferenceType operator*() const noexcept
			{
				return m_value;
			}

			/**
			 * @brief Accessor to the value.
			 * @return ValueReferenceType Reference to the protected.
			 */
			ReferenceType operator*() noexcept
			{
				return m_value;
			}

			/// The lock that manages the mutex.
			mutable LockType<RemoveRefMutexType> lock;

		private:
			/// The protected value.
			ReferenceType m_value;
		};

		/// Reference-to-const ValueType.
		using ConstValueReferenceType = const RemoveRefValueType&;
		/// Reference to ValueType.
		using ValueReferenceType = RemoveRefValueType&;
		/// Reference to MutexType.
		using MutexReferenceType = RemoveRefMutexType&;

	public:
		template<template<typename> class LockType=DefaultReadOnlyLock>
		using ReadAccess = Access<LockType, AccessMode::ReadOnly>;
		template<template<typename> class LockType=DefaultReadWriteLock>
		using WriteAccess = Access<LockType, AccessMode::ReadWrite>;
		
		/**
		 * @brief Construct a Lockable object
		 */
		Lockable() = default;

		// Delete all copy and move operations, as they sometimes require protected access.
		Lockable(const Lockable&) = delete;
		Lockable(Lockable&&) = delete;
		Lockable& operator =(const Lockable&) = delete;
		Lockable& operator =(Lockable&&) = delete;

		/**
		 * @brief Construct a Lockable object with default construction of
		 * the mutex and perfect forwarding of the other arguments to
		 * construct the value object.
		 * 
		 * @tparam ValueArgs Deduced from valueArgs.
		 * @param valueArgs Perfect forwarding arguments to construct the value object.
		 * @param tag Indicates that the mutex should be default constructed.
		 */
		template<typename... ValueArgs>
		explicit Lockable(DefaultConstructMutex, ValueArgs&&... valueArgs):
			m_mutex(),
			m_value(std::forward<ValueArgs>(valueArgs)...)
		{}
		/**
		 * @brief Construct a Lockable object, perfect forwarding the first
		 * argument to construct the mutex and perfect forwarding the other
		 * arguments to construct the value object.
		 * 
		 * @tparam MutexArg Deduced from mutexArg.
		 * @tparam ValueArgs Deduced from valueArgs.
		 * @param valueArgs Perfect forwarding arguments to construct the value object.
		 * @param mutexArg Perfect forwarding argument to construct the mutex object.
		 */
		template<typename MutexArg, typename... ValueArgs>
		explicit Lockable(MutexArg&& mutexArg, ValueArgs&&... valueArgs):
			m_mutex(std::forward<MutexArg>(mutexArg)),
			m_value(std::forward<ValueArgs>(valueArgs)...)
		{}

		/**
		 * @brief Unsafe const accessor to the value. If you use this
		 * function, you exit the realm of safe!
		 * 
		 * @return ConstValueReferenceType Const reference to the value
		 * object.
		 */
		ConstValueReferenceType unsafe() const noexcept
		{
			return m_value;
		}
		/**
		 * @brief Unsafe accessor to the value. If you use this function,
		 * you exit the realm of safe!
		 * 
		 * @return ValueReferenceType Reference to the value object.
		 */
		ValueReferenceType unsafe() noexcept
		{
			return m_value;
		}

		/**
		 * @brief Accessor to the mutex.
		 * 
		 * @return MutexReferenceType Reference to the mutex.
		 */
		MutexReferenceType mutex() const noexcept
		{
			return m_mutex.get();
		}

	private:
		/// The helper object that holds the mutable mutex, or a reference to a mutex.
		trick::MutableIfNotReference<MutexType> m_mutex;
		/// The value to protect.
		ValueType m_value;
	};

	/**
	 * @brief Type alias for read-only Access.
	 * 
	 * @tparam LockableType The type of Lockable object to give read-only
	 * access to.
	 * @tparam LockType=DefaultReadOnlyLock The type of lock.
	 */
	template<typename LockableType, template<typename> class LockType=DefaultReadOnlyLock>
	using ReadAccess = typename LockableType::template ReadAccess<LockType>;

	/**
	 * @brief Type alias for read-write Access.
	 * 
	 * @tparam LockableType The type of Lockable object to give
	 * read-write access to.
	 * @tparam LockType=DefaultReadWriteLock The type of lock.
	 */
	template<typename LockableType, template<typename> class LockType=DefaultReadWriteLock>
	using WriteAccess = typename LockableType::template WriteAccess<LockType>;
}  // namespace safe
