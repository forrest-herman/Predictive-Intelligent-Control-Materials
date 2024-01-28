a=-0.993639
b=0.217500
c=-0.161967

f(x) = (a * x * x + b * x + c)

plot "DATA" u 1:2   with p title "y vs x", \
f(x) title 'regression' with lines linestyle 1

pause -1 "Hit return to continue"