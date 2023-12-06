#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

result_part1= && result_part2=

function main {
	part1 "$1"
	part2 "$1"
}

function part1 {
	declare -A seeds_part1 && rownr=0
	while IFS=$'\n' read -ra row; do
		currentlocation=""
		if [ $rownr = 0 ]; then
			for seed in $row; do
				[ "$seed" != "seeds:" ] && {
					seeds_part1[$seed]=""
					eval "part1seed_${seed}=()"
				}
			done
		elif [ -z "$row" ]; then
			:
		elif [[ $row =~ map:$ ]]; then
			IFS=' ' read -r conversion scoop <<<"$row"
		else
			IFS=' ' read -ra numbers <<<"$row"
			range_start_destination=${numbers[0]} && range_start_source=${numbers[1]} && range_length=${numbers[2]}
			for key in "${!part1seed_@}"; do
				declare -n value=$key
				seednr=${key//"part1seed_"/""}

				currentlocation=${seeds_part1[$seednr]}
				[ ! "$currentlocation" ] && currentlocation="$seednr"
				newlocation=$((currentlocation + ((range_start_destination - range_start_source))))
				thename="ass_${seednr}_${conversion//"-"/"_"}"
				eval '[ -v $thename ] && continue'

				if [ $currentlocation -ge $range_start_source ] 2>/dev/null && [ $currentlocation -le $((range_start_source + range_length)) ]; then
					currentlocation=$newlocation
					seeds_part1[$seednr]=$newlocation
					eval "$thename=$newlocation"
				fi
			done
		fi
		rownr=$((rownr + 1))
	done <"data/$1.txt"

	for key in "${!seeds_part1[@]}"; do
		value=${seeds_part1[$key]}
		[ ! $result_part1 ] && result_part1=$value
		[ $value -lt $result_part1 ] && result_part1="$value"
	done
}

function part2 {
	declare -A seeds_part2 && rownr=0 && from= && to= && currentlocation=""
	while IFS=$'\n' read -ra row; do
		if [ $rownr = 0 ]; then
			for seed in $row; do
				[ "$seed" != "seeds:" ] && {
					if [ "$from" = "" ]; then
						from=$seed
					elif [ "$to" = "" ]; then
						to=$((from + seed - 1))
					fi
					[ $from ] && [ "$to" ] && {
						#for ((i = $from; i <= $to; i++)); do
						#	part2_seeds[$i]=""
						#	eval "seedpart2_${i}=()"
						#done
						eval "rangepart2_${from}_${to}=()"
						from= && to=
					}
				}
			done
		elif [ -z "$row" ]; then
			:
		elif [[ $row =~ map:$ ]]; then
			IFS=' ' read -r conversion scoop <<<"$row"
		else
			IFS=' ' read -ra numbers <<<"$row"
			range_start_destination=${numbers[0]} && range_start_source=${numbers[1]} && range_length=${numbers[2]}
			for key in "${!rangepart2_@}"; do
				IFS='_' read -r _ from to <<<"$key"
				echo "from:$from, to:$to"
			done
		fi
		rownr=$((rownr + 1))
	done <"data/$1.txt"

	result_part2=nah
}

SECONDS=0

main "$(basename -s .sh "$0")"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1" | cowsay
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2" | cowsay
printf "Elapsed: %02d:%02d:%02d\n" $((SECONDS / 3600)) $(((SECONDS % 3600) / 60)) $((SECONDS % 60))
