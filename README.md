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

Create the nesting-autotools repository at GitHub initialized with a README.md an clone it.

    git clone https://github.com/vicnala/nesting-autotools.git
    cd nesting-autotools

[commit]()
[tree]()

## Set-up the development environment

First install the necessary system tools. If you use Debian-like distributions:

    sudo apt-get install build-essential
    sudo apt-get install automake autoconf libtool

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

[commit]()
[tree]()
