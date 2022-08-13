#!/bin/bash

# writeable_files.sh
#
# This script prints the names of all the writeable files in 
# a directory specified by the user as a command-line argument
# or, if no directory is specified, the current directory.
#
# J. Mohr
# 2015-01-22

# Set the default directory to the current directory.
dir=.

# See if a directory was specified on the command line.
if [ $# -gt 0 ]; then
  dir=$1
fi

# List all the writeable files in the specified or current directory.
for file in $dir/*
do
  if [ -w $file ]; then
      echo $file
  fi
done

exit 0
