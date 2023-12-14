#!/usr/bin/env bash
SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd) && cd "$SCRIPTDIR" || exit

SECONDS=0 && result_part1=0 && result_part2=0
trap $'printf "Result part1: \e[42m%s\e[0m\n" "$result_part1" \nprintf "Result part2: \e[42m%s\e[0m\n" "$result_part2" \ntimestr="";y=$((SECONDS / 60 / 60 / 24 / 365)) && [ "$y" -gt 0 ] && timestr="${timestr}${y}y ";d=$((SECONDS / 60 / 60 / 24 % 365)) && [ "$d" -gt 0 ] && timestr="${timestr}${d}d ";printf "Elapsed: %s %02d:%02d:%02d\n" "$timestr" $((SECONDS / 60 / 60 % 24)) $((SECONDS / 60 % 60)) $((SECONDS % 60))' EXIT

result_part1=$(node -e "$(cat <<EOF
var resArr=[];
\`$(cat data/day14.txt)\`.split('\n').map((row, y, arr1) => row.split('')).forEach((row, y, arr1) => row.forEach((val, x, rowarr) => {if(! resArr[rowarr.length - 1 - x]){ resArr[rowarr.length - 1 - x] = []}; resArr[rowarr.length - 1 - x][y] = val })); 
resArr.forEach((row, y, arr1) => row.forEach((val, x, arr2) => { if (resArr[y][x] == 'O') for (let xx = x - 1; xx >= 0; xx--) if (resArr[y][xx] == '.') { resArr[y][xx] = val; resArr[y][xx + 1] = '.' } else break; }));
console.log(resArr.reduce((a, row, y, arr1) => a += row.reduce((a, val, x, arr2) => (val == 'O') ? a += row.length - x : a, 0), 0));
EOF
)")
