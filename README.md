# Fail2Drop

A simple alternative for those who are too lazy to configure fail2ban


## Setup

To run this project, you will need to follow next steps

- `git clone https://github.com/fwflunky/fail2drop && cd fail2drop`
- `cmake .`
- `make -j2`
- `sudo ./fail2drop`

The ssh log must be activated in order to program work

Additional: background work
- `sudo apt install tmux`
- `tmux`
- `./fail2drop`