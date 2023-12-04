#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

result_part1=0 && result_part2=0

function main {
	rownr=0
	copies_of_each_row=()
	while IFS="|" read -r left row; do
		copies_of_each_row[rownr]=$((copies_of_each_row[rownr] + 1))
		wins_this_row=0
		points_this_row=0
		IFS=':' read -r text winning_numbers <<<"$left"
		IFS=' ' read -r text cardnr <<<"$text"

		for number in ${row[@]}; do
			for winning_number in ${winning_numbers[@]}; do
				[ "${number}" = "${winning_number}" ] && {
					wins_this_row=$((wins_this_row + 1))
					[ "$points_this_row" = "0" ] && points_this_row=1 || points_this_row=$((points_this_row * 2))
				}
			done
		done

		for ((i = ((cardnr)); i < ((cardnr + wins_this_row)); i++)); do
			copies_of_each_row[i]=$((copies_of_each_row[i] + 1 * copies_of_each_row[rownr]))
		done
		result_part1=$((result_part1 + points_this_row))
		result_part2=$((result_part2 + copies_of_each_row[rownr]))
		rownr=$((rownr + 1))
	done <<<"$@"
}

SECONDS=0

main "$(cat data/"$(basename -s .sh "$0")".txt)"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2"
printf "Elapsed: %02d:%02d:%02d\n" $((SECONDS / 3600)) $(((SECONDS % 3600) / 60)) $((SECONDS % 60))
