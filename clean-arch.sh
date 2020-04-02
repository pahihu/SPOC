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
make clean
rm -rf $SPOC/gmdtools/reuse/$ARCH

#
# translator
#
cd $SPOC/build
rm -rf $SPOC/bin/$ARCH
rm -f *.o

#
# libraries
#
rm -rf $SPOC/lib/$ARCH

#
# remove ARCH
#
rm -f $SPOC/build/ARCH
