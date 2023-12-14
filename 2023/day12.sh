#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

SECONDS=0 && result_part1=0 && result_part2=0
date +'%Y-%m-%d %H:%M:%S' && trap $'printf "Result part1: \e[42m%s\e[0m\n" "$result_part1" \nprintf "Result part2: \e[42m%s\e[0m\n" "$result_part2" \ntimestr="";y=$((SECONDS / 60 / 60 / 24 / 365)) && [ "$y" -gt 0 ] && timestr="${timestr}${y}y ";d=$((SECONDS / 60 / 60 / 24 % 365)) && [ "$d" -gt 0 ] && timestr="${timestr}${d}d ";printf "Elapsed: %s %02d:%02d:%02d\n" "$timestr" $((SECONDS / 60 / 60 % 24)) $((SECONDS / 60 % 60)) $((SECONDS % 60))' EXIT

main() {
	while IFS= read -ra row || [ -n "${row[*]}" ]; do
		IFS=' ' read -r list right <<<"$row"
		IFS=',' read -ra contiguous_groups <<<"$right"
		[[ ! "$row" || ! "$list" || ! "${contiguous_groups[*]}" ]] && continue
		thegroups=() && for ((xgrp = 0; xgrp < ${#contiguous_groups[@]}; xgrp++)); do [ "${contiguous_groups[$xgrp]}" != "" ] && thegroups[$xgrp]="" && for ((xxx2 = 0; xxx2 < ${contiguous_groups[$xgrp]}; xxx2++)); do thegroups[$xgrp]="${thegroups[$xgrp]}#"; done; done
		part1 "${list}"
	done <"data/${1}.txt"
}
function part1 {
	local str=${2:-''}
	local x=0 && for ((x = 0; x < ${#1}; x++)); do
		if [ "${1:$x:1}" = "." ]; then
			str="${str}."
		elif [ "${1:$x:1}" = "#" ]; then
			str="${str}#"
		elif [ "${1:$x:1}" = "?" ]; then
			part1 "${1:$((x + 1))}" "${str}."
			str="${str}#"
		fi
	done

	branchrest="$str" && win="true"
	for grp in "${thegroups[@]}"; do
		x="${branchrest#*"${grp}"}" && [ "$branchrest" = "$x" ] && win="false" && break 1
		before="${branchrest%%"#"*}" && removed=$((${#branchrest} - ${#x} - ${#grp})) && [ ${#before} -lt "${removed}" ] && win="false" && break 1
		branchrest="$x" && [ "${branchrest:0:1}" = "#" ] && win="false" && break 1
	done
	[ "$win" = "true" ] && x="${branchrest%%"#"*}" && [ "$x" = "$branchrest" ] && result_part1=$((result_part1 + 1))
}

main "$(basename -s .sh "$0")"

dpkg -l | grep -q chafa && curl -s https://en.meming.world/images/en/b/be/But_It%27s_Honest_Work.jpg | chafa -
