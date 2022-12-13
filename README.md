# PSP Kprintf library

`libkprintf` is a kernel module library for the Sony PSP.

The PSP kernel provides two functions for printing debug output:
1. `Kprintf` (kernel mode) is typically used by firmware modules
2. `sceKernelPrintf` (user mode) is used by game applications

They don't do anything by themselves. To catch the messages, a kernel handler must be registered.  
That's what this library plugin is for. It enables and registers an internal kernel handler.  
This internal handler formats the message, adds it to the messages queue, and raises an event.  
The internal event thread will then wake up, and flush the message(s) in the queue.  
Each user sub-handler will be invoked for each formatted message.

A default sub-handler is registered on startup.  
It will print all caught messages to kernel's stdout.  
This is sufficient 99% of the time.

The `libkprintf` library should work anywhere (GAME/VSH).  
It cannot be used to dump messages too early in the bootchain (e.g `sysmem.prx`).  
The functions which the library depends on are loaded later in the bootchain. 

## Features

- Handle kernel mode `Kprintf` PSP API calls.
- Handle user mode `sceKernelPrintf` PSP API calls.
- Register a default and or multiple custom handlers.
- Provide libc string formatting functions (`vsnprintf`, `snprintf`, `printf`, etc).
- Re-entrant and thread-safe.
- Low kernel memory footprint.
- Settings can be tweaked (at build-time).

## Exported libraries

The `libkprintf` module exports four stub libraries:
- `KprintfLibrary` (user mode)
- `KprintfLibraryForKernel` (kernel mode)
- `KprintfLibc` (user mode)
- `KprintfLibcForKernel` (kernel mode)

If your program resides in user memory, link to the user mode one(s), otherwise kernel.

### API KprintfLibrary

- `kprintf_register_default_handler`
   Register the default handler that prints all messages to stdout.  
   By default, it is automatically registered on startup.  
   You can disable the automatic registration by [changing the parameters](src/config.cmake).
- `kprintf_register_handler`
   Register a custom handler.  
   By default, up to 6 handlers can be registered (including the default one).
- `kprintf_unregister_handler`
   Unregister a custom or the default handler.  
- `kprintf_install`
   Re-register the internal `Kprintf` handler in case it was over-registered.

You can also [adjust the max number of handlers, Kprintf buffer size, etc](src/config.cmake).

### API KprintfLibc

- `printf` (prints the characters to kernel's stdout)
- `sprintf`
- `snprintf`
- `vprintf`
- `vsprintf`
- `vsnprintf`

The following conversion specifiers are disabled (and thus removed) by default:
- floating-point conversion (%f, %F)
- floating point format conversion specifiers (%e, %E, %g, %G)
- size integer specifiers (%I8, %I16, %I32, %I64)
- length write-back specifier (%n)
- long long integral types (allows for the ll length modifier and affects %p)  

If you need one or more of those specifiers, you can [change the configuration here](libs/config.cmake).

## Cloning this repo

```shell
git clone --recurse-submodules https://github.com/Linblow/libkprintf.git
```

## Building

Before you can build the library, you need to build and install the [PSPSDK tools](https://github.com/Linblow/pspsdk_tools).  
Once done, open a terminal to the root of this project's directory:

```shell
# Build & install.
# The files for the PSP will be installed to the project's dist directory (created),
# and the files for PSP dev to the pspsdk's local directory ($PSPDEV/psp).
./b.sh all install

# Uninstall the files that were previously installed to this system.
./b.sh uninstall

# Show the other available commands.
./b.sh help
```

If you wish, you can build for an older firmware:  
Add your stubs to `sdk/stubs/XXX` where `XXX` is the firmware version.  
Then edit the firmware version in the root [CMakeLists](CMakeLists.txt).

### Generated files

On success, you will find the following files in the project's `dist` directory:
- `usr/local/lib/libkprintf/libkprintf.prx` is the library's module
- `seplugins/test_kprintf.prx` is a kernel-module to test `Kprintf`
- `PSP/GAME/test_sce_printf/EBOOT.PBP` is a user app to test `sceKernelPrintf`
- `PSP/GAME/test_tiny_printf/EBOOT.PBP` is a user app to test `printf`

For development, you will also find these files in `$PSPDEV/psp` (e.g `/usr/local/pspdev/psp`):
- `$PSPDEV/psp/lib/libkprintf/libKprintfLibrary.a`
- `$PSPDEV/psp/lib/libkprintf/libKprintfLibraryForKernel.a`
- `$PSPDEV/psp/lib/libkprintf/libKprintfLibc.a`
- `$PSPDEV/psp/lib/libkprintf/libKprintfLibcForKernel.a`
- `$PSPDEV/psp/include/libkprintf/kprintf.h`

## Usage

You need a PSP that runs a 6.60 or 6.61 custom firmware.

Add the following line to either one of `GAME.txt` or `VSH.txt` or both:
`ms0:/usr/local/lib/libkprintf/libkprintf.prx 1`

In order to view the messages through the USB connection, you need [psplink](https://github.com/pspdev/psplinkusb).  
The `psplink.prx` plugin must be enabled in VSH or GAME or both depending on your needs.  
You should add psplink in `VSH.txt` / `GAME.txt` before the `libkprintf.prx`'s line.  
On your computer, have both `usbhostfs_pc` and `pspsh` run in seperate terminals.

Make sure plugins are enabled in your CFW settings, and you're good to go!  
You should now see all messages in `pspsh` terminal.

## Credits

Library, CMake scripts, build script, etc by Linblow.  
[printf library](https://github.com/eyalroz/printf) by eyalroz, forked from [mpaland/printf](https://github.com/mpaland/printf).  
SDK headers by the uOFW team.  
PSPSDK contributors for the toolchain.
