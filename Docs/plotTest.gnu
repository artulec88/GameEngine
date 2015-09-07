set terminal pdf
set output "plotTest.pdf"
set autoscale # scale axes automatically
unset log # remove any log-scaling
unset label # remove any previous labels
set xtic auto # set xtics automatically
set ytic auto # set ytics automatically
set title "Trigonometry"
set xlabel "x"
set ylabel "y"
plot sin(x) title 'Sine', tan(x) title 'Tangent'