# Console, TTY, PTY

TTY/PTY support belongs before networking.

## TTY

TTY is the terminal abstraction used by shells/programs.

A physical virtual console can connect keyboard/display directly to a TTY.

Multiple virtual consoles can maintain independent terminal state and shells. A console mux chooses which receives physical keyboard input and which screen buffer is presented.

A Linux-style text virtual console does **not** inherently require a PTY.

## PTY

A PTY allows software to impersonate the terminal side.

    shell/app
       |
    PTY slave
       |
    PTY master
       |
    terminal emulator / telnetd / sshd

PTYs should therefore arrive early because they support:
- graphical terminal windows
- terminal multiplexers
- remote login later
- testing/automation

## Console-first milestone

Before GUI:
- keyboard/text display
- TTY layer
- multiple virtual consoles
- PTY
- shell
- pipes/redirection
- process/memory/module utilities
- debugger/monitor

The shell should not care whether its TTY is local, virtual, serial, GUI-hosted, Telnet, or SSH.