#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

result_part1=1 && result_part2=0

function main {
	while IFS=$'\n' read -ra row || [ -n "$row" ]; do
		[[ "$row" = "Time:"* ]] && {
			row=${row//"Time:"/""} && IFS=' ' read -ra times <<<"$row" && part2time=${row//" "/""}
		} || {
			row=${row//"Distance:"/""} && IFS=' ' read -ra records <<<"$row" && part2record=${row//" "/""}
		}
	done <"data/$1.txt"

	part1
	part2
}

function part1 {
	for ((i = 0; i < ${#times[@]}; i++)); do
		eval "ways_to_win_part1_$i=()"
		for ((s = 1; s < "${times[$i]}"; s++)); do
			[ "$((s * (${times[$i]} - s)))" -gt "${records[$i]}" ] && eval "ways_to_win_part1_$i+=($s)"
		done
	done

	for key in "${!ways_to_win_part1_@}"; do
		declare -n value=$key && result_part1=$((result_part1 * ${#value[@]}))
	done
}
function part2 {
	lowest=$(find-lowest-win)
	highest=$(find-highest-win)
	result_part2=$((highest - lowest + 1))
}
function find-lowest-win {
	top="$part2time" && bottom="0"
	while [ $((top - bottom)) -gt 1 ]; do
		checktime=$(((top + bottom) / 2))
		[ "$((checktime * (part2time - checktime)))" -gt "$part2record" ] && top=$checktime || bottom=$checktime
	done
	echo "$top"
}
function find-highest-win {
	top="$part2time" && bottom="0"
	while [ $((top - bottom)) -gt 1 ]; do
		checktime=$(((top + bottom) / 2))
		[ "$((checktime * (part2time - checktime)))" -gt "$part2record" ] && bottom=$checktime || top=$checktime
	done
	echo "$bottom"
}

SECONDS=0

main "$(basename -s .sh "$0")"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2"
printf "Elapsed: %02d:%02d:%02d\n" $((SECONDS / 3600)) $(((SECONDS % 3600) / 60)) $((SECONDS % 60))
