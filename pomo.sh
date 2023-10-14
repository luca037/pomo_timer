#!/bin/bash

pomo_dir="${HOME}/Desktop/random/pomodoro/"
calendar_path="${HOME}/Desktop/random/pomodoro/calendar.txt"
today=$(date -I'date')

add_done_pomos=0
add_max_pomos=0

check_last_day() {
    last_pomo_day=$(tail -1 $calendar_path | cut -d ' ' -f 1)

    if [ $today == $last_pomo_day ]; then
        echo ">> Last session: $(tail -1 $calendar_path)"
        echo ">> Do you want to restart where you left? [y/n]"

        while true; do
            read res
            if [ "$res" == "y" ]; then
                add_done_pomos=$(tail -1 $calendar_path | cut -d ' ' -f 13 | cut -d '/' -f 1)
                add_max_pomos=$(tail -1 $calendar_path | cut -d ' ' -f 13 | cut -d '/' -f 2)
                break
            elif [ "$res" == "n" ]; then
                break
            fi
        done

        # delete last line 
        lines=$(wc -l < $calendar_path)
        ((lines=lines-1))
        touch "${pomo_dir}tmp.txt"
        head -n$lines $calendar_path > "${pomo_dir}tmp.txt"
        cat "${pomo_dir}tmp.txt" > $calendar_path
        rm "${pomo_dir}tmp.txt"
    fi
}

# manage flags
if [ "$1" == "-h" ]; then
    cat $calendar_path
    exit 0
fi

# manage last session
check_last_day

# run pomo
"${pomo_dir}build/pomodoro" "$add_done_pomos" "$add_max_pomos" "$calendar_path" "$today" 2>/dev/null
