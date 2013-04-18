#! /bin/sh

SQLITE_URL="http://www.sqlite.org/2013/sqlite-autoconf-3071602.tar.gz"
LOG4CPP_URL="http://downloads.sourceforge.net/project/log4cpp/log4cpp-1.1.x%20%28new%29/log4cpp-1.1/log4cpp-1.1.tar.gz"

echo "downloading sqlite ... "
wget -q $SQLITE_URL
if test "$?" -eq "0"
then
		echo "done"
		echo -n "unpacking sqlite ... "
		tar zxf sqlite-autoconf-3071602.tar.gz
		if test "$?" -eq "0"
		then
				mv sqlite-autoconf-3071602 sqlite
				rm sqlite-autoconf-3071602.tar.gz
				echo "done"
		else
				echo "failed"
				exit 1
		fi
else
	echo "failed" 1>&2
	exit 1
fi

echo "downloading log4cpp ... "
wget -q $LOG4CPP_URL
if test "$?" -eq "0"
then
		echo "done"
		echo -n "unpacking log4cpp ... "
		tar zxf log4cpp-1.1.tar.gz
		if test "$?" -eq "0"
		then
				rm log4cpp-1.1.tar.gz
				echo "done"
		else
				echo "failed"
				exit 1
		fi
else
	echo "failed" 1>&2
	exit 1
fi

echo "running autoreconf ... "
ln -s README.md README
touch NEWS ChangeLog AUTHORS

autoreconf --force --install --verbose

if test "$?" -eq "0"
then
    echo "package ready to run ./configure && make && make install"
else
		echo "autoreconf failed!"
		exit 1
fi
