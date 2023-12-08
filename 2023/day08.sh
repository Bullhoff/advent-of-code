#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

result_part1=0 && result_part2=1

function main {
	rownr=0
	declare -A themap
	declare -A part2_pos
	declare -A part2_goal
	while IFS= read -ra row || [ -n "$row" ]; do
		if [ "$rownr" -eq 0 ]; then
			instructions="$row"
		elif [ "$row" ]; then
			IFS='=' read -r pos leftright <<<"$row"
			pos=${pos// /}
			leftright=${leftright//[() ]/}
			IFS=',' read -r left right <<<"$leftright"
			themap[$pos]="$left $right"
			[[ "$pos" =~ [A]+$ ]] && part2_pos[$pos]="$pos"
			[[ "$pos" =~ [Z]+$ ]] && part2_goal[$pos]="$pos"
		fi
		rownr=$((rownr + 1))
	done <"data/$1.txt"

	part1
	part2
}
function part1 {
	currentpos="AAA"
	while :; do
		for ((i = 0; i < ${#instructions}; i++)); do
			result_part1=$((result_part1 + 1))
			IFS=$' ' read -r left right <<<"${themap[$currentpos]}" && [ "${instructions:$i:1}" = "L" ] && currentpos="$left" || currentpos="$right"
			[ "$currentpos" = "ZZZ" ] && break 2
		done
	done
}

function part2 {
	declare -A itotal
	for key in "${!part2_pos[@]}"; do
		itotal[$key]=0
		while :; do
			for ((i = 0; i < ${#instructions}; i++)); do
				itotal[$key]=$((itotal[$key] + 1))
				IFS=$' ' read -r left right <<<"${themap[${part2_pos[$key]}]}" && [ "${instructions:$i:1}" = "L" ] && part2_pos[$key]="$left" || part2_pos[$key]="$right"
				if [ "${part2_goal[${part2_pos[$key]}]+abc}" ]; then
					result_part2=$((result_part2 * ${itotal[$key]} / $(find-gcd $result_part2 ${itotal[$key]})))
					break 2
				fi
			done
		done
	done
}
function find-gcd {
	[ "$2" -eq 0 ] && echo "$1" || find-gcd "$2" "$(($1 % $2))"
}

SECONDS=0

main "$(basename -s .sh "$0")"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2"
printf "Elapsed: %02d:%02d:%02d\n" $((SECONDS / 3600)) $(((SECONDS % 3600) / 60)) $((SECONDS % 60))
