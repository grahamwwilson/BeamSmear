#!/bin/sh

# This is for ILCZ using the values in the file

# First the electrons
root -l -b -q beamsmearZ.C >elZ.out

# Sort the file by z value
sort -g --key=4 electronZ.ini >electronZ.tmp
mv electronZ.tmp electronZ.ini

# Now the positrons
# Specify the different file name and seed
root -l -b -q 'beamsmearZ.C("positronZ.ini",19)' >posZ.out

# Sort the file by z value
sort -g --key=4 positronZ.ini >positronZ.tmp
mv positronZ.tmp positronZ.ini

exit
