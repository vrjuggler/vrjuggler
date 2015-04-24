# Porting from VR Juggler 2.0 to 2.2 #

## Configuration Changes ##

**IMPORTANT**: Config elements for keyboard keys should use the symbolic identifiers instead of the integer constants. Support for new keys has been added in VR Juggler 2.2, and as a result, the integer values for the keys have changed. For example, the following two config elements demonstrate how the integer values are replaced with symbolic identifiers:

```
<key_modifier_pair name="Forward" version="1">
   <modifier_key>0</modifier_key>
   <key>15</key>
</key_modifier_pair>
```

```
<key_modifier_pair name="Forward" version="1">
   <modifier_key>NONE</modifier_key>
   <key>KEY_8</key>
</key_modifier_pair>
```

For better or for worse, VRJConfig will not perform this change automatically, but an XSL Transform has been written to make the process at least somewhat automated. The transform is in the file [modules/vrjuggler/data/xslt/key\_sym\_2\_0.xsl](http://code.google.com/p/vrjuggler/source/browse/juggler/branches/2.2/modules/vrjuggler/data/xslt/key_sym_2_0.xsl) (or `$VJ_BASE_DIR/share/vrjuggler-2.2/data/xslt/key_sym_2_0.xsl`), and it should be applied to all VR Juggler 2.0 configuration files. This can be done before or after applying other automatic configuration updates using VRJConfig. Instructions for using this transform can be found in the file after the copyright notice. The sample configuration files that come with VR Juggler [have been updated](http://code.google.com/p/vrjuggler/source/detail?r=19831).

For other configuration changes, VRJConfig can be used to migrate config elements to newer versions automatically, and it is recommended that all VR Juggler 2.0 configuration files be loaded into VRJConfig and saved using the "Save All" button. For sites that need to maintain configuration files for both versions, the VR Juggler 2.0 files should be copied into a new location for use by VR Juggler 2.2.

## VPR Interface Changes ##

**IMPORTANT**: The VPR version number can be used at compile time to make decisions about which version of the API will be needed. The version number is available using the preprocessor symbol `__VPR_version`, defined in `vpr/vprParam.h`. The encoding is based on the major, minor, and patch version numbers using the scheme `MMMmmmPPP` (three digits for the major version number, three for the minor, and three for the patch) with left zero padding. For example, VPR version 1.1.32 is encoded as 1001032. Code examples below will make use of `__VPR_version` to demonstrate how to write code that works with VPR 1.0 and 2.0.

The VR Juggler Portable Runtime (VPR) has undergone extensive work to improve error reporting through the use of exceptions (derived from `vpr::Exception`) rather than the clumsier use of `vpr::ReturnStatus`. Methods in VPR 1.0 classes that used to return `vpr::ReturnStatus` (generally) now have a `void` return type and report errors by throwing exceptions. In some cases, the use of `vpr::ReturnStatus` as the return type was replaced by returning a `bool` and throwing exceptions to report critical errors. This was only done for cases where an operation can "fail" under standard usage; exceptions are used to report _exceptional_ cases.

Ultimately, VPR 2.0 does not use `vpr::ReturnStatus`, and the class is basically considered deprecated. It is not scheduled to be removed, but as VR Juggler 2.x evolves, the internal use of `vpr::ReturnStatus` will eventually be eliminated. While the API changes may be unpleasant to deal with, the end result will be worth it. Exceptions allow for much cleaner algorithm implementations because the error handling can usually be separated from the algorithm. Code flow within a try/catch block is almost always easier to follow and understand as a result. Furthermore, exceptions are a much more expressive and scalable way of reporting errors than error codes.

In general, most VR Juggler applications will probably not be affected by the VPR 2.0 API changes. Those that use sockets or create and manage threads will have to be updated, though. The most likely place where users would be tripped up is by the change in return type for `vpr::ReadableObject::readObject()` and `vpr::WritableObject::writeObject()`. In VPR 2.0, these methods have a `void` return type, and user implementations must throw exceptions of type `vpr::IOException` (or a subclass thereof) to indicate errors in object serialization and object de-serialization. See **Application-Specific Cluster Data** below for more information.

### I/O Classes ###

All the classes in VPR related to input and output throw exceptions derived from `vpr::Exception` instead of using `vpr::ReturnStatus`. Every method that throws an exception is documented to identify which exceptions are thrown and under what circumstances a given exception will be thrown. The I/O classes that have been affected by this change are as follows:

  * `vpr::BlockIO`
  * `vpr::Selector`
  * `vpr::SerialPort` (see `vpr::SerialPortImplTermios` and `vpr::SerialPortImplWin32`)
  * `vpr::ObjectReader`
  * `vpr::ObjectWriter`
  * `vpr::BufferedObjectReader`
  * `vpr::BufferedObjectWriter`
  * `vpr::XMLObjectReader`
  * `vpr::XMLObjectWriter`
  * `vpr::ReadableObject`
  * `vpr::WritableObject`
  * `vpr::Socket` (see `vpr::Socket_t<T>`, `vpr::SocketImplBSD`, and `vpr::SocketImplNSPR`)
  * `vpr::SocketDatagram` (see `vpr::SocketDatagram_t<T>`, `vpr::SocketDatagramImplBSD`, and `vpr::SocketDatagramImplNSPR`)
  * `vpr::SocketStream` (see `vpr::SocketStream_t<T>`, `vpr::SocketStreamImplBSD`, and `vpr::SocketStreamImplNSPR`)
  * `vpr::InetAddr` (see `vpr::InetAddrBSD` and `vpr::InetAddrNSPR`)
  * `vpr::FileHandle` (see `vpr::FileHandle_t<T>` and `vpr::FileHandleImplUNIX`)

#### I/O Exceptions ####

The exceptions thrown by I/O methods are the following (indentation indicates class hierarchy):

  * `vpr::Exception` (declared in `vpr/Util/Exception.h`)
    * `vpr::IOException` (declared in `vpr/IO/IOException.h`)
      * `vpr::EOFException` (declared in `vpr/IO/EOFException.h`)
      * `vpr::SocketException` (declared in `vpr/IO/Socket/SocketException.h`)
        * `vpr::ConnectionAbortedException` (declared in `vpr/IO/Socket/ConnectionAbortedException.h`)
        * `vpr::ConnectionRefusedException` (declared in `vpr/IO/Socket/ConnectionRefusedException.h`)
        * `vpr::ConnectionResetException` (declared in `vpr/IO/Socket/ConnectionResetException.h`)
        * `vpr::NoRouteToHostException` (declared in `vpr/IO/Socket/NoRouteToHostException.h`)
        * `vpr::UnknownHostException` (declared in `vpr/IO/Socket/UnknownHostException.h`)
      * `vpr::TimeoutException` (declared in `vpr/IO/TimeoutException.h`)
      * `vpr::WouldBlockException` (declared in `vpr/IO/WouldBlockException.h`)

#### I/O Object Read/Write Call Signature ####

Now that the VPR I/O classes throw exceptions to report errors, we had the opportunity to make the signature of the read/write method calls more "friendly." Previously, the number of bytes read or written was reported through a method pass-by-reference parameter, a technique that can be cumbersome and awkward--especially for people used to the behavior of the `read(2)` and `write(2)` system calls.

Now, the methods `vpr::BlockIO::read()`, `vpr::BlockIO::readn()`, and `vpr::BlockIO::write()` return the number of bytes read or written "out the back." This same change was made to `vpr::Socket::recv()`, `vpr::Socket::recvn()`, `vpr::Socket::send()`, `vpr::SocketDatagram::recvfrom()`, and `vpr::SocketDatagram::sendto()`. This change was made in VPR 1.1.42 (encoded as 1001042), and the use of exception handling for I/O classes was introduced in version 1.1.5 (1001005). The following shows an example of how to read data from a socket that will work with VPR 1.0 and with 1.1.42 or newer.

```
#include <vpr/vprParam.h>
#include <vpr/IO/IOException.h>


vpr::Uint32 MyClass::readString(std::string& buffer, const vpr::Uint32 amount)
{
   vpr::Uint32 bytes_read;

#if __VPR_version < 1001005
   vpr::ReturnStatus status = mSocket->read(buffer, amount, bytes_read);
   if ( ! status.success() )
   {
      // Report the error.
   }
#else
   try
   {
      // NOTE: This read() signature is for VPR 1.1.42 and newer.
      bytes_read = mSocket->read(buffer, amount);
   }
   catch (vpr::IOException& ex)
   {
      // Report the error.
      // For example, just print it to stderr:
      std::cerr << "Read failed!\n" << ex.what() << std::endl;
   }
#endif

   return bytes_read;
}
```

The above example will not work with VPR versions 1.1.5 through 1.1.41 because the signature of `vpr::BlockIO::read()` still used the by-reference mechanism for returning the number of bytes read. It is possible to support all VPR versions between 1.0 and 2.0, but that would complicate this example and is probably unnecessary for most users. The change would be to add an extra conditional for `__VPR_version` values less than 1001042 in the `try` block.

#### Leveraging Exception Handling ####

The example shown above leads us to an important design decision. Given that VPR now reports exceptional states using exceptions, it is much easier for user code to take advantage of exception handling for sophisticated error reporting and recovery. In the above example, the read failure may already be reported by `MyClass::readString()` by throwing an exception. In that case, it is worth considering whether the VPR exception thrown by `vpr::SocketStream::read()` should be allowed to propagate up a level in the call stack rather than throwing a different, custom exception type. The example above shows catching `vpr::IOException`, but there are a variety of more specific exception types derived from `vpr::IOException` (see the earlier description of the I/O exception types). The handling of each exceptional case is likely to vary, and throwing away the information provided by VPR through the exception type thrown may not always be a wise decision.

Another important thing to know is that, in general, **VPR no longer prints error messages on its own**. Instead, it builds an error message and puts it in the exception description. There are still some parts of the code where the error message is printed before the exception is thrown, but these will be removed eventually. Thus, **it is up to calling code to decide whether to print an error message**.

#### Application-Specific Cluster Data ####

The changes to the return type of `vpr::ReadableObject::readObject()` and `vpr::WritableObject::writeObject()` impact code written for sharing application-specific data in a cluster. These changes were made as part of VPR 1.1.5, so all code built against versions of VPR prior to this must return `vpr::ReturnStatus` in their override of these methods. For VPR 1.1.5 and beyond, the return type must be `void`. The following shows an example of how to write code that will work with VPR 1.0, 1.1, and 2.0. Here, the VPR 1.1.5 version number is encoded as 1001005.

```
#include <vpr/vprParam.h>
#include <plugins/ApplicationDataManager/UserData.h>

class MyType : public vpr::SerializableObject
{
public:
#if __VPR_version < 1001005
   vpr::ReturnStatus
#else
   void
#endif
   readObject(vpr::ObjectReader* reader)
   {
      // Read object from reader...
#if __VPR_version < 1001005
      return vpr::ReturnStatus();
#endif
   }

#if __VPR_version < 1001005
   vpr::ReturnStatus
#else
   void
#endif
   writeObject(vpr::ObjectWriter* writer)
   {
      // Write object to writer...
#if __VPR_version < 1001005
      return vpr::ReturnStatus();
#endif
   }
};
```

The methods of `vpr::ObjectWriter` and `vpr::ObjectReader` throw exceptions if anything goes wrong while serializing or de-serializing an object. The exception will be of type `vpr::IOException` or `vpr::EOFException` (a subclass of `vpr::IOException`). Code that would be operating on objects of type `vpr::ReadableObject` or `vpr::WritableObject` must be written to handle these exceptions when calling `vpr::ReadableObject::readObject()` and `vpr::WritableObject::writeObject()`. Thus, implementations of these methods do not need to worry about catching exceptions thrown by `vpr::ObjectReader` or `vpr::ObjectWriter`. Of course, there may be a desire to catch these errors during the serialization and/or de-serialization process. In this case, the programmer should re-throw the exception after handling it so that the higher level code knows that something went wrong. An example of this is shown below:

```
#include <vector>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/EOFException.h>
#include <plugins/ApplicationDataManager/UserData.h>

class MyType : public vpr::SerializableObject
{
public:
   void readObject(vpr::ObjectReader* reader)
   {
      try
      {
         const vpr::Uint8 val_count = reader->readUint8();
         std::vector<vpr::Uint32> values(val_count);
         for ( vpr::Uint8 i = 0; i < val_count; ++i )
         {
            values[i] = reader->readUint32();
         }

         // Wait to replace mValues until we know that we have
         // read everything.
         mValues = values;
      }
      catch (vpr::EOFException& ex)
      {
         std::cerr << "Ran out of data unexpectedly:\n"
                   << ex.what() << std::endl;
         throw;
      }
      catch (vpr::IOException& ex)
      {
         std::cerr << "I/O error occurred during de-serialization:\n"
                   << ex.what() << std::endl;
         throw;
      }
   }

   // writeObject() implementation omitted

private:
   std::vector<vpr::Uint32> mValues;
};
```

Of course, the exception handling in the above example is not very interesting since it just prints a different error message based on the exception type. One can imagine more complex uses, however, and the point of the above example is to demonstrate what is possible rather than to give a definitive example of what to do in all cases.

### Querying Environment Variables ###

The helper functions `vpr::System::getenv()` and `vpr::System::setenv()` have had their return type changed from `vpr::ReturnStatus` to `bool`. Since these methods either succeed or fail, using `vpr::ReturnStatus` was overkill. This change was made in VPR 1.1.35 (encoded as 1001035). The following shows an example of how to use `vpr::System::getenv()` in a way that will work with VPR 1.0, 1.1, and 2.0:

```
#include <vpr/vprParam.h>
#include <vpr/System.h>

void f()
{
   const std::string envvar("SOME_VAR");
   std::string value;
#if __VPR_version < 10010135
   const bool result = vpr::System::getenv(envvar, value).success();
#else
   const bool result = vpr::System::getenv(envvar, value);
#endif

   if ( result )
   {
      // Do something with value ...
   }
}
```

Another approach that does not require the version testing would be the following:

```
#include <vpr/System.h>

void f()
{
   std::string value;
   vpr::System::getenv("SOME_VAR", value);

   if ( ! value.empty() )
   {
      // Do something with value ...
   }
}
```

### Threads ###

The use of `vpr::ThreadFunctor` has been replaced by the much more powerful and flexible [Boost.Function](http://www.boost.org/doc/html/function.html). In VPR 1.0, threads were created using one of the following approaches:

```
// Using a member function.
MyType* my_obj = new MyType();
vpr::ThreadMemberFunctor<MyType>* functor =
   new vpr::ThreadMemberFunctor<MyType>(my_obj,
                                        &MyType::threadFunc,
                                        NULL);
vpr::Thread* thread = new vpr::Thread(functor);
```

```
// Using a non-member function or a static member function.
vpr::ThreadNonMemberFunctor* functor =
   new vpr::ThreadNonMemberFunctor(threadFunc, NULL);
vpr::Thread* thread = new vpr::Thread(functor);
```

The signature of the thread function passed to the functor had to have a `void` return type and take a single `void*` parameter (that often went unused).

In VPR 2.0, the thread function can take any number of parameters, though it must still have a `void` return type. (The `boost::function<T>` type instantiation is `boost::function<void ()>`.) This is possible by using [boost::bind()](http://www.boost.org/libs/bind/bind.html). `boost::bind()` must also be used for creating thread functors for member functions. The following shows how to do the equivalent of the above VPR 1.0 thread creation mechanisms:

```
// Using a member function.
MyType* my_obj = new MyType();
vpr::Thread* thread =
   new vpr::Thread(boost::bind(&MyType::threadFunc, my_obj));
```

```
// Using a non-member function.
vpr::Thread* thread = new vpr::Thread(threadFunc);
```

Clearly, this new approach is much simpler and avoids a potential (and all-too-common) memory leak wherein the heap-allocated `vpr::ThreadFunctor` instance would not get deleted.

As noted, `boost::bind()` can be used to allow for thread functors that take any number of arguments. For example, consider these (somewhat contrived) cases:

```
void threadFunc(const int val,
                std::vector<SomeClass>& vecRef)
{
   vecRef.reserve(val);
   for ( int i = 0; i < val; ++i )
   {
      vecRef.push_back(SomeClass(i));
   }
}

void createThread(const int val)
{
   std::vector<SomeClass> vec;
   vpr::Thread* thead =
      new vpr::Thread(boost::bind(threadFunc, val,
                                  boost::ref(vec));
   thread->join();
   std::cout << vec.size() << std::endl;
}
```

These changes were made in VPR 1.1.16 (encoded as 1001016). Furthermore, in VPR 1.1.31, the use of `vpr::ReturnStatus` in `vpr::Thread` was replaced by exception handling, and `vpr::Thread::isValid()` was removed. The following example shows how one might write code to work with VPR 1.0, 1.1, and 2.0. This could be simplified by eliminating VPR 1.1 from the mix and comparing `__VPR_version` only against 2000000 (the encoding of the VPR 2.0.0 version). In that case, the clause for versions between 1.1.16 and 1.1.31 in `MyClass::startThread()` would be unnecessary.

```
#include <vpr/vprParam.h>
#include <vpr/Thread/Thread.h>
#if __VPR_version >= 1001016
#  include <vpr/Util/Exception.h>
#endif

class MyClass
{
public:
   MyClass()
      : mRunning(false)
#if __VPR_version < 1001016
      , mThreadFunctor(NULL)
#endif
      , mThread(NULL)
   {
   }

   ~MyClass()
   {
      if ( mRunning )
      {
         stopThread();
      }

      if ( NULL != mThread )
      {
         delete mThread;
         mThread = NULL;
      }

#if __VPR_version < 1001016
      if ( NULL != mThreadFunctor )
      {
         delete mThreadFunctor;
         mThreadFunctor = NULL;
      }
#endif
   }

   bool startThread()
   {
#if __VPR_version < 1001016
      mThreadFunctor =
         new vpr::ThreadMemberFunctor<MyClass>(this,
                                               &MyClass::run,
                                               NULL);
      mThread = new vpr::Thread(mThreadFunctor);

      return mThread->isValid();
#else
      bool started(false);
      try
      {
         mThread =
            new vpr::Thread(boost::bind(&MyClass::run,
                                        this));

         // Handle versions between 1.1.16 and 1.1.31.
#if __VPR_version < 1001031
         started = mThread->isValid();
#else
         started = true;
#endif
      }
      catch (vpr::Exception& ex)
      {
         std::cerr << ex.what() << std::endl;
      }

      return started;
#endif
    }

   void stopThread()
   {
      mRunning = false;
      if ( NULL != mThread )
      {
         mThread->join();
      }
   }

private:
   void run(
#if __VPR_version < 1001016
      void*
#endif
   )
   {
      mRunning = true;
      while ( mRunning )
      {
         // Do work...
      }
   }

	bool mRunning;
#if __VPR_version < 1001016
   vpr::ThreadMemberFunctor<MyClass>* mThreadFunctor;
#endif
   vpr::Thread* mThread;
};
```

### Synchronization Primitives ###

The syntactic and semantic behavior of the VPR synchronization primitives have changed to eliminate the use of `vpr::ReturnStatus` in favor of using exceptions. Now, locking and unlocking operations are guaranteed to succeed as long as the synchronization primitive object was constructed successfully. All the classes in VPR related to thread synchronization throw exceptions derived from `vpr::Exception` instead of using `vpr::ReturnStatus`. Every method that throws an exception is documented to identify which exceptions are thrown and under what circumstances a given exception will be thrown. The synchronization classes that have been affected by this change are as follows:

  * `vpr::Mutex`
  * `vpr::Guard<T>`
  * `vpr::CondVar`
  * `vpr::Semaphore`
  * `vpr::RWMutex`
  * `vpr::RWGuard<T>`

Previously, operations such as `vpr::Mutex::acquire()` and `vpr::Mutex::release()` returned a `vpr::ReturnStatus` to indicate whether the lock/unlock operation succeeded. Now, these methods have a `void` return type. If locking/unlocking should fail due to an exceptional case, then an exception is thrown.

These changes were made in the following VPR versions:

  * 1.1.28: Change `vpr::Mutex` to use exceptions instead of `vpr::ReturnStatus`
  * 1.1.29: Change `vpr::CondVar` to use exceptions instead of `vpr::ReturnStatus`
  * 1.1.30: Change `vpr::Semaphore` to use exceptions instead of `vpr::ReturnStatus`
  * 1.1.31: Change `vpr::Mutex` and `vpr::Semaphore` to report deadlock detection
  * 1.1.33: Change `vpr::RWMutex` to use exceptions instead of `vpr::ReturnStatus` and to report deadlock detection

To write code that works with VPR 1.0 and 2.0, it is recommended that users test `__VPR_version` against either 1001033 (the encoding of 1.1.33) or 2000000 (the encoding of 2.0.0). Since most of these version increments were made in quick succession, it is highly unlikely that code would need to work with all the different VPR 1.1 versions.

#### Synchronization Exceptions ####

The exceptions thrown by synchronization methods are the following (indentation indicates class hierarchy):

  * `vpr::Exception` (declared in `vpr/Util/Exception.h`)
    * `vpr::ResourceException` (declared in `vpr/Util/ResourceException.h`)
    * `vpr::LockException` (declared in `vpr/Sync/LockException.h`)
      * `vpr::DeadlockException` (declared in `vpr/Sync/DeadlockException.h`)

The exception type `vpr::ResourceException` is thrown by the constructor of each of the various synchronization classes if allocation of the platform-specific synchronization primitive should fail. In that case, there is no way for the VPR synchronization object to be used, and all locking and unlocking methods would fail.

The exception type `vpr::LockException` is thrown if a lock or unlock operation fails due to misuse of the object. For example, if `vpr::Mutex::release()` is invoked on an object from a thread that is not currently holding the mutex lock, then that is a misuse of the mutex object, and `vpr::LockException` is thrown. Similarly, if `vpr::CondVar::signal()` or `vpr::CondVar::broadcast()` is invoked on a condition variable object that is not currently locked, that violates the contract of those methods, and `vpr::LockException` is thrown.

The exception type `vpr::DeadlockException` is thrown when an attempt to lock a synchronization object would cause a thread to deadlock. This occurs when the same thread tries to lock the same mutex twice. The operating system detects this case and reports it, and VPR synchronization objects translate this error code into a `vpr::DeadlockException` that is thrown. Previously, this case was only "reported" through an assertion failure when building with `_DEBUG` defined, so this change is definitely a big improvement.

#### Return Type Changes ####

The synchronization classes all have a non-blocking lock acquisition method named `tryAcquire()`, and many have `tryAcquireRead()` and `tryAcquireWrite()` methods. In VPR 1.0, these methods return a `vpr::ReturnStatus` object to indicate whether or not the lock was acquired. Since this operation either succeeded in acquiring the lock or failing to do so, the return type was changed to `bool` to allow simplification of user-level code. The following example shows a simple use of `vpr::Guard<vpr::Mutex>` and `vpr::Mutex::tryAcquire()` that will compile with VPR 1.0, 1.1, and 2.0:

```
#include <boost/noncopyable.hpp>
#include <vpr/vprParam.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>

// The class is non-copyable because vpr::Mutex is non-copayble.
class MyClass : boost::noncopyable
{
public:
   void changeData(const int newValue)
   {
      // Acquires the mutex on construction; releases it on destruction.
      vpr::Guard<vpr::Mutex> g(mMutex);
      mData = newValue;
   }

   bool useData()
   {
#if __VPR_version < 1001033
      const bool locked = mMutex.tryAcquire().success();
#else
      const bool locked = mMutex.tryAcquire();
#endif

      if ( locked )
      {
         const int cur_value(mData);
         mMutex.release();
         // Do work based on cur_value ...
      }

      return locked;
   }

private:
   vpr::Mutex mMutex;
   int mData;
};
```

We could take the above example a little farther by handling the case of deadlock detection in the implementation of `MyClass::changeData()`, as shown below (note that `vpr/Sync/DeadlockException.h` would need to be included):

```
   void changeData(const int newValue)
   {
#if __VPR_version >= 1001033
      try
      {
         // Acquires the mutex on construction; releases it on destruction.
         // If an exception is thrown during mutex acquisition, g is
         // automatically destroyed, but no change is made to the state of
         // mMutex.
         vpr::Guard<vpr::Mutex> g(mMutex);
         mData = newValue;
      }
      // If deadlock was detected, then this thread is already holding a
      // lock on mMutex, so it is still save to change mData.
      catch (vpr::DeadlockException& ex)
      {
         mData = newValue;
      }
#else
      // Acquires the mutex on construction; releases it on destruction.
      vpr::Guard<vpr::Mutex> g(mMutex);
      mData = newValue;
#endif
   }
```

This example is rather contrived and is intended to demonstrate what options exist for handling deadlock detection and how `vpr::Guard<vpr::Mutex>` will behave in this context.

## `cluster::ApplicationData` Construction and Initialization ##

[Application-specific shared data types](http://developer.vrjuggler.org/docs/vrjuggler/2.2/programmer.guide/programmer.guide/ch06.html#section.cluster.UserData) may now be derived from `cluster::ApplicationData` directly. In VR Juggler 2.0, this was not (supposed to be) allowed, but it may have occurred. In cases when it did, such code has to be updated for a signature change to the `cluster::ApplicationData` constructor and a change in initialization behavior.

The construction and initialization of `cluster::ApplicationData` are now separate operations. The constructor takes no arguments, and the new `init()` method takes type-specific GUID and host name or IP address of the managing machine as its parameters. This allows objects to be default constructed, a handy feature for use of STL containers or for cases where the programmer does not want to use a pointer type for a class data member. To put it another way, it is no longer necessary to use `operator new` in the `vrj::App::init()` override to construct and initialize the `cluster::ApplicationData` subclass in one step.

This change was made in Gadgeteer 1.1.16. This value is encoded in `__GADGET_version` (defined in `gadget/gadgetParam.h`) as 1001016.

## Device Driver Base Class ##

Device driver plug-ins used by Gadgeteer must be derived from a _known_ instantiation of the type `gadget::InputMixer<S,T>` in order for input device sharing in a cluster configuration to work. That is, Gadgeteer knows about specific instantiations of this type for various input device categories (position, position and digital, digital and analog, etc.). If an unknown instantiation of `gadget::InputMixer<S,T>` is used, then Gadgeteer will not be able to share the input device data in a cluster configuration, but this will not be known until run time, usually when a crash occurs.

To address this problem, the known instantiations of `gadget::InputMixer<S,T>` are now enumerated in the new header file [gadget/Type/InputBaseTypes.h](http://code.google.com/p/vrjuggler/source/browse/juggler/branches/2.2/modules/gadgeteer/gadget/Type/InputBaseTypes.h). This was added to the Juggler 2.2 branch in Gadgeteer version 1.1.28 (encoded in `__GADGET_version` as 1001028) and to the trunk in Gadgeteer version 1.3.8 (1003008). It was also added to the Juggler 2.0 branch in [r20304](https://code.google.com/p/vrjuggler/source/detail?r=20304).

One of the types listed in `gadget/Type/InputBaseTypes.h` must be used as the base type for all device drivers, though this requirement is enforced only as a _convention_. It is still possible to derive from `gadget::InputMixer<S,T>`, though doing so is strongly discouraged. Using one of the types defined in `gadget/Type/InputBaseTypes.h` makes the code more readable and far more reliable.

## Shared Pointer Usage in VR Juggler ##

Instances of the VR Juggler classes `vrj::User`, `vrj::GlWindow`, and `vrj::Display` are now handled as shared pointers (`vrj::UserPtr`, `vrj::GlWindowPtr`, and `vrj::DisplayPtr` respectively) rather than as raw pointers. User-level code (i.e., application objects) would only deal with these types when requesting the user data object from the OpenGL Draw Manager or the list of users from the kernel. For more information, see the VR Juggler C++ API reference for the classes `vrj::GlDrawManager`, `vrj::GlUserData`, and `vrj::Kernel`.

## Using Flagpoll ##

The `*-config` Perl scripts (`vpr-config`, `vrjuggler-config`, etc.) have been removed and replaced by the use of the [Flagpoll](http://code.google.com/p/flagpoll/) tool. There are two changes to make if using [Doozer](http://sourceforge.net/projects/doozer) for building VR Juggler applications:

  1. Install Doozer 2.1.4
  1. Make a small change to the application makefile(s)

Basically, there are two steps involved to update a Doozer-based makefile:

  1. Remove the line that includes `$(VJ_BASE_DIR)/share/vrjuggler/vrj.appdefs.mk`
  1. Add the following lines:

```
DZR_BASE_DIR=  $(shell flagpoll doozer --get-prefix)
include $(DZR_BASE_DIR)/ext/vrjuggler/dzr.vrjuggler.mk
```

An example of the change to the Doozer-bsaed makefile is shown below as a unified diff:

```
--- Makefile.in (revision 19219)
+++ Makefile.in (revision 19220)
@@ -37,7 +37,8 @@
 srcdir=		@srcdir@
 SRCS=		MPApp.cpp main.cpp
 
-include $(VJ_BASE_DIR)/share/vrjuggler/vrj.appdefs.mk
+DZR_BASE_DIR=	$(shell flagpoll doozer --get-prefix)
+include $(DZR_BASE_DIR)/ext/vrjuggler/dzr.vrjuggler.mk
 include $(DZR_BASE_DIR)/ext/vrjuggler/dzr.vrjuggler.glapp.mk
 
 # -----------------------------------------------------------------------------
```

It is possible to use Flagpoll with VR Juggler 2.0.2 and newer. See the [wiki page about VR Juggler 2.0 and Flagpoll](FlagpollWithVrJugglerTwoZero.md) for more information.

## Versioned Installations ##

A great deal of work has gone into making it possible and easy to have multiple VR Juggler installations installed in parallel. [The details](PackageManagement.md) are far beyond the scope of this page. For non-Windows installations of VR Juggler, the major differences are the following:

  * Header files are now in versioned subdirectories of `$VJ_BASE_DIR`/include. For example, the VR Juggler header subdirectory `vrj` is in `$VJ_BASE_DIR/include/vrjuggler-2.2`, and the Gadgeteer header subdirectory `gadget` is in `$VJ_BASE_DIR/include/gadgeteer-1.2`. The header directories are _not_ versioned in Windows installations.
  * Data files are in versioned directories. For example, the VR Juggler sample configuration files are now in `$VJ_BASE_DIR/share/vrjuggler-2.2` rather than `$VJ_BASE_DIR/share/vrjuggler`. (The default search path for VR Juggler configration files knows to look in the versioned directory, so specifying `standalone.jconf` without any path prefix will work just as it does with VR Juggler 2.0.)

For all operating systems, the shared libraries now have versioning in their base name. For example, `libvrj.so` in VR Juggler 2.0 is now `libvrj-2_2.so` in VR Juggler 2.2. (Technically, the file is `libvrj.so.2.0.x` for VR Juggler 2.0.x, but a symlink hides this detail.) On Windows, the files are `vrj-2_2.lib` and `vrj-2_2.dll`. This naming convention is used for all the Juggler libraries.

**NOTE:** For stable releases of VR Juggler, only the major and minor version numbers are used in the version information for files and directories. Unstable, development versions use the major, minor, and patch numbers. This reflects that incremental stable releases are binary commpatible and can be upgraded "in place."

To deal with these changes, it is _highly recommended_ that Flagpoll be used for builds run from a command line. For Windows where Visual C++ project files are likely to be used, automatic linking will ensure that applications link against the correct Juggler libraries. It is no longer necessary to list the `.lib` files explicitly. (This change was made in VR Juggler 2.0.2 to aid with forward compatibility of application builds.)

**NOTE:** At this time, Flagpoll is not quite ready for use from a command line on Windows. The `.fpc` files for Juggler are installed for use on Windwos when the Windows support in Flagpoll is finished.

## Visual Studio Project Changes ##

To make debugging easier (especially with Visual C++ 8.0), the way in which the Juggler DLLs are compiled has been changed. There are now three versions of each DLL. The versions are listed below with the changes to Visual C++ project files needed to utilize each correctly:

  1. Release: Compiled against the Visual C++ release runtime with optimization features enabled. The DLL name has no special name extension (simply `vpr-2_0.dll`).
    * Define `JUGGLER_OPT` instead of `_OPT`
    * Use the `/MD` compiler flag
  1. Debug: Compiled against the Visual C++ **release** runtime with optimization features disabled and debugging symbols built in. These DLLs are named the same as the release version, and they are found in the directory `lib\debug` of the VR Juggler installation tree. They are named the same as the release version so that user code can switch to using debug-enabled libraries simply by changing the value of the `PATH` environment variable. This capability has always existed with VR Juggler on non-Windows platforms (using `(DY)LD_LIBRARY_PATH` instead of `PATH`, of course), and it is finally available for Windows users.
    * Define `JUGGLER_DEBUG` instead of `_DEBUG`
    * Use the `/MD` compiler flag
    * Do not pass `/DEBUG` to the linker
  1. DebugRt: Compiled against the Visual C++ debug runtime with optimization features disabled and debugging symbols built in. This is known as the "_d" variant since the DLLs include `_d` in their name (e.g., `vpr_d-2_0.dll`).
    * Define `JUGGLER_DEBUG` instead of `_DEBUG`
    * Use the `/MDd` compiler flag_

The first and the third were the two DLL types available with VR Juggler 2.0. The second DLL type allows users to step into Juggler code without requiring the use of the Visual C++ debug runtime.

Related to this, CppDOM 0.7.8 and beyond have a similar arrangement of DLLs, although the debug variant uses `_h` as its name extension (referring to the questionable term "hybrid DLL"). The relevant symbol to define with CppDOM is `CPPDOM_DEBUG` instead of `_DEBUG`.

Finally, it is highly recommended that user code request the dynamic versions of the Boost and CppDOM libraries. This is done by defining the symbols `BOOST_ALL_DYN_LINK` and `CPPDOM_DYN_LINK` respectively. Without these symbols, the automatic linking for Boost and CppDOM chooses the static libraries.

**IMPORTANT**: User project files should not define `_DEBUG` explicitly in the preprocessor options. Instead, leave that up to the compiler to decide based on whether `/MD` or `/MDd` is being used.

## New Configurable Properties ##

First things first: as usual, users should load configuration files in VRJConfig to get automatic updates applied. After the automatic update completes, click the **Save All** button or use the **Save As...** operation to give the VR juggler 2.2 configuration files names that are distinct from the VR Juggler 2.0 files.

A few VR Juggler 2.1 configuration enhancements were backported to the VR Juggler 2.0 branch and included with VR Juggler 2.0.2 and beyond. These changes are as follows:

  * Analog input support for the VRPN driver
  * NVIDIA swap group support in the `display_system` config element

For versions after VR Juggler 2.0.3, there is also the following:

  * More flexible multi-sampling configuration of the OpenGL frame buffer in display windows

For VR Juggler 2.2, display windows now have the following new configurable properties:

  * Full screen mode
  * Always on top

These properties are available for Windows, the X Window System, and Cocoa. If full screen mode is enabled, the window will be opened to the full size of the managed area (the desktop) regardless of the origin and size of the window.

The "always on top" property can be used to prevent other windows from being opened above VR Juggler display windows. This also comes in handy when using the GNOME Desktop because some window managers do not normally allow windows to open above the GNOME Panel.