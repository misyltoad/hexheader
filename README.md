Hexheader is a simple program which converts any file specified to a 
hexdump, and implements it in a C header file.

Example:
./hexheader <filename>
Resulting file:
<filename>.h

How to build:
On Linux with GCC, run `./build.sh`, it should build to 'hexheader' and 
become executable (:

On other platforms, have fun with CMake.
