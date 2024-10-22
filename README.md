# WindyDNS
*WindyDNS* is a Windows console DNS server that always reply to any DNS queries 
with a user-specified IP address. It is usually used for simulating a multiple 
machines or network nodes environment in a single machine. Such mock-up 
environments are common in software development, testing, and malware analysis.

*WindyDNS* is based on and adapted from [Sam Trenholme's NanoDNS][nanodns] -- 
a DNS server written for Linux/Unix with remarkably short codes with 538 
characters in 14 lines. The focus is on creating a working DNS server with the 
shortest codes possible.

*WindyDNS* differs from *NanoDNS* in the following ways:

- Ported to use [Windows Socket (WinSock) API][winsock]
- Tested to build with MinGW/Microsoft Visual C++ compiler
- Added [CMake build system][cmake] support for wider compiler support
- Retains compatibility with *GNU C compiler* on Linux/Unix environment
- Much longer with added comments and renamed variables to aid understanding 
  i.e. brevity and keeping the codes short is not the focus
  
> [!TIP]
>
> While *WindyDNS* is primarily a Windows DNS server, care is taken to ensure 
> compatibility is retained such that it still builds and runs fine on 
> Linux/Unix with the GCC compiler.
>

## Building WindyDNS

*WindyDNS* uses the [CMake](https://cmake.org) build system.

In the *WindyDNS* source directory:

```bat
mkdir build
cd build
```

[CMake](https://cmake.org) allows the use of various build tools. Common ones 
include the traditional [GNU make](https://www.gnu.org/software/make/), and 
the newer [Ninja](https://ninja-build.org/).

### MinGW

To use MinGW:

```bat
cmake -G "MinGW Makefiles" ..
mingw32-make
```

### Visual C++

To use Microsoft Visual C++:

```bat
cmake -G "NMake Makefiles" ..
nmake
```

### Visual Studio

To use Microsoft Visual Studio 17 2022:

```bat
cmake -G "Visual Studio 17 2022" -A Win32 ..
msbuild
```

### Ninja

To use Ninja:

```bat
cmake -G Ninja ..
ninja
```

### GNU make

To use standard Unix makefile:

```bat
cmake -G "Unix Makefiles" ..
make
```

The built executable binary `windydns.exe` will be in the build directory, or
for multi-configuration build in Visual Studio either in `build\Release` or 
`build\Debug` sub-directory.

## Using WindyDNS

*WindyDNS* always bind to `0.0.0.0`. There is no way to change this currently.

To run, specify the fake IP address to be returned for all DNS queries as a 
command line argument.

```bat
windydns.exe <IP Address>
```

Example: To make *WindyDNS* always return `123.234.123.234`, run:

```bat
windydns.exe 123.234.123.234
```

[nanodns]: http://maradns.blogspot.com/2010/08/nanodns-updated.html
[winsock]: https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2
[cmake]: https://cmake.org
