#!/bin/bash

# Tudor Berariu, 2014

if [ $# -ne 1 ]; then
    echo "Usage: `basename $0` project-name"
    exit 65
else
    if [ ! -d "$1" ]; then
	mkdir $1
	mkdir $1/src
	mkdir $1/build
	cp .new-project/Makefile $1/Makefile
	cp .new-project/hello.cc $1/src/$1.cc
        cp .new-project/gitignore $1/.gitignore
	cd $1
	make build
	make run
    else
	echo "Directory $1 already exists!"
    fi
fi
