#!/usr/bin/env bash
# -*- coding: utf-8 -*-
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd $SCRIPTDIR

max_red=12 && max_green=13 && max_blue=14

function main {
	while IFS=":" read -r game_id game_set || [ -n "$row" ]; do
		is_possible=/bin/true
		min_red=0 && min_green=0 && min_blue=0

		IFS=';' read -ra subsets <<<"$game_set"
		for subset in "${subsets[@]}"; do
			IFS=',' read -ra amount_of_each_color <<<"$subset"

			for amount_of_the_color in "${amount_of_each_color[@]}"; do
				IFS=' ' read -r thecount thecolor <<<"$amount_of_the_color"
				(($(eval "echo \${max_$thecolor}") < $thecount)) && $is_possible && is_possible=/bin/false
				(($(eval "echo \$min_$thecolor") < $thecount)) && eval "min_$thecolor=$thecount"
			done
		done
		
		result_part2=$((result_part2 + min_red * min_green * min_blue))
		$is_possible && IFS=' ' read -r _ gamenr <<<"$game_id" && result_part1=$((result_part1 + gamenr))

	done <<<"$@"
}

SECONDS=0

main "$(cat data/$(basename -s .sh "$0").txt)"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2"
printf "Elapsed: %02d:%02d:%02d\n" $(($SECONDS / 3600)) $((($SECONDS % 3600) / 60)) $(($SECONDS % 60))
