#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

result_part1=0 && result_part2=0

function card-to-point {
	[[ "$1" =~ ^[0-9]+$ ]] && return "$(($1 + 10))"
	[ "$1" = "T" ] && return 21
	[ "$1" = "J" ] && return 22
	[ "$1" = "Q" ] && return 23
	[ "$1" = "K" ] && return 24
	[ "$1" = "A" ] && return 25
}

function main {
	declare -A handbids
	declare -A handcards_part1
	declare -A handcards_part2
	while IFS=' ' read -r hand bid || [ -n "$hand" ]; do
		handbids[$hand]=$bid

		unset thishand && declare -A thishand
		for ((i = 0; i < ${#hand}; i++)); do
			card-to-point ${hand:$i:1}
			thecard=$?
			! [ ${thishand[$thecard]+abc} ] && thishand[$thecard]=0
			thishand[$thecard]=$((${thishand[$thecard]} + 1))

			handcards_part1[$hand]="${handcards_part1[$hand]}$thecard"
			handcards_part2[$hand]="${handcards_part2[$hand]}${thecard/22/11}"
		done

		part1
		part2
	done <"data/$1.txt"

	rank=0
	for key in "${!handpart1_@}"; do
		declare -n value=$key
		rank=$((rank + 1))
		result_part1=$(($result_part1 + $rank * ${handbids[$value]}))
	done

	rank=0
	for key in "${!handpart2_@}"; do
		declare -n value=$key
		rank=$((rank + 1))
		result_part2=$(($result_part2 + $rank * ${handbids[$value]}))
	done
}

function part1 {
	type="0"
	for key in "${!thishand[@]}"; do
		cardcount=${thishand[$key]}
		case $cardcount in
		5) type="6" ;;
		4) type="5" ;;
		3)
			if [ "$type" = "0" ]; then
				type="3"
			elif [ "$type" = "1" ]; then
				type="4"
			fi
			;;
		2)
			if [ "$type" = "0" ]; then
				type="1"
			elif [ "$type" = "1" ]; then
				type="2"
			elif [ "$type" = "3" ]; then
				type="4"
			fi
			;;
		esac
	done
	eval "handpart1_${type}${handcards_part1[$hand]}=$hand"
}
function part2 {
	type=0 && jokers=0 && higestpair=0
	for key in "${!thishand[@]}"; do
		[ "$key" = "22" ] && jokers=${thishand[$key]} && continue
		[ "$higestpair" -lt "${thishand[$key]}" ] && higestpair="${thishand[$key]}"

		case ${thishand[$key]} in
		5) type="6" ;;
		4) type="5" ;;
		3)
			if [ "$type" = "0" ]; then
				type="3"
			elif [ "$type" = "1" ]; then
				type="4"
			fi
			;;
		2)
			if [ "$type" = "0" ]; then
				type="1"
			elif [ "$type" = "1" ]; then
				type="2"
			elif [ "$type" = "3" ]; then
				type="4"
			fi
			;;
		esac
	done
	if [ "$jokers" -ge 1 ]; then
		higestpair=$((higestpair + jokers))
		if [ "$type" -eq "2" -a "$jokers" -eq 1 ]; then
			type=4
		elif [ "$higestpair" -le "2" ]; then
			type=$((higestpair - 1))
		elif [ "$higestpair" -eq "3" ]; then
			type=3
		elif [ "$higestpair" -ge "4" ]; then
			type=$((higestpair + 1))
		fi
	fi
	eval "handpart2_${type}${handcards_part2[${hand}]}=$hand"
}


SECONDS=0

main "$(basename -s .sh "$0")"

printf 'Result part1: \e[42m%s\e[0m\n' "$result_part1"
printf 'Result part2: \e[42m%s\e[0m\n' "$result_part2" 
printf "Elapsed: %02d:%02d:%02d\n" $((SECONDS / 3600)) $(((SECONDS % 3600) / 60)) $((SECONDS % 60))
