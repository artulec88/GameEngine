set terminal pdf
set output "ApplicationStats.pdf"
set autoscale # scale axes automatically
set size 1,1
set logscale y
set xtics font "Verdana,6" rotate by 90 right
set style data histogram
set style histogram cluster gap 1
set style fill solid 1.0
set style fill solid border -1
set boxwidth 0.9
# set title "Application statistics"
# set xlabel "Class"
set ylabel "Total time [s]"
plot '..\\Release\\AppStats.dat' using ($2/1000000):xtic(1) title col,\
	'' using ($3/1000000):xtic(1) title col

set output "ApplicationStatsPercentage.pdf"
set ylabel "Total time [%]"
plot "ApplicationStatsPercentage.dat" skip 1 using 1:($3*100):xtic(2) with boxes title "Total time"