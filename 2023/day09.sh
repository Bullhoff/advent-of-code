#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

result_part1=0 && result_part2=0

function main {
	while IFS= read -ra row || [ -n "${row[*]}" ]; do
		IFS=$' ' read -ra therow <<<"$row"
		lastnumbers=("${therow[-1]}")
		firstnumbers=("${therow[0]}")

		while :; do
			extrapolated=()
			zero="true"
			for ((i = 1; i < "${#therow[@]}"; i++)); do
				diff=$((therow[i] - therow[((i - 1))]))
				extrapolated+=("$diff")
				[ "$i" -eq 1 ] && firstnumbers+=("$diff")
				[ "$i" -eq $((${#therow[@]} - 1)) ] && lastnumbers+=("$diff")
				[ "$diff" -ne 0 ] && zero="false"
			done
			[ "$zero" = "true" ] && break 1
			therow=("${extrapolated[@]}")
		done

		compare=0
		for ((i = ((${#lastnumbers[@]} - 1)); i >= 0; i--)); do
			compare=$((lastnumbers[i] + compare))
		done
		result_part1=$((result_part1 + compare))

		compare=0
		for ((i = ((${#firstnumbers[@]} - 1)); i >= 0; i--)); do
			compare=$((firstnumbers[i] - compare))
		done
		result_part2=$((result_part2 + compare))

	done <"data/$1.txt"
}

SECONDS=0

main "$(basename -s .sh "$0")"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2"
printf "Elapsed: %02d:%02d:%02d\n" $((SECONDS / 3600)) $(((SECONDS % 3600) / 60)) $((SECONDS % 60))
