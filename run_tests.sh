#!/bin/bash

if [ -z "$1" ]; then
  files=`find . -name *.rb`
else
  files=`find $1`
fi 
passed_counter=0
failed_counter=0

for file in $files
do

  # generate file paths
  file_name=`echo $file | sed 's/\(.*\).\rb/\1/'`
  out_file=`echo $file | sed 's/\(.*\.\)rb/\1out/'`
  res_file=`echo $file | sed 's/\(.*\.\)rb/\1res/'`

  # run tests
  ./ruby $file > $res_file

  # run diff command
  errors=`diff $res_file $out_file 2>&1`

  # success
  if [ -z "$errors" ]; then
  	tput setaf 2 # green
    echo "."
    passed_counter=`expr $passed_counter + 1`
  #errors
  else
    tput setaf 1 # red

    # if files exist
    if [ -e "$out_file" ] && [ -e "$res_file" ]; then
  	  echo "Diff for file $file_name"
    fi

    echo "$errors"
    failed_counter=`expr $failed_counter + 1`
  fi
  tput setaf 1 # red

done

tput setaf 2 # green
echo "Passed $passed_counter"
tput setaf 1 # red
echo "Failed $failed_counter"

tput setaf 7 #back to white
