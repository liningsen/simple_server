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
else
    mkdir ${DIR}
fi
cd ${DIR}

DIR="./poco"
if [ -d "${DIR}" ]; then
    echo "${DIR} already exists"
    cd ${DIR}
    git pull
else
    echo "###### Downloading Poco Library ..."
    git clone -b master https://github.com/pocoproject/poco.git
    cd ${DIR}
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
echo "${PWD}"

cd ./downloads
DIR="./protobuf"
if [ -d "${DIR}" ]; then
    echo "${DIR} already exists"
    cd ${DIR}
    git pull
else
    mkdir ${DIR}
    echo "###### Downloading Protobuf Library ..."
    git clone https://github.com/google/protobuf.git
    cd ${DIR}
fi
git submodule update --init --recursive
./autogen.sh

DIR="${PWD}/../../external/usr/local"
./configure --prefix=${DIR}
echo "${DIR}"
make
make install
cd ../../
echo "${PWD}"

DIR="../external/usr/local"
cd ./proto
${DIR}/bin/protoc --cpp_out=. ReqRsp.proto
g++ -std=c++0x -c -fPIC -I${DIR}/include ReqRsp.pb.cc
g++ ReqRsp.pb.o -shared -o libpbReqRsp.so -L${DIR}/lib/ -lprotobuf
cp ./ReqRsp.pb.h ../include
DIR="../lib"
if [ ! -d "${DIR}" ]; then
    mkdir ${DIR}
fi
cp ./libpbReqRsp.so ${DIR}
cd ../

DIR="./cmake-build"
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
