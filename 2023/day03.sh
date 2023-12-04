#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

result_part1=0 && result_part2=0

function main {
	rownr=0
	declare -A look_for_numbers_at
	declare -A asterisks_count
	declare -A asterisks_value
	while IFS="" read -r row; do
		for ((x = 0; x < ${#row}; x++)); do
			! [[ ${row:$x:1} =~ ^[0-9.]+$ ]] && {
				[ "${row:$x:1}" == "*" ] && {
					cord_if_asterisk="$rownr $x"
					asterisks_count[$cord_if_asterisk]="0"
					asterisks_value[$cord_if_asterisk]="1"
				} || cord_if_asterisk=""
				look_for_numbers_at["$((rownr - 1)) $((x - 1))"]="${cord_if_asterisk}"
				look_for_numbers_at["$((rownr - 1)) $((x - 0))"]="${cord_if_asterisk}"
				look_for_numbers_at["$((rownr - 1)) $((x + 1))"]="${cord_if_asterisk}"
				look_for_numbers_at["$((rownr + 1)) $((x - 1))"]="${cord_if_asterisk}"
				look_for_numbers_at["$((rownr + 1)) $((x - 0))"]="${cord_if_asterisk}"
				look_for_numbers_at["$((rownr + 1)) $((x + 1))"]="${cord_if_asterisk}"
				look_for_numbers_at["$((rownr - 0)) $((x - 1))"]="${cord_if_asterisk}"
				look_for_numbers_at["$((rownr - 0)) $((x + 1))"]="${cord_if_asterisk}"
			}
		done
		rownr=$((rownr + 1))
	done <<<"$@"

	rownr=0 && current_number="" && this_number_is_next_to_asterix_at_cord=""
	declare -A added_numbers
	while IFS="" read -r row; do
		for ((x = 0; x < ${#row}; x++)); do
			[[ ${row:$x:1} =~ ^[0-9]+$ ]] && {
				cord="$rownr $x"
				[ "${look_for_numbers_at[$cord]}" != "" ] && this_number_is_next_to_asterix_at_cord="${look_for_numbers_at[$cord]}"
				[ -v "look_for_numbers_at[$cord]" ] && next_to_symbol=/bin/true
				current_number="$current_number${row:$x:1}"
			} || check-if-valid
		done
		check-if-valid
		rownr=$((rownr + 1))
	done <<<"$@"

	for item in "${added_numbers[@]}"; do
		result_part1=$((result_part1 + item))
	done
	for key in "${!asterisks_value[@]}"; do
		[ "${asterisks_count[$key]}" == "2" ] && result_part2=$((result_part2 + ${asterisks_value[$key]}))
	done

}

function check-if-valid {
	[ "$current_number" ] && [ ! -v "${added_numbers[$rownr $x]}" ] && {
		$next_to_symbol && added_numbers["$rownr $x"]="$current_number"
		[ "$this_number_is_next_to_asterix_at_cord" != "" ] && {
			asterisks_count[$this_number_is_next_to_asterix_at_cord]=$((asterisks_count[$this_number_is_next_to_asterix_at_cord] + 1))
			asterisks_value[$this_number_is_next_to_asterix_at_cord]=$((${asterisks_value[$this_number_is_next_to_asterix_at_cord]} * current_number))
		}
	}

	current_number=""
	next_to_symbol=/bin/false
	this_number_is_next_to_asterix_at_cord=""
}

SECONDS=0

main "$(cat data/"$(basename -s .sh "$0")".txt)"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2"
printf "Elapsed: %02d:%02d:%02d\n" $((SECONDS / 3600)) $(((SECONDS % 3600) / 60)) $((SECONDS % 60))