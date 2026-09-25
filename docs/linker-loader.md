# Linker, Loader, Binder

## Split responsibility

Compiler:
- emits code/data and semantic metadata

Development linker:
- resolves internal symbols
- performs ordinary optimization
- groups code/data into extents
- emits relocations/imports/exports
- emits static dependency/call information
- assigns/prepares canonical logical windows where appropriate
- writes `.mex`

OS binder/loader:
- resolves OS/library imports
- creates memory objects
- chooses current physical placement
- establishes process mappings
- performs remaining relocations/binding
- loads dynamic dependencies
- integrates current memory pressure/runtime state

The development linker knows the program. The OS knows the current machine state.

## Fixed logical execution windows

Avoid pervasive PIC if possible.

An extent linked for `$2000` normally always executes at `$2000`. Its physical backing may move anywhere.

If A and B are both linked for the same window:

    A mapped at $2000
      -> far call B
      -> save A mapping
      -> map B at $2000
      -> call B
      -> restore A mapping

The trampoline/gateway and mapping state must live outside replaceable windows.

## Dependency/call graph

Use static metadata plus runtime observations.

Static linker can report:
- call relationships
- estimated weights
- code/data dependencies
- startup likelihood
- preferred/coexistent windows
- resident/IRQ requirements

Runtime can count actual cross-extent calls and heat.

Logical window placement resembles weighted graph coloring. Physical retention uses current heat, pressure, and dependency information.

Because DMA is synchronous, dependency information is more useful for retention decisions than speculative background prefetch.

## Demand loading

Executable extents are naturally file-backed memory objects. Clean code can be discarded and later rematerialized from the executable.

Loading a program need not mean loading every code extent.