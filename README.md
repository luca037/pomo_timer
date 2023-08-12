# Description
Simple pomodoro timer.

```
┌──────────────────────────┐
│      POMODORO TIMER      │
└──────────────────────────┘
┌──────────────────────────┐
 Your settings:
    ► Work time: 45 min
    ► Break time: 7 min
└──────────────────────────┘
┌──────────────────────────┐
 Working time:
    ► timer: 0:13
    ► pomos: 5/6
   ┌───────────────────┐
    ████████████████
   └───────────────────┘
└──────────────────────────┘
Type 's' start/skip timer
```

# Install
- Clone repo
- Run setup.sh

# Configuration
In pomo.sh change

```
pomo_dir="<abs pomo_timer directory path>"
calendar_path="<abs calendar.txt path>"
```

In bashrc add
```
pomo () {
    source <pomo.sh path>
}
```

# Usage
Just run `pomo` in your terminal.
