### INSTALL
 1. Windows
 2. Mac OS X
 3. Linux
 
### Windows

 If using Cygwin:
    
    ./configure
    make
    make install
    
 If using Visual Studio:
 
    Follow the directions at the link for running CMake on Windows:
    http://www.opentissue.org/wikitissue/index.php/Using_CMake
    
    NOTE: Select the "build" folder as the location to build the binaries.
	
	e.g. cmake .. -G"Visual Studio 12 Win64" -DBUILD_CONF_202=ON
    
== Mac OS X ==

 ./configure
 make
 sudo make install

== Linux ==

 ./configure
 make
 sudo make install
