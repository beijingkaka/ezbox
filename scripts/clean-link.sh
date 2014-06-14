#!/bin/bash

BUILD_DIR=$1
PACKAGE_LIST=$2

# PACKAGE_LIST has format[c:source:target]
# c: command, c=copy, l=link
# source: source directory
# target: target directory

usage() {
  echo "usage: ./clean-link.sh <build directory> <packages list>"
}

dbg() {
  echo $1
}

if [ "x$BUILD_DIR" = "x" ] ; then
  usage
  exit -1
fi

if [ "x$PACKAGE_LIST" = "x" ] ; then
  usage
  exit -1
fi

while read LINE
do
  echo $LINE
  if [ "x$LINE" = "x" ] ; then
    echo "LINE is empty."
    continue;
  fi
  FIRST_CHAR=${LINE:0:1}
  LINE2=${LINE%:*}
  if [ $FIRST_CHAR = "#" ] ; then
    echo "it's a comment"
  else
    SOURCE=${LINE2%:*}
    TARGET=${LINE2#*:}
    dbg "FIRST_CHAR=[$FIRST_CHAR]"
    dbg "SOURCE=[$SOURCE]"
    dbg "TARGET=[$TARGET]"
    if [ "x$FIRST_CHAR" = "x" ] ; then
      echo "file format error @ [$LINE] !!!"
      exit -1
    fi
    if [ "x$SOURCE" = "x" ] ; then
      echo "file format error @ [$LINE] !!!"
      exit -1
    fi
    if [ "x$TARGET" = "x" ] ; then
      echo "file format error @ [$LINE] !!!"
      exit -1
    fi
    dbg "rm -rf $BUILD_DIR/$TARGET"
    rm -rf $BUILD_DIR/$TARGET
  fi
done < $PACKAGE_LIST

