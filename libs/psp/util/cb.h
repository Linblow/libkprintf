// SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
// SPDX-License-Identifier: BSD-3-Clause
#ifndef PSP_UTIL_CB_H
#define PSP_UTIL_CB_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create and start the callback thread.
 * The callback thread will register the exit callback.
 * The thread will then be put to sleep while responding to system callbacks.
 * Once invoked, the exit callback invokes the user-callback, if any, 
 * and proceeds to call sceKernelExitGame().
 * @param exit_cb_cb User-callback invoked within the internal exit callback.
 * @param common Argument passed to the user-callback.
 * @return 0 success; < 0 SCE kernel error
 */
int setup_callbacks(void (*exit_cb_cb)(void *common), void *common);

/**
 * Terminate and delete the callback thread. 
 * Delete the registered exit callback.
 */
void kill_callbacks();

#ifdef __cplusplus
}
#endif

#endif /* PSP_UTIL_CB_H */
