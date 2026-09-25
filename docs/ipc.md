# IPC

## Primary mechanisms

Keep kernel IPC primitives relatively small:

### Message ports / queues
Primary structured IPC mechanism. Asynchronous send; blocking/nonblocking receive; optional reply endpoint. Small messages should be copied; large payloads should normally pass memory-object handles.

### Shared memory
First-class shared memory objects. Different processes may receive different permissions to the same underlying object.

Useful for:
- graphics surfaces
- audio buffers
- large data transfers
- databases

### Signals
Lightweight process/thread notification and control, semantically distinct from hardware interrupts.

### Pipes
Byte-stream IPC, suitable for shell pipelines/stdin/stdout. Can be built over kernel buffering/wait machinery.

### Synchronization
- mutexes
- semaphores
- events
- priority inheritance where needed for realtime behavior

### Wait sets
A thread should be able to sleep until any of several waitable objects becomes ready:
- message port
- pipe
- socket
- keyboard/device
- timer
- child process
- event

Avoid polling.

## Transferable handles

Handles/capabilities should be transferable between processes subject to permissions.

Instead of copying a 2 MB object through IPC, send/grant a handle to the same object.

## Services

Named services can publish endpoints in a service namespace. Opening a service can cause its implementation module/process to load on demand.

RPC should be a user/library convention over message ports, not a special kernel primitive.