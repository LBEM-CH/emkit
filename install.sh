#!/bin/sh

SOURCE_DIR=${PWD}
BUILD_DIR=${PWD}/build
INSTALL_DIR=${PWD}/bin

if [ ! -e ${BUILD_DIR} ]; then
	echo "The build dir does not exist, therefore it will be created."
	mkdir -p ${BUILD_DIR}
fi

if [ ! -e ${INSTALL_DIR} ]; then
        echo "The install dir does not exist, therefore it will be created."
        mkdir -p ${INSTALL_DIR}
fi

export CC=gcc
export CXX=g++

echo '*============================================================================*'
echo '+----------------------------------------------------------------------------+'
echo '| prepare                                                                    |'
cd ${BUILD_DIR}
echo 'workinkg directory: '
pwd
echo '+----------------------------------------------------------------------------+'
echo '| configure                                                                  |'
echo cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} ${SOURCE_DIR} || { echo "Configure failed." ; exit 5; }  
cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} ${SOURCE_DIR} || { echo "Configure failed." ; exit 5; }
echo ''
echo '+----------------------------------------------------------------------------+'
echo '| make                                                                       |'
make -j1 || { echo "Build failed"; exit 6; }
echo ''
echo '+----------------------------------------------------------------------------+'
echo '| make install                                                               |'
make -j1 install || { echo "Install failed"; exit 7; }
echo '*============================================================================*'
echo ''
echo ''
echo ''
echo ''
