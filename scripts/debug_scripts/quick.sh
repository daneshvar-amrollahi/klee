#!/bin/bash
inp=$1
op=$2
arg=$3


if [ "$inp" -nt "$op" ]; then
  echo "$inp -> $op"

  if grep -q "map_put" $1; then
    START=$(grep -n -m 1 "map_put" $1 |sed  's/\([0-9]*\).*/\1/')
  elif grep -q "ixgbe_recv" $1; then
    START=$(grep -n -m 1 "ixgbe_recv" $1 |sed  's/\([0-9]*\).*/\1/')
  else
    echo "no packet received"
    echo " " > $2
    exit
  fi


  if grep -q "map_put"  $1; then
    END=$(grep -n "map_put" $1 | tail -1 |sed  's/\([0-9]*\).*/\1/')
  elif grep -q "ixgbe_xmit_pkts" $1; then
    END=$(grep -n "ixgbe_xmit_pkts" $1 | tail -1 |sed  's/\([0-9]*\).*/\1/')
  else
    END=$(grep -n "exit@plt" $1 | tail -1 |sed  's/\([0-9]*\).*/\1/')
  fi
  ENDEND=$((END+1))
  sed -n ""$START","$END"p;"$ENDEND"q" $1 > $2
fi
