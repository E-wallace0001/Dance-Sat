# Dance-Sat
Dance Sat is a simple and efficent sat solver written in C.

The basic algorithm leverages the power of C with Knuth's Dancing Links algorithm, with the applied limitation that each clause must contain one correct literal and dependency backtracking.

The next version will include clause learning.

## Use
./dance_sat *.cnf
fini.cnf will be created, and the results posted to this file.

## compilation
Compilation can be done simply with ./make.sh

