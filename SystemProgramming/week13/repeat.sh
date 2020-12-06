# !bin/bash

for ((i=0;i<100;i++)); do
    make clean
    ./memPlay2 game.txt
    ./countP game.txt
done