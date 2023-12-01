#!/usr/bin/env bash
# -*- coding: utf-8 -*-
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd $SCRIPTDIR

numbers=(zero one two three four five six seven eight nine)

function get-first-number {
	local row="$@"
	for ((i = 0; i < ${#row}; i++)); do
		[[ "${row:$i:1}" =~ ^[0-9]+$ ]] && found+=("${row:$i:1}") && atpos+=($i) && break
	done
}

function main {
	while IFS="" read -r row || [ -n "$row" ]; do
		found=()
		atpos=()
		get-first-number "$row"
		get-first-number "$(echo $row | rev)"
		result_part1=$(($result_part1 + "${found[0]}${found[1]}"))

		part2-stuff
	done <<<"$@"
}

function part2-stuff {
	[[ "${atpos[1]}" ]] && atpos[1]=$((${#row} - ${atpos[1]} - 1))
	for ((i = 1; i < ${#numbers[@]}; i++)); do
		[[ $row =~ ${numbers[i]} ]] && {
			pos="$(echo $row | grep -b -o ${numbers[i]} | head -1 | awk 'BEGIN {FS=":"}{print $1}' | xargs)"
			[ ! "${atpos[0]}" ] || [ "$pos" -lt "${atpos[0]}" ] && found[0]=$i && atpos[0]=$pos
		}
	done
	for ((i = 1; i < ${#numbers[@]}; i++)); do
		[[ $row =~ ${numbers[i]} ]] && {
			pos="$(echo $row | grep -b -o ${numbers[i]} | tail -1 | awk 'BEGIN {FS=":"}{print $1}')"
			[ ! "${atpos[1]}" ] || [ "$pos" -gt "${atpos[1]}" ] && found[1]=$i && atpos[1]=$pos
		}
	done
	result_part2=$(($result_part2 + "${found[0]}${found[1]}"))
}

SECONDS=0

main "$(cat data/$(basename -s .sh "$0").txt)"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2"
printf "Elapsed: %02d:%02d:%02d\n" $(($SECONDS / 3600)) $((($SECONDS % 3600) / 60)) $(($SECONDS % 60))
