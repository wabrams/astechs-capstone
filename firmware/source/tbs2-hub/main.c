/**
 * @file main.c
 * @brief   Main source file
**/

#include "sl_system_init.h"
#include "sl_system_process_action.h"
#include "sl_component_catalog.h"

#include "app.h"

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
  #include "sl_power_manager.h"
#endif // SL_CATALOG_POWER_MANAGER_PRESENT

int main(void)
{
  // initializations
  sl_system_init();
  app_init();

  // infinite loop
  while (1)
  {
    // process actions
    sl_system_process_action();
    app_process_action();

    #if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
      sl_power_manager_sleep();
    #endif
  }
  app_exit();
}
