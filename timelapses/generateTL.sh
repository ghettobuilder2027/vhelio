#!/bin/bash

commande="ffmpeg -framerate $2 -pattern_type glob -i \"$1\/*.jpg\" -c:v libx264 -crf 17 -pix_fmt yuv420p $1.mp4"
echo $commande
eval "$commande"
