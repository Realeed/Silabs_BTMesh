

#include <sl_common.h>
#include "sl_bluetooth.h"
#include "sl_bt_stack_init.h"
#include "sl_component_catalog.h"
#include "sl_btmesh_provisionee.h"

#include <em_device.h>


void sl_bt_process_event(sl_bt_msg_t *evt)
{
  sl_bt_provisionee_on_event(evt);
  sl_btmesh_bgapi_listener(evt);
}


SL_WEAK bool sl_bt_can_process_event(uint32_t len)
{
  (void)(len);
  return true;
}

void sl_bt_step(void)
{
  sl_bt_msg_t evt;
  sl_bt_run();
  uint32_t event_len = sl_bt_event_pending_len();
  if ((event_len == 0) || (!sl_bt_can_process_event(event_len))) {
    return;
  }
  sl_status_t status = sl_bt_pop_event(&evt);
  if(status != SL_STATUS_OK){
    return;
  }
  sl_bt_process_event(&evt);
}
