#!/bin/bash

files=`find . -name *.in`

for file in $files
do

  # generate file paths
  file_name=`echo $file | sed 's/\(.*\).\in/\1/'`
  out_file=`echo $file | sed 's/\(.*\.\)in/\1out/'`
  res_file=`echo $file | sed 's/\(.*\.\)in/\1res/'`

  # run tests
  #echo generatin $file $res_file
  ./interpreter < $file > $res_file

  # run diff command
  diff_output=`diff $res_file $out_file`

  # success
  if [ "$diff_output" == "" ]; then
  	tput setaf 2 # green
    echo "."
  #errors   
  else
  	tput setaf 1 # red
  	echo "Diff for file $file_name"
   	echo "$diff_output"
  fi
  tput setaf 1 # red
  
done

tput setaf 7 #back to white
