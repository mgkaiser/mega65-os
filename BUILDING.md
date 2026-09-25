# Building MEGA65 OS

MEGA65 OS uses **LLVM-MOS/Clang** as the Phase-1 reference compiler. The build expects `mos-mega65-clang`.

## Install LLVM-MOS

- LLVM-MOS: https://llvm-mos.org/
- SDK releases: https://github.com/llvm-mos/llvm-mos-sdk/releases

A typical Linux install:

```sh
wget https://github.com/llvm-mos/llvm-mos-sdk/releases/latest/download/llvm-mos-linux.tar.xz
tar xf llvm-mos-linux.tar.xz -C "$HOME"
export PATH="$HOME/llvm-mos/bin:$PATH"
mos-mega65-clang --version
```

## Build

```sh
git clone https://github.com/mgkaiser/mega65-os.git
cd mega65-os
make
```

The root build now produces:

```text
kernel/kernel.bin
console/console.bin
loader/loader.prg
```

The boot medium must make `kernel.bin` and `console.bin` available by those names to the transition loader. The loader preloads the kernel at `$E000` and the console module at physical `$020000`, then publishes their extents in the versioned boot manifest.

Individual targets are `make kernel`, `make console`, and `make loader`.

The kernel is limited to 8192 resident bytes. The bootstrap console is additionally limited to 4096 executable bytes because bring-up currently exposes the conventional `$D000-$DFFF` I/O aperture while the driver executes in Page 6.

## Current bring-up status

These sources have not yet been validated by a successful LLVM-MOS build or MEGA65/Xemu boot in this repository workflow. Treat linker-script syntax, compiler ABI details and hardware behaviour as requiring that validation before calling the image bootable.

## Hardware authority

`mega65-book.pdf` is the authoritative hardware reference. Implementation must distinguish documented hardware behaviour from OS policy and from behaviour still requiring measurement.
