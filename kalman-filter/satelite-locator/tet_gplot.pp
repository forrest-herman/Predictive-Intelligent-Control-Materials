
set xrange [0:25]
set yrange [0:25]


set style line 2 lc rgb 'black' pt 7    # circle

plot "MARK" u 1:2 with p ls 2 t "Markers",\
     "ROB"  u 1:2 with l t "Robot",\
     "ROB"  u 4:5 with l t "Robot Est",\
     "SAT"  u 1:2 with p ls 1 t "SAT1",\
     "SAT"  u 3:4 with p ls 1 t "SAT2",\
     "SAT"  u 5:6 with p ls 1 t "SAT3",\
     "LOC"  u 1:2:4:5:3 with ellipses t "Var",\



pause -1 "Hit return to continue"


unset xrange
unset yrange

plot "LOC" u 4 with l t "sigma x",\
     "LOC" u 5 with l t "sigma y",\

pause -1 "Hit return to continue"





