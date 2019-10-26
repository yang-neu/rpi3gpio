package com.example.gpio_led;

import com.example.gpio_led.ICallback;

interface IDemoService {
    int SetLed(in int a);
    int LedOn();
    int LedOff();
    int AddNumber(in int a, in int b);
    int MaxNumber(in int a, in int b);
    void regist(ICallback cb);
    int pollSW();
}
