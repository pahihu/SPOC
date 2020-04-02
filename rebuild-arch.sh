#
# rebuild for ARCH
#

if [ ! -f $SPOC/build/ARCH ];
then
  echo "no ARCH file in $SPOC/build"
  echo "do make select ARCH=<arch> in $SPOC/build"
  exit 1
fi

#
# GMD TOOLS
#
ARCH=`cat $SPOC/build/ARCH`
cd $SPOC/gmdtools/reuse/c
rm -f ../$ARCH/*
make clean
make

#
# translator
#
cd $SPOC/build
rm -f $SPOC/bin/$ARCH/*
rm -f *.o
make all-wotools

#
# libraries
#
cd $SPOC/lib/src
rm -rf $SPOC/lib/$ARCH/*
cp Debug.h Occam2C.c Occam2C.h $SPOC/lib/$ARCH
./libinstall
