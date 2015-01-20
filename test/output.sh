#!/bin/ksh
Arg0="$0"
ProcessNameBase="`echo ${Arg0} | sed 's/\// /g' | nawk '{print $NF}'`"
echo ${ProcessNameBase}
      # full path specified in command line
      CommandBase="`echo ${Arg0} | sed -e 's/\/$//g' | \
      nawk -F\/ '{for (i = 1; i < (NF-1); i++) printf $i\"/\"; printf $(NF-1)}'`"
      # fix nedit syntax highlight with a: '
      CurrentWorkingDir="${CommandBase}"
echo ${CommandBase}   
echo ${CurrentWorkingDir}   
SpinnerSpeepInterval="1"
StartSpinner()
{
  # print the start of the message
  printf "$*...|"
  
  # loop forever
  while true
  do
    # sleep, backspace and print the next character
    sleep ${SpinnerSpeepInterval}
    printf "\b/"

    # sleep, backspace and print the next character
    sleep ${SpinnerSpeepInterval}
    printf "\b-"

    # sleep, backspace and print the next character
    sleep ${SpinnerSpeepInterval}
    printf "\b\\"

    # sleep, backspace and print the next character
    sleep ${SpinnerSpeepInterval}
    printf "\b|"
  done

  # return clean
  return ${ExitClean}
}
#StartSpinner "Hello World!"

