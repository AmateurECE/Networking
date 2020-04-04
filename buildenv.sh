#!/bin/bash
###############################################################################
# NAME:             buildenv.sh
#
# AUTHOR:           Ethan D. Twardy <edtwardy@mtu.edu>
#
# DESCRIPTION:      Utilities for operating on the build environment, needed to
#                   test on OS X since Apple has deprecated openssl in favor
#                   of Common Crypto.
#
# CREATED:          04/04/2020
#
# LAST EDITED:      04/04/2020
###

die()
{
    printf '%s\n' "$2"
    exit $1
}

# It's very important that the indentation is made with tabs, not spaces!
read -r -d '' USAGE <<- EOF
	Usage: buildenv.sh [build]

	(no arguments):    Start the build environment using Docker
	build:             Build the build environment using 'docker build'
EOF

if [[ "x$1" = "x-h" || "x$1" = "x--help" ]]; then
    die 1 "$USAGE"
fi

CONTAINER=openssl

if [[ "x$1" = "x" ]]; then
    docker run -it --rm -v `realpath .`:/root --entrypoint="/bin/sh" $CONTAINER
elif [[ $1 = "build" ]]; then
    docker build -t $CONTAINER .
elif [[ $1 = "openssl" ]]; then
    docker run -it --rm -v `realpath .`:/root --entrypoint="openssl" \
           $CONTAINER ${@:2}
else
    die 1 "$USAGE"
fi

###############################################################################
