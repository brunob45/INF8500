#!/bin/bash
#
# $0 is the HOST full path to location of current script
#

target=./home/root
source="$(dirname "$(dirname $0)")"/vision_robot

cp -R $source $target

exit 0