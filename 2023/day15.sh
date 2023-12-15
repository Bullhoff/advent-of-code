#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

SECONDS=0 && result_part1=0 && result_part2=0
trap $'printf "Result part1: \e[42m%s\e[0m\n" "$result_part1" \nprintf "Result part2: \e[42m%s\e[0m\n" "$result_part2" \ntimestr="";y=$((SECONDS / 60 / 60 / 24 / 365)) && [ "$y" -gt 0 ] && timestr="${timestr}${y}y ";d=$((SECONDS / 60 / 60 / 24 % 365)) && [ "$d" -gt 0 ] && timestr="${timestr}${d}d ";printf "Elapsed: %s %02d:%02d:%02d\n" "$timestr" $((SECONDS / 60 / 60 % 24)) $((SECONDS / 60 % 60)) $((SECONDS % 60))' EXIT

function string_to_hash {
	local -n value="${1:-0}" && local _value=0
	[ "${hashmap["$2"]}" ] && { value=${hashmap["$2"]} && return; }
	for ((i = 0; i < ${#2}; i++)); do LC_CTYPE=C printf -v dec "%d" "'${2:$i:1}" && _value=$((((_value + dec) * 17) % 256)); done
	hashmap["$2"]="$_value"
	value=$_value
}
main() {
	declare -A hashmap lenses && declare -a boxes

	IFS=',' read -ra steps <"data/${1}.txt"
	for step in "${steps[@]}"; do
		string_to_hash val "$step" && result_part1=$((result_part1 + val))
		part2 "$step"
	done

	for box in "${!boxes[@]}"; do
		IFS='  ' read -ra labels <<<"${boxes[$box]}" && for labelnr in "${!labels[@]}"; do
			result_part2=$((result_part2 + (1 + box) * (1 + labelnr) * ${lenses[${box}_${labels[$labelnr]}]}))
		done
	done
}
function part2 {
	local str="${1:-0}"
	if label="${str%%"="*}" && [[ "$label" != "$str" ]]; then
		string_to_hash box "$label"
		local focal_length=${str:((${#label} + 1))}
		[[ "${boxes[$box]}" = *" $label "* ]] || boxes[$box]="${boxes[$box]} ${label} "
		lenses["${box}_${label}"]="$focal_length"
	elif label="${str%%"-"*}" && [[ "$label" != "$str" ]]; then
		string_to_hash box "$label"
		[[ "${boxes[$box]}" = *" $label "* ]] && boxes[$box]="${boxes[$box]/" $label "/}"
	fi
}

main "$(basename -s .sh "$0")"
