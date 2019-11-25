#!/bin/bash

# THIS SCRIPT BUILDS THE SERVER IF NEEDED BE AND THEN EXECUTES IT.

THIS_SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null && pwd)"

if [ -z "$MINGW64_SPACESTUDIO_DIR" ]; then
    echo "Please set the MINGW64_SPACESTUDIO_DIR variable before running this script." >&2
    echo 'You can download the SpaceStudio MinGW64 toolchain here: ' >&2
    echo 'https://github.com/anthonyd973/MinGW64_SpaceStudio' >&2
    echo 'Unzip it somewhere (e.g. on a USB stick), then set the variable with:' >&2
    echo '    $ export MINGW64_SPACESTUDIO_DIR="<unzipped_dir>" # For example, <unzipped_dir> == /h/mingw64' >&2
    exit 1
fi

echo '----COMPILE AND LINK SERVER----'
"${THIS_SCRIPT_DIR}/server/mkServer"

echo '----RUN SERVER----'

# Add the 'lib' directory of OpenCV to the directories we will search for when
# trying to load shared objects.
export LD_LIBRARY_PATH="${MINGW64_SPACESTUDIO_DIR}/lib:${LD_LIBRARY_PATH}"
export PATH="${MINGW64_SPACESTUDIO_DIR}/bin:${MINGW64_SPACESTUDIO_DIR}/lib:${PATH}"

# tcp_server.exe [SERVER_IP] [IS_SIMULATION? true or false]
"${THIS_SCRIPT_DIR}/server/build/exe/tcpip_server/tcpip_server.exe" localhost true "${THIS_SCRIPT_DIR}/spacestudio/import/vision_robot"
