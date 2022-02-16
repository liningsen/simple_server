#!/bin/bash

displayHelp() {
    echo "###### Please Switch one from below"
    echo "./autogen"
    echo "./autogen DEBUG"
    echo "./autogen RELEASE"
}

if [ $# -gt 1 ]; then
    displayHelp
    exit 1
fi

BUILD_TYPE="RELEASE"
if [ $# -eq 1 ]; then
    if [ $1 != "DEBUG" ] && [ $1 != "RELEASE" ]; then
        displayHelp
        exit 1
    else
        BUILD_TYPE=$1
    fi
fi

DIR="./external"
if [ -d ${DIR} ]; then
    echo "${DIR} already exists"
else
    mkdir ${DIR}
fi

DIR="./log"
if [ -d ${DIR} ]; then
    echo "${DIR} already exists"
    rm -rf ${DIR}/*
else
    mkdir ${DIR}
fi

DIR="./downloads"
if [ -d "${DIR}" ]; then
    echo "${DIR} already exists"
    cd ${DIR}
    cd poco
    git pull
else
    mkdir ${DIR}
    cd ${DIR}
    echo "###### Downloading Poco Library ..."
    git clone -b master https://github.com/pocoproject/poco.git
    cd poco
fi

DIR="./cmake-build"
if [ ! -d "${DIR}" ]; then
    mkdir ${DIR}
fi

cd ${DIR}
echo "###### Building Poco Library ..."
cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake --build .
#echo "###### Please grant admistrator privilege to install library to /usr/local"
make DESTDIR=../../../external install
cd ../../../

if [ -d "${DIR}" ]; then
    rm -rf ${DIR}/*
else
    mkdir ${DIR}
fi

cd ${DIR}
echo "###### Building SimpleServer Program [${BUILD_TYPE}]..."
if [ ${BUILD_TYPE} == "DEBUG" ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug ..
else
    cmake -DCMAKE_BUILD_TYPE=Release ..
fi

export LD_LIBRARY_PATH=../external/usr/local/lib:$LD_LIBRARY_PATH
echo "####### LD_LIBRARY_PATH: ${LD_LIBRARY_PATH}"
make
echo "###### Build Complete !"
