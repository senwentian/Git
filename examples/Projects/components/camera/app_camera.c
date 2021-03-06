/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2019-2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS chips only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include "esp_log.h"
#include "esp_camera.h"
#include "app_camera.h"
#include "esp_system.h"

static camera_config_t *config;

static const char *TAG = "app_camera";

camera_config_t *app_get_camera_cfg(void)
{
    return config;
}

esp_err_t app_camera_init(pixformat_t format)
{
    if (config) {
        return ESP_FAIL;
    }

    config = calloc(1, sizeof(camera_config_t));

    if (!config) {
        return ESP_FAIL;
    }

    /* IO13, IO14 is designed for JTAG by default,
     * to use it as generalized input,
     * firstly declair it as pullup input */
    gpio_config_t conf;
    conf.mode = GPIO_MODE_INPUT;
    conf.pull_up_en = GPIO_PULLUP_ENABLE;
    conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    conf.intr_type = GPIO_INTR_DISABLE;
    conf.pin_bit_mask = 1LL << 13;
    gpio_config(&conf);
    conf.pin_bit_mask = 1LL << 14;
    gpio_config(&conf);

    // Camera Config
    config->ledc_channel = LEDC_CHANNEL_0;
    config->ledc_timer = LEDC_TIMER_0;
    config->pin_d0 = Y2_GPIO_NUM;
    config->pin_d1 = Y3_GPIO_NUM;
    config->pin_d2 = Y4_GPIO_NUM;
    config->pin_d3 = Y5_GPIO_NUM;
    config->pin_d4 = Y6_GPIO_NUM;
    config->pin_d5 = Y7_GPIO_NUM;
    config->pin_d6 = Y8_GPIO_NUM;
    config->pin_d7 = Y9_GPIO_NUM;
    config->pin_xclk = XCLK_GPIO_NUM;
    config->pin_pclk = PCLK_GPIO_NUM;
    config->pin_vsync = VSYNC_GPIO_NUM;
    config->pin_href = HREF_GPIO_NUM;
    config->pin_sscb_sda = SIOD_GPIO_NUM;
    config->pin_sscb_scl = SIOC_GPIO_NUM;
    config->pin_pwdn = PWDN_GPIO_NUM;
    config->pin_reset = RESET_GPIO_NUM;
    // Only support 10 MHz current. Camera will output bad image when XCLK is 20 MHz.
    config->xclk_freq_hz = 20000000;    // 10000000
    config->pixel_format = format;
    config->frame_size = FRAMESIZE_HQVGA;
    config->jpeg_quality = 10;
    config->fb_count = 1;

    // camera init
    esp_err_t ret = esp_camera_init(config);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed with error 0x%x", ret);
        free(config);
        config = NULL;
    }

    return ret;
}

void skr_camera_init(void)
{
      // Initialize camera
    if (app_camera_init(PIXFORMAT_GRAYSCALE) != ESP_OK) {
        ESP_LOGE(TAG, "camera init failed, ready to restart..");
        esp_restart();
    }
}


