
:: a batch to test rtems envirement

path d:\compiler\msys\bin;d:\compiler\mingw\bin;d:\compiler\rtems-4.9\bin;%path%
tar jxvf rtems-4.9.4.tar.bz2
mkdir rtems-4.9.4-test
cd rtems-4.9.4-test
sh ../rtems-4.9.4/configure --target=arm-rtems4.9 --enable-rtemsbsp=edb7312 --enable-cxx --enable-tests=samples
make install
pause

