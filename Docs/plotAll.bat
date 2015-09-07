@echo off
for %%F in (*.gnu) do call :Sub %%F
popd
exit /b 0
:Sub
echo Processing file '%1'...
C:\Users\Artur\Downloads\gnuplot\bin\gnuplot.exe -c %1