ACLOCAL_AMFLAGS = -I m4
AM_MAKEFLAGS = --no-print-directory

SUBDIRS = src

all:
	cd src && make

clean:
	cd src && make clean
