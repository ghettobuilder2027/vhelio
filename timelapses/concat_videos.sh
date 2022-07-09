#!/bin/bash

# Generation du fichier liste.txt
ls intro_$1.mp4 > liste.txt
ls $1.mp4 >> liste.txt

sed "s/$/\'/" liste.txt > nliste.txt
sed "s/^/file \'/" nliste.txt >liste.txt

commande="ffmpeg -f concat -safe 0 -i liste.txt -c copy d_$1.mp4"
echo $commande
eval "$commande"
