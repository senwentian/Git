/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "Gyro.h"

static const int RX_BUF_SIZE = 1024;
QueueHandle_t uart1_queue;

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_5)

void init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_EVEN,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 20, &uart1_queue, 0);
    ESP_LOGI("Project", "UART Init is success");
}

static void rx_task(void *arg)
{
    int read_len;
    uart_event_t event;
    ESP_LOGI("UART", "Hello_world");
    uint8_t* dtmp = (uint8_t*) malloc(1024);
    for(;;) {
        uint8_t flag = 1;
        //Waiting for UART event.
        xQueueReceive(uart1_queue, (void * )&event, (portTickType)portMAX_DELAY);
        bzero(dtmp, 1024);
        //ESP_LOGI("UART", "uart[%d] event:", UART_NUM_1);
        switch(event.type) {
            //Event of UART receving data
            /*We'd better handler data event fast, there would be much more data events than
            other types of events. If we take too much time on data event, the queue might
            be full.*/
            case UART_DATA:
                read_len = uart_read_bytes(UART_NUM_1, dtmp, event.size, portMAX_DELAY);
                //ESP_LOGI("UART", "Read %d bytes: %d, %d, %d, %d, %d", read_len, *dtmp, *(dtmp+1), *(dtmp+2), *(dtmp+3), *(dtmp+4));
                dtmp[read_len] = 0;
                const uint8_t *d = dtmp;
                static uint8_t RxState = 0, RxDataIndex = 0;
                while(flag){
                    switch(RxState){
                        case 0:
                            if(*d == 0x80) {
                                RxState++;
                            } else
                                RxState = 0;
                            break;

                        case 1:
                            Gyro_Msg[RxDataIndex] = *d;
                            RxDataIndex++;
                            if(RxDataIndex >= 6) {
                                int Check_t = Gyro_Data_validation();
                                if(Check_t == 1) {
                                    RxState++;
                                } else {
                                    RxState = 0;
                                    RxDataIndex = 0;
                                }
                            }
                            break;

                            case 2:
                                Gyro_Msg[RxDataIndex] = *d;
                                RxDataIndex++;
                                if(RxDataIndex >= 9) {
                                    int Check_t = Gyro_Temperature_validation();
                                    if(Check_t == 1) {
                                        Gyro_Info();
                                        RxState = 0;
                                        flag = 0;
                                    } else {
                                        RxState = 0;
                                        RxDataIndex = 0;
                                    }
                                }
                                break;
                            
                            default:
                                RxState = 0;
                    }
                    d++;
                }
                break;

            default:
                ESP_LOGI("UART", "uart event type: %d", event.type);
                break;
        }
    }
    free(dtmp);
    dtmp = NULL;
}

void app_main(void)
{
    init();
    // xTaskCreate(rx_task, "uart_rx_task", 1024 * 4, NULL, configMAX_PRIORITIES, NULL);
    Gyro_Msg[0] = 0x2C;
    Gyro_Msg[1] = 0x3E;
    Gyro_Msg[2] = 0x71;
    Gyro_Msg[3] = 0x31;
    Gyro_Msg[4] = 0x08;
    Gyro_Msg[5] = 0x5A;
    Gyro_Msg[6] = 0x2F;
    Gyro_Msg[7] = 0x3C;
    Gyro_Msg[8] = 0x13;

    int Check_t = Gyro_Data_validation();
    if(Check_t == 1) {
        printf("data validation is success\n");
    } else {
        printf("data validation is err\n");
    }

    Check_t = Gyro_Temperature_validation();
    if(Check_t == 1) {
        printf("temperature validation is success\n");
        Gyro_Info();
    } else {
        printf("temperature validation is err\n");
    }
}
