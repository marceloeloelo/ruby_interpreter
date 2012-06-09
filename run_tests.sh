#!/bin/bash

files=`find . -name *.in`

for file in $files
do

  # generate file paths
  file_name=`echo $file | sed 's/\(.*\).\in/\1/'`
  out_file=`echo $file | sed 's/\(.*\.\)in/\1out/'`
  res_file=`echo $file | sed 's/\(.*\.\)in/\1res/'`

  # run tests
  ./interpreter < $file > $res_file

  # run diff command
  errors=`diff $res_file $out_file 2>&1`

  # success
  if [ "$errors" == "" ]; then
  	tput setaf 2 # green
    echo "."
  #errors   
  else
    tput setaf 1 # red

    # if files exist
    if [ -e "$out_file" ] && [ -e "$res_file" ]; then
  	  echo "Diff for file $file_name"
    fi
    
    echo "$errors"
  fi
  tput setaf 1 # red
  
done

tput setaf 7 #back to white
