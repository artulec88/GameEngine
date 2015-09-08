set terminal pdf font "Verdana,7"
set output filename.".pdf"
set autoscale # scale axes automatically
set logscale y
set style data histogram
set style histogram cluster gap 1
set style fill solid 1.0
set style fill solid border -1
set boxwidth 0.9
set xtics scale 0 rotate by 60 right
set ylabel "Total time [s]"
plot filename.".dat" using ($3/1000000):xtic(1) title col,\
	filename.".dat" using ($4/1000000):xtic(1) title col,\
	filename.".dat" using ($5/1000000):xtic(1) title col