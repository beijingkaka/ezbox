#!/bin/bash

SRC_DIR=$1
DST_DIR=$2
ITEM_LIST=$3
DIFF_DIR=$4

usage() {
  echo "usage: ./gen_diff.sh <source dir> <dest dir> <item-list file> <diff dir>"
}

if [ "x$SRC_DIR" = "x" ] ; then
  usage
  exit -1
fi

if [ "x$DST_DIR" = "x" ] ; then
  usage
  exit -1
fi

if [ "x$ITEM_LIST" = "x" ] ; then
  usage
  exit -1
fi

if [ "x$DIFF_DIR" = "x" ] ; then
  usage
  exit -1
fi

mkdir -p "$DIFF_DIR"

while read LINE
do
  echo $LINE
  if [ "x$LINE" = "x" ] ; then
    echo "LINE is empty."
    continue;
  fi
  FIRST_CHAR=${LINE:0:1}
  if [ $FIRST_CHAR = "#" ] ; then
    echo "it's a comment."
  else
    ACTION=${LINE%:*}
    echo "ACTION=$ACTION"
    FILE_PATH=${LINE#*:}
    # do a diff action
    if [ $ACTION = "d" ] ; then
      DIFF_FILE=${FILE_PATH//\//-}.diff
      diff -urNd $SRC_DIR/$FILE_PATH $DST_DIR/$FILE_PATH > $DIFF_DIR/$DIFF_FILE
      LINE_COUNT=$(cat $DIFF_DIR/$DIFF_FILE | wc -l)
      # remove empty diff file
      if [ $LINE_COUNT -lt 1 ] ; then
        rm -f $DIFF_DIR/$DIFF_FILE
      fi
    fi
    # do a move action
    if [ $ACTION = "m" ] ; then
      FILE_DIR=${FILE_PATH%/*}
      FILE_NAME=${FILE_PATH#*/}
      if [ "x$FILE_DIR" = "x$FILE_NAME" ] ; then
        FILE_DIR=""
      fi
      mkdir -p $SRC_DIR/tmp/$FILE_DIR
      cp -af $SRC_DIR/$FILE_PATH $SRC_DIR/tmp/$FILE_DIR
      rm -rf $SRC_DIR/$FILE_PATH
      mkdir -p $DST_DIR/tmp/$FILE_DIR
      cp -af $DST_DIR/$FILE_PATH $DST_DIR/tmp/$FILE_DIR
      rm -rf $DST_DIR/$FILE_PATH
      DIFF_FILE=tmp-${FILE_PATH//\//-}.diff
      diff -urNd $SRC_DIR/tmp/$FILE_PATH $DST_DIR/tmp/$FILE_PATH > $DIFF_DIR/$DIFF_FILE
      LINE_COUNT=$(cat $DIFF_DIR/$DIFF_FILE | wc -l)
      # remove empty diff file
      if [ $LINE_COUNT -lt 1 ] ; then
        rm -f $DIFF_DIR/$DIFF_FILE
      fi
    fi
    # do a full path diff action
    if [ $ACTION = "f" ] ; then
      SOURCE_PATH=${FILE_PATH%|*}
      TARGET_PATH=${FILE_PATH#*|}
      DIFF_FILE=${SOURCE_PATH//\//-}.diff
      diff -urNd $SRC_DIR/$SOURCE_PATH $DST_DIR/$TARGET_PATH > $DIFF_DIR/$DIFF_FILE
      LINE_COUNT=$(cat $DIFF_DIR/$DIFF_FILE | wc -l)
      # remove empty diff file
      if [ $LINE_COUNT -lt 1 ] ; then
        rm -f $DIFF_DIR/$DIFF_FILE
      fi
    fi
  fi
done < $ITEM_LIST

