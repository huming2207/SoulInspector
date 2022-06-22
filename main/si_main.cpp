#include <cstdio>
#include "ble_manager.hpp"
#include "config_manager.hpp"

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(config_manager::instance().init());
    ESP_ERROR_CHECK(ble_manager::instance().init());

    vTaskDelay(portMAX_DELAY);
}
