#!/bin/bash
#As this only runs once for each map, and there is only one map, there is no
#need to bother optimizing

for y in {1..20}; do
	for x in {1..20}; do
		cat "$1" | head -n $y | tail -n 1 | head -c $x | tail -c 1 | {
			read -rn 1 c;
			if [ "$c" = "1" ]; then
				echo "	w.hasFood[$((--x))][$((--y))] = true;";
			fi;
		} ;
	done;
	echo;
done
