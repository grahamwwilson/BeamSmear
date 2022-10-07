#!/bin/sh

# This is for ILC250 using the values in the file

# First the electrons

STATS=320k
CORR=None

root -l -b -q beamsmear.C >el250.out

# Sort the file by z value
sort -g --key=4 electron.ini >electron.tmp
mv electron.tmp electron.ini
mv Plots.root Plots_Electron_1.root

# Now the positrons
# Specify the different file name, seed, and beam energy spread
root -l -b -q 'beamsmear.C("positron.ini",7,0.00152)' >pos250.out
mv Plots.root Plots_Positron_1.root

# Sort the file by z value
sort -g --key=4 positron.ini >positron.tmp
mv positron.tmp positron.ini

mv electron.ini electron_${STATS}_${CORR}.ini
mv positron.ini positron_${STATS}_${CORR}.ini
mv el250.out el250_${STATS}_${CORR}.out
mv pos250.out pos250_${STATS}_${CORR}.out

exit
