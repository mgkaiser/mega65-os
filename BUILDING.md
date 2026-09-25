# Building MEGA65 OS

MEGA65 OS uses **LLVM-MOS/Clang** as the Phase-1 reference compiler. The build expects the MEGA65 driver program `mos-mega65-clang`.

## Install LLVM-MOS

Download and install the LLVM-MOS SDK from the official project:

- LLVM-MOS: https://llvm-mos.org/
- SDK releases: https://github.com/llvm-mos/llvm-mos-sdk/releases

Prebuilt SDKs are available for Linux, macOS, and Windows. The MEGA65 toolchain documentation also confirms that LLVM-MOS supports the 45GS02 CPU with `-mcpu=mos45gs02`.

A typical Linux install using the current prebuilt SDK is:

```sh
wget https://github.com/llvm-mos/llvm-mos-sdk/releases/latest/download/llvm-mos-linux.tar.xz
tar xf llvm-mos-linux.tar.xz -C "$HOME"
export PATH="$HOME/llvm-mos/bin:$PATH"
```

For macOS or Windows, use the corresponding SDK asset from the releases page and add its `bin` directory to `PATH`.

Verify the compiler is visible:

```sh
mos-mega65-clang --version
```

## Clone and build

```sh
git clone https://github.com/mgkaiser/mega65-os.git
cd mega65-os
make
```

The root Makefile builds the kernel first and then the transition loader.

Expected build products:

```text
kernel/kernel.bin
loader/loader.prg
```

To clean:

```sh
make clean
```

To build one component:

```sh
make kernel
make loader
```

The kernel build is freestanding and currently uses:

```text
-Os
-ffreestanding
-fno-builtin
-mcpu=mos45gs02
-nostdlib
```

The kernel Makefile enforces the architectural **8192-byte resident-nucleus limit**.

## Current bring-up status

The project is in early bring-up. A successful host build does not yet imply a bootable OS image.

In particular, the documented architecture now places the resident nucleus at `$E000-$FFFF`, while some current loader/linker implementation still reflects the earlier `$4000` bring-up address. That migration and hardware/emulator validation are active project work. Do not treat the present binaries as release images until those tasks are complete.

## Hardware authority

For MEGA65 hardware and Hypervisor behavior, the project's design work treats `mega65-book.pdf` as the authoritative hardware reference. Implementation should distinguish documented hardware behavior from OS policy and from behavior that still requires measurement on hardware/emulator.
