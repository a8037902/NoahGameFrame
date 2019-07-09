cd mysql_libs/
copy lib\\Release\\*.lib ..\\lib\\Release\\ /Y
copy lib\\Release\\*.lib ..\\lib\\Debug\\ /Y
copy lib\\Release\\*.dll ..\\..\\_Out\\Release\\ /Y
copy lib\\Release\\*.dll ..\\..\\_Out\\Debug\\ /Y
cd ../

cd mysqlpp_libs/
copy lib\\Release\\*.lib ..\\lib\\Release\\ /Y
copy lib\\Debug\\*.lib ..\\lib\\Debug\\ /Y
copy lib\\Release\\*.dll ..\\..\\_Out\\Release\\ /Y
copy lib\\Debug\\*.dll ..\\..\\_Out\\Debug\\ /Y