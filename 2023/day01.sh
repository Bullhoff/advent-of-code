#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

result_part1=0 && result_part2=0

function main {
	numbers=(zero one two three four five six seven eight nine)

	while IFS="" read -r row || [ -n "$row" ]; do
		found=() && atpos=()

		for ((i = 0; i < ${#row}; i++)); do
			[[ ! "${found[0]}" && "${row:$i:1}" =~ ^[0-9]+$ ]] && found[0]="${row:$i:1}" && atpos[0]=$i
			[[ ! "${found[1]}" && "${row:$((${#row} - 1 - i)):1}" =~ ^[0-9]+$ ]] && found[1]="${row:$((${#row} - 1 - i)):1}" && atpos[1]=$((${#row} - 1 - i))
			[[ "${found[0]}" && "${found[1]}" ]] && break 1
		done
		result_part1=$((result_part1 + "${found[0]}${found[1]}"))

		for ((i = 1; i < ${#numbers[@]}; i++)); do
			str_left_of_first_match="${row%%"${numbers[i]}"*}" && [[ "$str_left_of_first_match" = "$row" ]] && pos="-1" || pos="${#str_left_of_first_match}"
			[[ "$pos" != "-1" && (! "${atpos[0]}" || "$pos" -lt "${atpos[0]}") ]] && found[0]=$i && atpos[0]=$pos
			str_left_of_last_match="${row%"${numbers[i]}"*}" && [[ "$str_left_of_last_match" = "$row" ]] && pos="-1" || pos="${#str_left_of_last_match}"
			[[ "$pos" != "-1" && (! "${atpos[1]}" || "$pos" -gt "${atpos[1]}") ]] && found[1]=$i && atpos[1]=$pos
		done
		result_part2=$((result_part2 + "${found[0]}${found[1]}"))

	done <"data/${1}.txt"
}

SECONDS=0

main "$(basename -s .sh "$0")"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2"
printf "Elapsed: %02d:%02d:%02d\n" $((SECONDS / 3600)) $(((SECONDS % 3600) / 60)) $((SECONDS % 60))
