#! /bin/sh

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
