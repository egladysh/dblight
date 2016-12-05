# dblight
Lightweight C++ wrapper for sqlite

I put this thing together to take advantage of the RAII idiom with sqlite3, and to make my db stuff look a bit like C++. It's just a very simple wrapper with some retry logic. Search for MAX_DB_RETRY in ssrc/dblight.cpp to set a retry counter.

For documentation, look at the sample code sample/main.cpp and header file include/dblight.hpp ... ;)

I tested it in OSX, but it's cross-platform as long as you have boost. It requires the system and thread libraries from boost.

Make sure to install sqlite3. On OSX, you can build it, or do something like:
$ sudo port install sqlite3

Build/Install boost:
$ sudo port install boost +universal -no_static

If you are curious, the -no_static option will create static libraries, +universal is of course optional. Macports puts the boost headers into opt/local/include and libraries, opt/local/lib.

If you don't have cmake, on OSX do
$ sudo port cmake

Create a build folder, for example 
$ mkdir ~/build

Assuming you cloned dblight to <dblight>
$ cd ~/build
$ cmake <dblight>/sample
$ make

If all is well, run the sample.
$ ./dbsample


