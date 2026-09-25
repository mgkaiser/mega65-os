# Networking

Networking is loadable, not resident-kernel functionality.

## Stack

    application
       -> socket API
       -> TCP/UDP
       -> IPv4 (IPv6 later)
       -> ARP/ICMP
       -> Ethernet
       -> /dev/net0

Use a conventional BSD-ish socket API where practical.

Sockets are kernel handles and waitable objects, so they participate in the same wait-set mechanism as pipes, timers, devices, and IPC.

## Interrupts

Ethernet IRQ wakes a network-driver thread. TCP/IP processing occurs in scheduled thread context, not true IRQ context.

## Buffers

Prefer memory-object references/zero-copy-ish handoff where practical rather than repeated packet copying.

Potential large-object API could allow sending ranges of a memory object.

## Initial services/tools

Likely sequence:
- DHCP
- DNS
- IPv4
- ICMP
- UDP
- TCP
- ping/netstat/route/nslookup
- telnetd
- simple httpd
- sshd later

## Telnet / SSH

`telnetd` is attractive early because PTY/session infrastructure already exists.

    TCP -> telnetd -> PTY -> login/session -> shell

`sshd` uses the same PTY/session architecture but modern cryptography makes it a later and heavier implementation task.

## HTTP

A small HTTP/1.0 or minimal HTTP/1.1 static server is a reasonable early network service once sockets and VFS work.