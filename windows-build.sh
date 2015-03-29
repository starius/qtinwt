# qtinwt, QtWebKit wrapped in Wt application
# Copyright (C) 2014-2015 Boris Nagaev
# See the LICENSE file for terms of use

# Prerequisites: http://mxe.cc/#requirements

git clone -b stable https://github.com/mxe/mxe.git
cd mxe
# get older version, with Qt 4.7.4
git show 02c11bb:src/qt.mk > src/qt.mk
git show 02c11bb:src/qt-1-cherrypicks.patch > src/qt-1-cherrypicks.patch
sed 's@get.qt.nokia.com/qt/source@download.qt-project.org/archive/qt/4.7@' \
    -i src/qt.mk
make -k qt wt
cd ..

mkdir qtinwt-build
cd qtinwt-build
toolchain=`echo ../mxe/usr/*ming*/share/cmake/mxe-conf.cmake`
moc_exe=../mxe/usr/i686-pc-mingw32/bin/moc
cmake -DCMAKE_TOOLCHAIN_FILE=$toolchain \
      -DQT_MOC_EXECUTABLE=$moc_exe ~/qtinwt/
make
