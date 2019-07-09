cd mysql_libs
cp -R -f ./lib/Release/*.a ../lib/Release/
cp -R -f ./lib/Release/*.a ../lib/Debug/
cp -R -f ./lib/Release/*.so ../../_Out/Release/
cp -R -f ./lib/Release/*.so ../../_Out/Debug/
cd ../

cd mysqlpp_libs
cp -R -f ./lib/Release/*.a ../lib/Release/
cp -R -f ./lib/Release/*.a ../lib/Debug/

