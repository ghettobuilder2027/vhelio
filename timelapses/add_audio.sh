#!/bin/bash

commande="ffmpeg -i s_$1.mp4 -i $1.mp3 -c:v copy -map 0:v:0 -map 1:a:0 -c:a aac -b:a 64k youtube_$1.mp4"
echo $commande
eval "$commande"
