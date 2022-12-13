# Kprintf library install directory.
set(KPRINTF_MS_LIB_PATH       "usr/local/lib/libkprintf")

# ----------
# libkprintf
# ----------

# Kprintf/sceKernelPrintf formatting buffer size.
# Allocated in the internal kprintf handler's stack.
set(KPRINTF_BUFSZ                     256)

# Maximum number of user-handlers, including the default one.
# Statically allocated on the heap.
set(KPRINTF_MAX_HANDLERS              6)

# VPL memory pool size in KiB for the messages queue (1 KiB = 1024 bytes).
# The pool is allocated in the primary kernel partition.
# The formatted messages queue is stored in this memory pool.
# Recommended: 2 KiB.
set(KPRINTF_MSG_VPLSIZE_KIB           2)
# Init priority of the events thread.
# Note PSPLINK's thread priority is 8.
# Recommended: 10.
set(KPRINTF_EV_THREAD_PRIORITY        10)
# Stack size in KiB of the events thread (1 KiB = 1024 bytes).
# The event thread processes the messages in the queue as they come.
# It calls the registered handlers for each message, using this stack.
# Recommended: 3 KiB.
set(KPRINTF_EV_THREAD_STACKSIZE_KIB   3)

# PSPLINK module detection timeout in usec.
# The library looks for PSPLINK before entering the main event loop. 
# When it's detected, another extra delay will be applied (see below).
# Recommended: 2 seconds.
set(KPRINTF_PSPLINK_DETECT_TIMEOUT    2000000) # usec
# Time to wait in usec right before entering the main event loop.
# Applied if, and only if, the PSPLINK module was detected.
# This gives time to PSPLINK and the host to properly setup the USB connection.
# Thus allowing early Kprintf/sceKernelPrintf messages to be seen in pspsh's terminal.
# Without this, they wouldn't be seen.
# Also, USB is slow to set up hence the longer delay.
# On my setup, measures of the USB setup time range from 1130000 to 6120000 usec.
# Recommended: 7 seconds.
set(KPRINTF_PSPLINK_USB_DELAY         7000000) # usec

# Default handler flags:
# Automatically register the default handler on startup.
# The default handler writes the messages to kernel's stdout.
set(KPRINTF_DH_AUTO_REGISTER          ON)
# Automatically append a newline to messages that don't have one.
set(KPRINTF_DH_AUTO_NEWLINE           ON)
# Write messages from Kprintf() API calls to stdout.
set(KPRINTF_DH_WRITE_KPRINTF          ON)
# Write messages from sceKernelPrintf() API calls to stdout.
set(KPRINTF_DH_WRITE_PRINTF           ON)
