#!/bin/bash

pomo_dir="${HOME}/Desktop/random/pomodoro/"
calendar_path="${HOME}/Desktop/random/pomodoro/calendar.txt"
today=$(date -I'date')

check_last_day() {
    last_pomo_day=$(tail -1 $calendar_path | cut -d ' ' -f 1)
    if [ $today == $last_pomo_day ]; then
        lines=$(wc -l < $calendar_path)
        ((lines=lines-1))
        touch "${pomo_dir}tmp.txt"
        head -n$lines $calendar_path > "${pomo_dir}tmp.txt"
        cat "${pomo_dir}tmp.txt" > $calendar_path
        rm "${pomo_dir}tmp.txt"
    fi
}

cd $pomo_dir
check_last_day
cd build
./pomo $calendar_path $today
