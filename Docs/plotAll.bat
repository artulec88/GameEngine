@echo off
pushd
for %%F in (*.gnu) do call :Sub %%F
cd .\ClassStats
for %%F in (*.dat) do call :Class %%~nF
popd
cd ..
GOTO :EOF
:Sub
echo Processing file '%1'...
C:\Users\Artur\Downloads\gnuplot\bin\gnuplot.exe -c %1
exit /b 0
:Class
echo Processing class file '%1'
C:\Users\Artur\Downloads\gnuplot\bin\gnuplot.exe -e "filename='%1'" -c ClassStats.gnu
exit /b 0
:EOF