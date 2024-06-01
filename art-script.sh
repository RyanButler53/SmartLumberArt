#!/bin/bash

filename=${1:-"art.png"}
seed=${2:-0}
len=${3:-80}
wid=${4:-80}
color=${5:-'cool'}
build/smart-lumber $seed $len $wid | python smart-lumber.py $filename $color
open $filename
