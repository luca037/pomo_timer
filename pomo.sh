#!/bin/bash

check_last_day() {
    last_pomo_day=$(tail -1 $calendar_path | cut -d ' ' -f 1)
    if [ $today == $last_pomo_day ]; then
        lines=$(wc -l < $calendar_path)
        ((lines=lines-1))
        touch tmp.txt
        head -n$lines $calendar_path > tmp.txt
        cat tmp.txt > $calendar_path
        rm tmp.txt
    fi
}

calendar_path="calendar.txt"
today=$(date -I'date')
check_last_day
cd build
./pomo ../calendar.txt $today
