This [autotools](http://www.gnu.org/savannah-checkouts/gnu/automake/manual/html_node/index.html) project shows how to use other existing autotools packages in a new autotools package.

## What is for?

To test my ability creating GNU/Linux redistributable packages.

# The requirements

 1. Use log4cpp as the application logger
 2. Save 101 records in a sqlite database at the same time using 101 threads
 3. Do not use system libraries for log4cpp and sqlite3
 4. Once stored, print the records with the format
     <xml><inicio indice=numIndice>HolaMundo</inicio>,...</xml>
 5. Create an Autotools redistributable package
 6. Write a HOWTO

# Starting

First install the necessary system tools. If you use Debian-like distributions:

    sudo apt-get install git
    sudo apt-get install build-essential
    sudo apt-get install automake autoconf libtool
    sudo apt-get install libsqlite3-dev liblog4cpp5-dev

Create the nesting-autotools repository at GitHub initialized with a README.md an clone it.
    
    git clone https://github.com/vicnala/nesting-autotools.git
    cd nesting-autotools

[commit](https://github.com/vicnala/nesting-autotools/commit/aa9352663b7e6e8fe202605101269742b515b097)
[tree](https://github.com/vicnala/nesting-autotools/tree/aa9352663b7e6e8fe202605101269742b515b097)

## Set-up the development environment


Please, __Note__ that we install libsqlite3-dev and liblog4cpp5-dev but only to start development more quickly.

Now, we need to edit 4 files to handle all the project needs. Run these commands to create the source tree:

    mkdir src
    touch autogen.sh configure.ac Makefile.am
    touch src/Makefile.am src/main.cpp
    git add .
    git commit -a

to get this _project tree_:

    nesting-autotools
    ├── autogen.sh
    ├── configure.ac
    ├── Makefile.am
    ├── README.md
    └── src
        ├── main.cpp
        └── Makefile.am

[commit](https://github.com/vicnala/nesting-autotools/commit/f3d80120067ed4d11d2fd6406857fe3dfb16175f)
[tree](https://github.com/vicnala/nesting-autotools/tree/f3d80120067ed4d11d2fd6406857fe3dfb16175f)

## Writing the code

__autogen.sh__ creates some standard needed files and it runs autoreconf to create all config files needed to build the project.

__configure.ac__ contains invocations of the Autoconf macros that test the system features the package needs.

Please, __Note__ that we check for sqlite3 and log4cpp system libraries. Let's make the application code run and we will worry about them later.

__Makefile.am__ contains a simple definition of the subdirectories containing source code.

__src/Makefile.am__ defines the _SOURCES_ needed to build our _PROGRAMS_

__src/main.cpp__ is the application source code.

[commit](https://github.com/vicnala/nesting-autotools/commit/8e2c94362a9f6d69367af6b890c1cec84f09a7ea)
[tree](https://github.com/vicnala/nesting-autotools/tree/8e2c94362a9f6d69367af6b890c1cec84f09a7ea)

## Ruining autoreconf

    sh autogen.sh

If everything goes well the package is _NOW_ ready.

## Building

Just run ./configure and then make.

If everything goes well, now you can run the application if you have sqlite3 and log4cpp installed in your system.

    ./src/nesting

## Nesting need libraries

Ok, we have a working source code!.
Now we are ready to deal with libraries.

First of all, we need to get the sources for sqlite3 and log4cpp. To do that, I have added some lines to __autogen.sh__ that make use of wget to retrieve the packages, extract and move them the __src__ directory.

Fortunately, they are packaged in autotools format. This will make very easy to deal with them using _libtool_.

Ok, let's make some changes to __configure.ac__. First we need to add the _AC_PROG_LIBTOOL_ macro, remove the system libs checks and tell autoconf to check our lib packages directories.

    ...
    NESTING_LIBS="src/sqlite src/log4cpp"
    AC_CONFIG_SUBDIRS([$NESTING_LIBS])
    ...

Last we need to change the __src/Makefile.am__ first adding a SUBDIRS directive and the linking directives to our libraries. 

    ...
    SUBDIRS = sqlite log4cpp
    nesting_LDADD = $(top_builddir)/src/sqlite/libsqlite3.la $(top_builddir)/src/log4cpp/src/liblog4cpp.la
    nesting_CPPFLAGS = -I$(top_builddir)/sqlite -I$(top_builddir)/log4cpp/include
    ...

Now, the sources are ready again to re-generate our package.

Run autoreconf

    sh autogen.sh

and Build

    ./configure
     make
     
If everything goes well, now you can install or distribute the project

    make dist
    sudo make install
    nesting

[commit](https://github.com/vicnala/nesting-autotools/commit/bb10ddb0013f208f8d00a059f461965cbb0fc174)
[tree](https://github.com/vicnala/nesting-autotools/tree/bb10ddb0013f208f8d00a059f461965cbb0fc174)
