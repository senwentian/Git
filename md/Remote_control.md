远程控制
============

本项目除了遥控器控制底盘移动、物料搬运之外，还可以通过TCP Client向开发板发送命令来进行控制

目前支持五种命令

>left:x           //小车向左运动x秒
>
>right:x        //小车向右运动x秒
>
>behind:x    //小车向右运动x秒
>
>front:x        //小车向右运动x秒
>
>Dstaddress:x.y  //向x方向走x秒，向y方向走y秒
>
>pickup:       //物料搬运拾起动作
>
>putdown:   //物料搬运放下物料动作

当然用户也可以修改Rev_Parse()API来增加自己的命令和控制

当开发板接收到TCP Client发送过来的命令时，会进行解析操作，

- 如果不是已有命令库中的命令，会提示用户“Invalid command”，
- 如果没有在命令后加冒号“：”，会提示用户"err format,please input ':' after an instruction"，
- 如果正确解析并成功执行，会返回给用户"Recieved！OK"

当命令通过Rev_Parse()API进行解析后，会通过任务通知之传递给Remote_Control_Task()，进而执行相应的控制操作

```c
void Remote_Control_Task(void *arg)
{
    BaseType_t xResult;
    uint32_t ulValue;
    wayfinding.num = 0;
    wayfinding.next_target_name = 0;
    while (1)
    {
        xResult = xTaskNotifyWait(0x00000000, 0xFFFFFFFF, &ulValue, portMAX_DELAY);
        if (xResult == pdPASS)
        {
            switch (ulValue)
            {
            case BIT_right:
                Current_Action(Dst_info->E_Distance, 1, -1);
                break;

            case BIT_behind:
                Current_Action(Dst_info->S_Distance, -1, 1);
                break;

            case BIT_left:
                Current_Action(Dst_info->W_Distance, -1, -1);
                break;

            case BIT_front:
                Current_Action(Dst_info->N_Distance, 1, 1);
                break;

            case BIT_Dst:
                if (x > 0)
                {
                    Current_Action(x, 1, -1);
                }
                else
                {
                    Current_Action(-x, -1, -1);
                }
                if (y > 0)
                {
                    Current_Action(y, 1, 1);
                }
                else
                {
                    Current_Action(-y, -1, 1);
                }
                break;

            case BIT_pickup:
                Pick_up_Action();
                break;

            case BIT_putdown:
                Put_down_Action();
                break;

            case BIT_QR:
                Automatic_wayfind_easy();
                break;

            default:
                break;
            }
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

```





-------------

未完待续

