#! /usr/bin/env bash

PROJ_DIR=$PWD

TEST_DIR=$PROJ_DIR/auto-tests

OUT_DIR=$PROJ_DIR/OUT

rm $OUT_DIR/*.out &>> /dev/null
mkdir $OUT_DIR &>> /dev/null

cd $PROJ_DIR

if [[ $# -ge 1 ]]; then
  if [[ $1 == "-c" ]]; then
    make clean && make
  fi
fi

if [[ -f "$PROJ_DIR/l22" ]]; then
  echo "Tests: "  
else
  echo "l22 compiler missing, exiting now"
  exit 1
fi

for test in `find $TEST_DIR -name "*.l22" | sort`; do
  $PROJ_DIR/l22 $test &> $OUT_DIR/${test##*/}.out
  OUTPUT=$(grep -E "[Ss]yntax [Ee]rror" $OUT_DIR/${test##*/}.out)
  if [[ -z $OUTPUT ]]; then
    echo -e "\033[0;32m - Passed ${test##*/} - \033[0m"
  else
    echo -e "\033[0;31m - Failed ${test##*/} - \033[0m"
  fi
done

rm $TEST_DIR/*.asm