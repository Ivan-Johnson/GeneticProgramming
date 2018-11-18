#!/bin/bash
fIn="$1"
slideLen="$2"
dirOut="$3"
#Each block of $slideLen lines of $fIn is written to its own file in $dirOut

fTmp="$(mktemp)"

mkdir "$dirOut"

slideNo=1;
while read slide; do
	slide+=$'\n'
	for i in $(seq 2 $slideLen); do
		read line
		slide+=$line$'\n'
	done
	echo "$slide" > "$fTmp"
	convert -size 960x1090 xc:white -font "Noto-Sans-Mono-Regular" -pointsize 36 -fill black -annotate +15+15 "@$fTmp" "$dirOut/${slideNo}.png"
	echo $((slideNo++))
done <<< "$(cat "$fIn")"
echo $((slideNo--))
for dupeNo in $(seq "$slideNo" $((slideNo + 48))); do
	cp "$dirOut/${slideNo}.png" "$dirOut/${dupeNo}.png"
done

ffmpeg -y -framerate 24 -i "${dirOut}/%d.png" demo.mp4
