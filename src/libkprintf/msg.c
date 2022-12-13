#include <common/psp/types.h>
#include <libkprintf/kprintf_int.h>

/* Interrupts are disabled inside this function. */
int new_msg(kprintf_msg_t **pM, const char *msg, u32 len, int isUser)
{
    int ret;
    kprintf_msg_t *m = NULL;
    u32 mSize;
    
    mSize = SIZEOF_KPRINTF_MSG(len);
    ret = sceKernelTryAllocateVpl(g_vplid, mSize, (void **)&m);
    if (ret < 0)
        return ret;
    
    memset(m, 0, mSize);
    m->next = NULL;
    m->isUser = isUser;
    m->len = len;
    m->size = KPRINTF_MSG_SIZE(len);
    strncpy(m->msg, msg, len);
    m->msg[len] = 0;

    *pM = m;
    return 0;
}

/* Interrupts are disabled inside this function.
   Add a message to the rear of the queue. */
void add_msg(kprintf_msg_t *m)
{
    kprintf_msg_t *prev, *cur;
    
    prev = NULL;
    cur = g_head;
    while (cur)
    {
        prev = cur;
        cur = cur->next;
    }

    if (!prev && !cur)
        g_head = m;
    else
        prev->next = m;
}

/* Remove a message from anywhere in the queue and free it. Thread-safe. */
static int remove_delete_msg(kprintf_msg_t *m)
{
    int ret;
    int intr;
    kprintf_msg_t *prev, *cur;

    if (g_head == NULL)
        return 0;

    ret = 0;
    intr = sceKernelCpuSuspendIntr();
    prev = NULL;
    cur = g_head;
    while (cur)
    {
        if (cur == m)
        {
            if (g_head == m)
                g_head = m->next;
            else if (prev)
                prev->next = m->next;
            
            ret = sceKernelFreeVpl(g_vplid, m);
            break;
        }
        prev = cur;
        cur = cur->next;
    }

    sceKernelCpuResumeIntrWithSync(intr);
    return ret;
}

void flush_messages()
{
    kprintf_msg_t *m, *n;
    m = g_head;
    while (m != NULL)
    {
        
        invoke_handlers(m);
        n = m->next;
        remove_delete_msg(m);
        m = n;
    }
}
