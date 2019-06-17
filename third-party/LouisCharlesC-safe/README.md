# Make your multithreaded code safe and crystal clear!
*All variables shared by multiple threads should be wrapped in a safe object.*
## Contents
*safe* is a header-only library that helps you get your multithreaded code safe and understandable.  
This readme will walk you through the important features of the library using several code examples. Read on, and enjoy safe multithreading!
- [Background](#Background)
- [Overview](#Overview)
- [Basic interface](#Basic-interface)
- [Main features](#Main-features)
- [Higher-level interface](#Higher-level-interface)
## Background
*safe* aims at preventing common multithread mistakes by providing tools that complement the C++ standard library. Using *safe*, you will find it much easier to write correct multithreaded code, and your code will be easier to understand.

In multithreaded code, there is one rule: *avoid data races*. A data race exists when more than one thread access the same variable, at least one of these accesses modifies the variable and the accesses are not serialized. It is impossible to reason about the correctness of a program in the presence of data races. To avoid data races, accesses to a shared variable must be serialized using a synchronization mechanism like a mutex. Since C++11, the standard library provides such mechanisms, like std::mutex, along with tools to facilitate their usage, like std::lock_guard and std::unique_lock.

The standard's tools are sufficient to write safe multithreaded code, but it is all too easy to write code you think is safe but actually is not. Typical mistakes are: locking the wrong mutex and accessing the value object before locking (or after unlocking) the mutex.
## Overview
Two class templates are at the core of *safe*: Lockable and Access. Lockable objects pack a mutex and a value object together. Access objects act as a lock for the mutex and provide pointer-like access to the value object. You will learn more about Lockable and Access [here](#Basic-interface).

Here is why you want to use *safe*:
### Without safe
```c++
std::mutex fooMutex;
std::mutex barMutex;
std::string foo; // <-- do I need to lock a mutex to safely access this variable ?

{
	std::lock_guard<std::mutex> lock(fooMutex); // <-- is this the right mutex ?
	foo = "Hello, World!";
}

std::cout << foo << std::endl; // <-- unprotected access, is this intended ?
```
### With safe
```c++
using LockableString = safe::Lockable<std::string>; // type alisases will save you a lot of typing
std::mutex barMutex;
LockableString foo; // <-- value and mutex packaged together!

{
	safe::WriteAccess<LockableString> fooAccess(foo); // <-- right mutex: guaranteed!

	*fooAccess = "Hello, World!"; // access the value using pointer semantics: * and ->
} // from here, you cannot directly access the value anymore: jolly good, since the mutex is not locked anymore!

std::cout << foo.unsafe() << std::endl; // <-- unprotected access: clearly expressed!
```
### Vocabulary
* *safe*: the safe library.
* mutex: a mutex like std::mutex.
* Value object: whatever needs to be protected by the mutex.
* Lockable object: combines a value object and a mutex.
* Lock object: an object that manages a mutex using RAII like std::lock_guard and std::unique_lock.
* Access object: a Lock object that also gives pointer-like access to the value object.
* Access mode: Access objects can be created with read-write or read-only behavior. Read-only Access objects are especially useful to enforce the read-only nature of std::shared_lock (C++14) and boost::shared_lock_guard.
## Basic interface
### Include the header-only library
```c++
#include "safe/lockable.h"
```
### Replace your value and lockable objects by a Lockable object
```c++
// std::mutex mutex;
// std::string value;
safe::Lockable<std::string> value;
```
### Replace your lock objects by Access objects
Access is a class template nested in the Lockable class template. Two aliases exist in the safe namespace to easily construct Access objects for read-only and read-write access. These aliases are ReadAccess and WriteAccess. Use them exactly as you would use std::lock_guard or std::unique_lcok:
```c++
// std::lock_guard<std::mutex> lock(mutex);
safe::WriteAccess<safe::Lockable<std::string>> valueAccess(value);
```
or
```c++
safe::ReadAccess<safe::Lockable<std::string>> valueAccess(value);
```
### Access your value object through the Access objects using pointer semantics
You can now safely access the value object *through the Access object* without worrying about the mutex.
```c++
// value = "42";
*valueAccess = "42";
```
## Main features:
### 1. Safety and clarity
No more locking the wrong mutex, no more mistaken access outside the safety of a locked mutex. No more naked shared variables, no more plain mutexes lying around and no more *mutable* keyword (ever used a member mutex variable within a const-qualified member function ?).
### 2. Flexibility
#### Choose the mutex and lock that fit your need
The Lockable class template has a template parameter for the mutex: 
- use std::mutex, std::shared_mutex (C++17), name it!

The Access class template has a template parameter for the lock object: 
- use std::lock_guard, boost::shared_lock_guard, anything you want!
- you can use the lock you need for every Access object you construct.
#### Store the value object and mutex inside the Lockable object, or refer to existing objects
You can use any combination of reference and non-reference types for your Lockable objects:
```c++
safe::Lockable<int, std::mutex>;
safe::Lockable<int>; // equivalent to the above, as the second template parameter defaults to std::mutex
safe::Lockable<int&, std::mutex>;
safe::Lockable<int, std::mutex&>;
safe::Lockable<int&, std::mutex&>;
```
See [this section](#With-legacy-code) for an example of using reference types to deal with legacy code.
#### Flexibly construct the value object and mutex
Just remember: the first argument to a Lockable constructor is used to construct the mutex, the other arguments are used for the value object.

*Note: when constructing a Lockable object and the mutex is default constructed but the value object is not, you must pass the safe::default_construct_mutex_ tag. You can also pass an empty set of curly brackets {}.*

Examples:
```c++
std::mutex aMutex;

safe::Lockable<int, std::mutex> bothDefault; // lockable and value are default constructed
safe::Lockable<int, std::mutex&> noDefault(aMutex, 42); // lockable and value are initialized
safe::Lockable<int, std::mutex&> valueDefault(aMutex); // lockable is initialized, and value is default constructed
safe::Lockable<int, std::mutex> lockableDefaultTag(safe::default_construct_mutex, 42); // lockable is default constructed, and value is initialized
safe::Lockable<int, std::mutex> lockableDefaultBraces({}, 42);
```
#### Flexibly construct the Lock objects
The Access constructors have a variadic parameter pack that is forwarded to the Lock object's constructor. This can be used to pass in standard lock tags such as std::adopt_lock, but also to construct your custom locks that may require additionnal arguments than just the mutex.
```c++
safe::Lockable<int> value; // given a safe object
value.mutex().lock(); // with the mutex already locked...
// Because the mutex is already locked, you need to pass the std::adopt_lock tag to std::lock_guard when you construct your Access object.

// Fortunately, all arguments passed to the Safe::writeAccess() function are forwarded to the lock constructor.
safe::WriteAccess<safe::Lockable<int>> valueAccess(value, std::adopt_lock);
```
### 3. Even more safety!
#### Choose the access mode that suits each access
Once you construct a Lockable object, you fix the type of the mutex you will use. From there, you will create an Access object every time you want to operate on the value object. For each of these accesses, you can choose whether the access is read-write or read-only.
Use the ReadAccess and WriteAccess aliases to easily construct read-only and read-write Access objects.

#### Force read-only access with shared mutexes and shared_locks
Shared mutexes and shared locks allow multiple reading threads to access the value object simultaneously. Unfortunately, using only mutexes and locks, the read-only restriction is not guaranteed to be applied. That is, it is possible to lock a mutex in shared mode and write to the shared value. With *safe*, you can enforce read-only access when using shared locking by using ReadAccess objects.
### 4. Compatibility
#### With legacy code
You can use *safe* with old-style unsafe code that uses the out-of-fashion separate mutex and value idiom. Imagine you are provided with the typical mutex and int. *safe* allows you to wrap these variables, without having to modify the existing code. Enjoy the safety and avoid the headaches:
```c++
std::mutex mutex;
int value;

// Wrap the existing variables
safe::Lockable<int&, std::mutex&> lockableValue(mutex, value);
// do not use mutex and unsafeValue directly from here on!
```
#### With code from the future
*safe* is written in C++11, but it is fully compatible with mutexes and locks from different sources like C++14's std::shared_lock and C++17's std::shared_mutex, thanks to template parameters. Of course, you can also use boost::shared_lock_guard and your own custom mutexes and locks.
#### With standard uses of mutexes and locks
The mutex is accessible from the Lockable object through accessor functions, and the lock object is a public member of the Access class. Anything you can do with your typical mutexes and locks you can do with *safe*. 

For example, *safe* can seamlessly be used with std::condition_variable:
```c++
std::condition_variable cv;
safe::Lockable<int> value;

safe::WriteAccess<safe::Lockable<int>, std::unique_lock> valueAccess(value);
cv.wait(valueAccess.lock);
```
## Advanced use cases
### Enforcing read-only access
Here is the full declaration of the Access class:
```c++
template<template<typename> class LockType, AccessMode Mode> class Access;
```
Don't be daunted be the looks of the first template parameter, it only means that the LockType must be a class template with one template parameter.

The second template parameter is the customization point you can use to change the behavior of you Access objects.  
As already mentioned, the AccessMode template parameter defines the access mode for the Access objects: either read-write or read-only. A trait class exists in safe/traits.h to tell *safe* whether the lock is read-write or read-only:
- If no specialization of the type trait exists for LockType, the lock is read-write.
- If a specialization exists, it must declare the IsReadOnly boolean variable.
  - If IsReadOnly is true, the lock is read-only: using Mode = AccessMode::ReadWrite will fail to compile.
  - If IsReadOnly is false, the lock is read-write.

It is useful to declare shared locks as read-only. Here is how to do it (you will find this exact code snippet in safe/traits.h):
```c++
template<>
struct LockTraits<std::shared_lock>
{
	static constexpr bool IsReadOnly = true;
};
```
## Higher-level interface
Here is a multithreading utility class template that I built on top of *safe*'s basic interface: Safe.
### Safe
The Safe class template is a bit higher level than Lockable; it offers a nice interface to safely manage your variables in multithreaded code.

Safe's interface provides thread-safe read and write to the variable. The **copy()** and **emplace()** member functions are your basic tools to access the variable. *copy()* returns a copy of the variable and *emplace()* replaces its content.

```c++
safe::Safe<std::vector<std::string>> vec;
vec = std::vector<std::string>(2, "bar"); // assign new vector
auto copy = vec.copy(); // get a copy
```

Now imagine your variable is an std::vector and you are only interested in knowing its size. Are your going to copy the whole vector only to call size() on it ? Of course not. You will use the **readAccess()** member function provided by the Safe class! *readAccess()* returns a ReadAccess object to the variable, allowing you to perform any operation you want on it without incurring the cost of a copy. Likewise, if you only want to modify the first element of the vector, it would be a shame to replace it as a whole using the *emplace()* function. It is much better to use the **writeAccess()** member function to get a WriteAccess object to the variable. Using *readAccess()* and *writeAccess()*, you can inspect and alter any part of the variable in isolation.

```c++
vec.writeAccess()->front() = "foo"; // replace front only
assert(vec.readAccess()->size() == 2); // check size
```

Thread-safe *copy()*, *emplace()*, *readAccess()* and *writeAccess()*: this is all the Safe class is about.
##### Specialization for Safe\<std::shared_ptr\>
Safe objects of std::shared_ptr are interesting because the reference counting apparatus of the shared pointer allows for a very nice optimization: copy-on-write. For this class template specialization, calls to *copy()* do not make a copy of the pointed-to variable, but they return a const std::shared_ptr to the variable. From there, a copy of the variable *may* happen, but only if this returned shared_ptr still exists when the next call to *assign()* or *writeAccess()* happens. That is:
```c++
safe::Safe<std::shared_ptr<std::vector<std::string>>> vec(2, "bar"); // std::shared_ptr managed internally
{
	auto view = vec.copy(); // no copy, view is a std::shared_ptr<const std::vector<std::string>>, notice the const!
	assert(view->front() == "bar");
} // view destroyed
(*vec.writeAccess())->front() = "foo";
assert((*vec.readAccess())->front() == "foo");
```
but:
```c++
safe::Safe<std::shared_ptr<std::vector<std::string>>> vec(2, "bar");
auto view = vec.copy(); // again, no copy here
assert(view->front() == "bar");
(*vec.writeAccess())->front() = "foo"; // content of vec is copied into a new std::shared_vector, then the first element is modified
assert(view->front() == "bar"); // this is still true!
assert((*vec.readAccess())->front() != "foo"); // see ? vec does hold a difference instance than view
```
Calls to *readAccess()* never cause copies because the mutex is locked while the ReadAccess object exists, delaying calls to any other member function.
