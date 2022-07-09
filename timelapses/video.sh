bash generateTL.sh $1 $2
bash vignette.sh $1 $2
bash concat_videos.sh $1
bash add_text.sh $1 $3
