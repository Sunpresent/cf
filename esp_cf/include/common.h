/**
 * @file common.h
 * @author Ali Nasrolahi (A.Nasrolahi01@gmail.com).
 * @brief Generic functionalities to be used across projects.
 * @date 2023-09-08.
 *
 */
#ifndef _COMMON_H_
#define _COMMON_H_

#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "sdkconfig.h"

/**
 * @brief Returns phy mode.
 */
#define PHY_MODE(_ap)                                                                              \
    (_ap)->phy_11n ? "802.11n"                                                                     \
                   : ((_ap)->phy_11g ? "802.11g" : ((_ap)->phy_11b ? "802.11b" : "UNKOWN"))

#define INFO(...) ESP_LOGI(CONFIG_LOGGER_TAG, ##__VA_ARGS__)

#define DELAY(_t) vTaskDelay(_t)
#define DELAY_IN_SEC(_t) DELAY((_t)*100)

#endif
