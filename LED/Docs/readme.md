���� GD32F150C8T6 �� LED ��˸����

�õ��ĺ�����
1. gd_eval_led_init(LED1);

2. led_spark(void);
```c
void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0;

    if(timingdelaylocal){

        if(timingdelaylocal < 50){
            gd_eval_led_on(LED1);
        }else{
            gd_eval_led_off(LED1);
        }

        timingdelaylocal--;
    }else{
        timingdelaylocal = 100;
    }
}
```

