#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

result_part1=0 && result_part2=0

function main {
	galaxies_x=() && galaxies_y=()
	galaxies_per_col=()
	empty_per_row=() && emptyrows=0 && y=0
	while IFS= read -ra row || [ -n "${row[*]}" ]; do
		galaxies_in_row=0
		for ((x = 0; x < ${#row}; x++)); do
			[ ! "${galaxies_per_col[$x]}" ] && galaxies_per_col[$x]=0
			if [ "${row:$x:1}" = "#" ]; then
				galaxies_x+=("$x") && galaxies_y+=("$y")
				galaxies_in_row=$((galaxies_in_row + 1))
				galaxies_per_col[$x]=$((galaxies_per_col[x] + 1))
			fi
		done
		[ ! "${empty_per_row[$y]}" ] && empty_per_row[$y]=0
		[ "$galaxies_in_row" -eq 0 ] && emptyrows=$((emptyrows + 1))
		empty_per_row[$y]=$((empty_per_row[y] + emptyrows))
		y=$((y + 1))
	done <"data/${1}.txt"

	emptycols=0 && empty_per_col=()
	for ((x = 0; x < ${#galaxies_per_col[@]}; x++)); do
		[ "${galaxies_per_col[$x]}" -eq 0 ] && emptycols=$((emptycols + 1))
		empty_per_col[$x]=$((empty_per_col[x] + emptycols))
	done

	len="${#galaxies_x[@]}"
	for ((i = 0; i < len; i++)); do
		for ((y = $((i + 1)); y < len; y++)); do
			diffx=$(((galaxies_x[y] + empty_per_col[${galaxies_x[$y]}]) - (galaxies_x[i] + empty_per_col[${galaxies_x[$i]}])))
			diffy=$(((galaxies_y[y] + empty_per_row[${galaxies_y[$y]}]) - (galaxies_y[i] + empty_per_row[${galaxies_y[$i]}])))
			result_part1=$((result_part1 + ${diffx#-} + ${diffy#-}))

			diffx=$(((galaxies_x[y] + 999999 * empty_per_col[${galaxies_x[$y]}]) - (galaxies_x[i] + 999999 * empty_per_col[${galaxies_x[$i]}])))
			diffy=$(((galaxies_y[y] + 999999 * empty_per_row[${galaxies_y[$y]}]) - (galaxies_y[i] + 999999 * empty_per_row[${galaxies_y[$i]}])))
			result_part2=$((result_part2 + ${diffx#-} + ${diffy#-}))
		done
	done
}

SECONDS=0

main "$(basename -s .sh "$0")"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2"
printf "Elapsed: %02d:%02d:%02d\n" $((SECONDS / 3600)) $(((SECONDS % 3600) / 60)) $((SECONDS % 60))
