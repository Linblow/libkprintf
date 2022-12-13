#include <common/psp/types.h>
#include <libkprintf/kprintf_int.h>

volatile int g_state = STATE_STOPPED;
SceUID g_stdout = -1;
SceUID g_vplid = -1;
SceUID g_evid = -1;
SceUID g_semid = -1;
SceUID g_thid = -1;
int g_default_handler_index = -1;
u32 g_handler_count = 0;
kprintf_reghandler_t g_handlers[KPRINTF_MAX_HANDLERS] = {0};
kprintf_msg_t *g_head = NULL;

