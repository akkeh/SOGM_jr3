set terminal postscript
set output '| ps2pdf - ODFplot.pdf'

set multiplot layout 2, 1
set title "Derv2:"
plot 'perFrame.txt'

set title "Waveform:"
plot 'waveform.txt'
unset multiplot
