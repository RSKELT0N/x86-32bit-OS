#!/bin/bash

#Reference: https://wiki.osdev.org/GCC_Cross-Compiler

set -e  # Exit on first error

export CROSS_COMPILER_PATH="$PWD/cross-compiler"
export PREFIX="$CROSS_COMPILER_PATH/toolchain"
export TARGET=i686-elf

echo "Defining cross-compiler under $CROSS_COMPILER_PATH"
if [[ -d $CROSS_COMPILER_PATH ]]; then
    rm -rf $CROSS_COMPILER_PATH
fi

mkdir -p $CROSS_COMPILER_PATH
echo "Entering build directory"
cd $CROSS_COMPILER_PATH

echo "Fetching binutils from 'https://ftp.gnu.org/gnu/binutils/'"
curl -s -LO https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.gz
echo "Fetching gcc from 'https://ftp.gnu.org/gnu/gcc/'"
curl -s -LO https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz
echo "Unzipping binutils and gcc"
tar -xzf binutils-2.42.tar.gz > /dev/null
tar -xzf gcc-13.2.0.tar.gz > /dev/null

echo "Building binutils to $PREFIX"
mkdir build-binutils
cd build-binutils

../binutils-2.42/configure \
  --target=$TARGET         \
  --prefix=$PREFIX         \
  --disable-nls            \
  --disable-werror         \
  --with-system-zlib       \
  --with-sysroot

echo "Building binutils"
make -j$(sysctl -n hw.logicalcpu)
echo "Installing binutils"
make install

echo "Building gcc to $PREFIX"
cd ../gcc-13.2.0
./contrib/download_prerequisites
mkdir ../build-gcc
cd ../build-gcc

CFLAGS="-Utoupper -Utolower -Uisspace -Uisprint -Uiscntrl -Uisupper -Uislower"
CXXFLAGS="-Utoupper -Utolower -Uisspace -Uisprint -Uiscntrl -Uisupper -Uislower"

../gcc-13.2.0/configure                \
    --target=$TARGET                   \
    --prefix=$PREFIX                   \
    --disable-nls                      \
    --disable-libgcov                  \
    --enable-languages=c,c++           \
    --without-headers                  \
    --with-system-zlib                 \
    --disable-coverage                 \
    --disable-libgcov                  \
    --disable-libgcov-coverage         \
    --disable-libsanitizer             \
    --disable-hosted-libstdcxx

make all-gcc -j$(sysctl -n hw.logicalcpu)
make all-target-libgcc -j$(sysctl -n hw.logicalcpu)
make all-target-libstdc++-v3 -j$(sysctl -n hw.logicalcpu)
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3

cd ..

export PATH="$PREFIX/bin:$PATH"
echo "Build finished successfully"
