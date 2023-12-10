#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

result_part1=0 && result_part2=0

function main {
	# todo: redo everything
	y=0
	declare -A startpos
	while IFS= read -ra row || [ -n "${row[*]}" ]; do
		for ((x = 0; x < ${#row}; x++)); do
			tile="${row:$x:1}"
			eval "tile_${x}_${y}=\"${tile}\""
			if [ "$tile" = "S" ]; then
				startpos[x]="$x" && startpos[y]="$y"
			fi
		done
		y=$((y + 1))
	done <"data/${1}.txt"

	xmax="$x"
	ymax="$y"
	part1
	part2
}

function part1 {
	path=()
	declare -A currentpos && currentpos[x]="${startpos[x]}" && currentpos[y]="${startpos[y]}"
	while :; do
		x=${currentpos[x]}
		y=${currentpos[y]}
		path+=("$x $y")

		if [ "$x" -eq "${startpos[x]}" -a "$y" -eq "${startpos[y]}" -a "${#path[@]}" -le 1 ]; then
			pick-direction
		elif [ "$x" -eq "${startpos[x]}" -a "$y" -eq "${startpos[y]}" -a "${#path[@]}" -gt 2 ]; then
			break
		else
			IFS=' ' read -r prev_x prev_y <<<"${path[-2]}"
			eval "fromtile=\$tile_${x}_${y}"
			eval "tile_${x}_${y}=\"x\""
			if [ "$fromtile" = '|' ]; then
				[ "$prev_y" -eq "$((y - 1))" ] && y=$((y + 1)) || y=$((y - 1))
			elif [ "$fromtile" = "J" ]; then
				[ "$prev_y" -eq "$((y - 1))" ] && x=$((x - 1)) || y=$((y - 1))
			elif [ "$fromtile" = "L" ]; then
				[ "$prev_y" -eq "$((y - 1))" ] && x=$((x + 1)) || y=$((y - 1))
			elif [ "$fromtile" = "F" ]; then
				[ "$prev_y" -eq "$((y + 1))" ] && x=$((x + 1)) || y=$((y + 1))
			elif [ "$fromtile" = "7" ]; then
				[ "$prev_y" -eq "$((y + 1))" ] && x=$((x - 1)) || y=$((y + 1))
			elif [ "$fromtile" = "-" ]; then
				[ "$prev_x" -eq "$((x - 1))" ] && x=$((x + 1)) || x=$((x - 1))
			elif [ "$fromtile" = "|" ]; then
				[ "$prev_y" -eq "$((y - 1))" ] && y=$((y + 1)) || y=$((y - 1))
			fi

			currentpos[x]="$x"
			currentpos[y]="$y"
		fi

	done
	[ "${path[0]}" = "${path[-1]}" ] && path=("${path[@]:1}")
	result_part1=$((${#path[@]} / 2))
}

function part2 {
	area=0
	j=$((${#path[@]} - 1))
	for ((i = 0; i < ${#path[@]}; i++)); do
		IFS=' ' read -r x1 y1 <<<"${path[$j]}"
		IFS=' ' read -r x2 y2 <<<"${path[$i]}"
		area=$((area + ((x1 + x2) * (y1 - y2))))
		j=$i
	done
	area=$(((area - ${#path[@]}) / 2))
	result_part2=$((area + 1))

}

function get-tile {
	eval "echo \$tile_${1}_${2}"
}
function pick-direction {
	up="$(get-tile ${currentpos[x]} $((${currentpos[y]} - 1)))"
	down="$(get-tile ${currentpos[x]} $((${currentpos[y]} + 1)))"
	left="$(get-tile $((${currentpos[x]} - 1)) ${currentpos[y]})"
	right="$(get-tile $((${currentpos[x]} + 1)) ${currentpos[y]})"

	if [ "$up" = "|" ]; then
		currentpos[y]=$((y - 1))
	elif [ "$up" = "7" ]; then
		currentpos[y]=$((y - 1))
	elif [ "$up" = "F" ]; then
		currentpos[y]=$((y - 1))

	elif [ "$down" = "|" ]; then
		currentpos[y]=$((y + 1))
	elif [ "$down" = "J" ]; then
		currentpos[y]=$((y + 1))
	elif [ "$down" = "L" ]; then
		currentpos[y]=$((y + 1))

	elif [ "$left" = "-" ]; then
		currentpos[x]=$((x - 1))
	elif [ "$left" = "L" ]; then
		currentpos[x]=$((x - 1))
	elif [ "$left" = "F" ]; then
		currentpos[x]=$((x - 1))

	elif [ "$right" = "-" ]; then
		currentpos[x]=$((x + 1))
	elif [ "$right" = "J" ]; then
		currentpos[x]=$((x + 1))
	elif [ "$right" = "L" ]; then
		currentpos[x]=$((x + 1))
	fi
}

SECONDS=0

main "$(basename -s .sh "$0")"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2"
printf "Elapsed: %02d:%02d:%02d\n" $((SECONDS / 3600)) $(((SECONDS % 3600) / 60)) $((SECONDS % 60))
