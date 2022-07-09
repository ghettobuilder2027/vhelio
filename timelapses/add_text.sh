#!/bin/bash

commande="ffmpeg -i d_$1.mp4 -vf "drawtext=text='$2':x=25:y=32:fontsize=40:fontcolor=white" s_$1.mp4"
echo $commande
eval "$commande"
