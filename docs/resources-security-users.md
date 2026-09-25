# Resources, Security, Users, Sessions

## Resource ownership

Every kernel-managed resource should have explicit ownership/reference semantics:
- memory objects
- file handles
- device handles
- IRQ registrations
- timers
- IPC endpoints
- graphics surfaces
- exclusive hardware ownership

Each process maintains an ownership/resource list so process death does not require scanning every system object by PID.

Ownership and references are distinct. A shared object may survive the death of its creator when valid references remain.

## Capabilities

Authority should be capability-based rather than relying on a magic omnipotent UID.

Examples:
- display surface access
- exclusive display
- raw VIC access
- raw DMA
- filesystem subtree access
- raw storage

## Principals

Architect for:
- SYSTEM/trusted kernel principal
- ordinary users
- service identities

System services should receive only the capabilities they require.

## Users

Support genuine multiple users in the architecture, even if the default machine auto-logs into a single user and feels like a single-user computer.

Filesystem metadata should include ownership from the beginning.

## Sessions

A user can have one or more sessions containing processes. Logout can terminate the session and reclaim nonpersistent session-owned resources.

Remote login later becomes another way to create a user session rather than a special networking concept.