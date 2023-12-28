#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

SECONDS=0 && result_part1=0 && result_part2=0
date +'%Y-%m-%d %H:%M:%S' && trap $'printf "Result part1: \e[42m%s\e[0m\n" "$result_part1" \nprintf "Result part2: \e[42m%s\e[0m\n" "$result_part2" \ntimestr="";y=$((SECONDS / 60 / 60 / 24 / 365)) && [ "$y" -gt 0 ] && timestr="${timestr}${y}y ";d=$((SECONDS / 60 / 60 / 24 % 365)) && [ "$d" -gt 0 ] && timestr="${timestr}${d}d ";printf "Elapsed: %s %02d:%02d:%02d\n" "$timestr" $((SECONDS / 60 / 60 % 24)) $((SECONDS / 60 % 60)) $((SECONDS % 60))' EXIT

main() {
	readarray -t rows <"data/${1}.txt"
	part1
	part2
}
function part1 { starthandler 0 0 +1 0 result_part1; }
function part2 {
	for ((y = 0; y < ${#rows[@]}; y++)); do
		starthandler "0" "$y" "+1" "0" result_part2
		starthandler "$((${#rows[@]} - 1))" "$y" "-1" "0" result_part2
	done
	for ((x = 0; x < ${#rows[0]}; x++)); do
		starthandler "$x" "0" "0" "+1" result_part2
		starthandler "$x" "$((${#rows[0]} - 1))" "0" "-1" result_part2
	done
}
function starthandler {
	local queue_x_pos=("${1:-'0'}")
	local queue_y_pos=("${2:-'0'}")
	local queue_x_dir=("${3:-'+1'}")
	local queue_y_dir=("${4:-'0'}")
	local -n value="${5}"
	declare -A visited
	declare -A energized
	handletile
	[ "${#energized[@]}" -gt "$value" ] && value="${#energized[@]}"
}

shift-queue() { queue_x_pos=("${queue_x_pos[@]:1}") && queue_y_pos=("${queue_y_pos[@]:1}") && queue_x_dir=("${queue_x_dir[@]:1}") && queue_y_dir=("${queue_y_dir[@]:1}"); }
add-to-queue() { queue_x_pos+=("$1") && queue_y_pos+=("$2") && queue_x_dir+=("$3") && queue_y_dir+=("$4"); }

function handletile {
	while [ "${#queue_x_pos[@]}" -gt 0 ]; do
		x_pos=${queue_x_pos[0]} && y_pos=${queue_y_pos[0]} && x_dir=${queue_x_dir[0]} && y_dir=${queue_y_dir[0]}

		if [ "${x_pos}" -lt 0 ] || [ "${y_pos}" -lt 0 ] || [ "${x_pos}" -ge ${#rows[0]} ] || [ "${y_pos}" -ge ${#rows} ] || [ "${visited[${x_pos}_${y_pos}_${x_dir}_${y_dir}]+abc}" ]; then
			shift-queue
			continue
		fi
		visited["${x_pos}_${y_pos}_${x_dir}_${y_dir}"]="#"
		energized["${x_pos}_${y_pos}"]="#"

		case "${rows[${y_pos}]:${x_pos}:1}" in
		".") ;;
		"|")
			[ "${x_dir}" -ne 0 ] && {
				add-to-queue "$x_pos" "$((y_pos + 1))" "0" "+1"
				queue_x_dir[0]="0" && queue_y_dir[0]="-1"
			}
			;;
		"-")
			[ "${y_dir}" -ne 0 ] && {
				add-to-queue "$((x_pos + 1))" "$y_pos" "+1" "0"
				queue_x_dir[0]="-1" && queue_y_dir[0]="0"
			}
			;;
		"/")
			if [ "${x_dir}" -eq -1 ]; then
				queue_x_dir[0]="0" && queue_y_dir[0]="+1"
			elif [ "${x_dir}" -eq +1 ]; then
				queue_x_dir[0]="0" && queue_y_dir[0]="-1"
			elif [ "${y_dir}" -eq -1 ]; then
				queue_x_dir[0]="+1" && queue_y_dir[0]="0"
			elif [ "${y_dir}" -eq +1 ]; then
				queue_x_dir[0]="-1" && queue_y_dir[0]="0"
			fi
			;;
		"\\")
			if [ "${x_dir}" -eq -1 ]; then
				queue_x_dir[0]="0" && queue_y_dir[0]="-1"
			elif [ "${x_dir}" -eq +1 ]; then
				queue_x_dir[0]="0" && queue_y_dir[0]="+1"
			elif [ "${y_dir}" -eq -1 ]; then
				queue_x_dir[0]="-1" && queue_y_dir[0]="0"
			elif [ "${y_dir}" -eq +1 ]; then
				queue_x_dir[0]="+1" && queue_y_dir[0]="0"
			fi
			;;
		esac

		queue_x_pos[0]="$((queue_x_pos[0] + queue_x_dir[0]))"
		queue_y_pos[0]="$((queue_y_pos[0] + queue_y_dir[0]))"
	done
}

main "$(basename -s .sh "$0")"
