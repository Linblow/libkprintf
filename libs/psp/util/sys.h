// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#ifndef PSP_UTIL_SYS_H
#define PSP_UTIL_SYS_H

#include <common/psp/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// int sys_is_emu_portable();
int sys_is_emu();
int sys_get_username(char *name, u32 bufsz);
int sys_exit();

#ifdef __cplusplus
}
#endif

#endif /* PSP_UTIL_SYS_H */
