#!/bin/bash

TARGET=./home/root
SOURCE="../../../../../../../../../fsinject/files/ip_config.sh"

echo "[FS INJECTOR] Injecting files in ${TARGET}"

mkdir -p ${TARGET}
cp -R -v ${SOURCE} ${TARGET}

echo "./ip_config.sh" > ${TARGET}/.profile