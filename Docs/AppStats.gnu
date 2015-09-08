set terminal pdf font "Verdana,7"
set output "AppStats.pdf"
set autoscale # scale axes automatically
set logscale y
set style data histogram
set style histogram cluster gap 1
set style fill solid 1.0
set style fill solid border -1
set boxwidth 0.9
set xtics scale 0 rotate by 60 right
set ylabel "Total time [s]"
plot 'AppStats.dat' using ($2/1000000):xtic(1) title col,\
	'' using ($3/1000000):xtic(1) title col

set output "ApplicationStatsPercentage.pdf"
unset logscale y
set ylabel "Total time [%]"
plot "AppStatsPercentage.dat" skip 1 using 1:($3*100):xtic(2) with boxes title "Total time"