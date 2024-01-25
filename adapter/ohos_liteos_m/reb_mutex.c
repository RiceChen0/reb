#include "reb_def.h"
#include "los_mux.h"

reb_mutex_id reb_mutex_create(void)
{
    UINT32 ret;
    UINT32 mutex;

    ret = LOS_MuxCreate(&mutex);
    if (ret != LOS_OK) 
    {
        return (reb_mutex_id)NULL;
    }
    return (reb_mutex_id)(GET_MUX(mutex));
}

reb_status reb_mutex_lock(reb_mutex_id mutex)
{
    UINT32 ret;
    if (mutex == NULL) {
        return REB_INVAL;
    }
    ret = LOS_MuxPend(((LosMuxCB *)mutex)->muxID, LOS_WAIT_FOREVER);
    if(ret != LOS_OK)
    {
        return REB_ERROR;
    }
    return REB_OK;
}

reb_status reb_mutex_unlock(reb_mutex_id mutex)
{
    UINT32 ret;
    if (mutex == NULL) {
        return REB_INVAL;
    }
    ret = LOS_MuxPost(((LosMuxCB *)mutex)->muxID);
    if(ret != LOS_OK)
    {
        return REB_ERROR;
    }
    return REB_OK;
}

void reb_mutex_delete(reb_mutex_id mutex)
{
    if (mutex == NULL) {
        return;
    }
    LOS_MuxDelete(((LosMuxCB *)mutex)->muxID);
}
