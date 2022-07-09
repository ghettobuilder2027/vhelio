#!/bin/bash

commande="ffmpeg -loop 1 -i \"$1\".jpg -c:v libx264 -t 3 -framerate $2 -crf 17 -pix_fmt yuv420p -vf scale=1296:976 intro_$1.mp4"
echo $commande
eval "$commande"
