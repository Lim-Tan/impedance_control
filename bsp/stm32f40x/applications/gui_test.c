#include <rtthread.h>   
#include <rtgui/rtgui.h>   
#include <rtgui/rtgui_server.h>   
#include <rtgui/rtgui_app.h>   
#include <rtgui/widgets/window.h>   
   
//#include <rtgui/calibration.h>   
//#include "setup.h"   
   
//rt_bool_t cali_setup(void)   
//{   
//    struct setup_items setup;   
//   
//    if(setup_load(&setup) == RT_EOK)   
//    {   
//        struct calibration_data data;   
//        rt_device_t device;   
//   
//        data.min_x = setup.touch_min_x;   
//        data.max_x = setup.touch_max_x;   
//        data.min_y = setup.touch_min_y;   
//        data.max_y = setup.touch_max_y;   
//   
//        device = rt_device_find("touch");   
//        if(device != RT_NULL)   
//            rt_device_control(device, RT_TOUCH_CALIBRATION_DATA, &data);   
//        return RT_TRUE;   
//    }   
//    return RT_FALSE;   
//}   
//   
//void cali_store(struct calibration_data *data)   
//{   
//    struct setup_items setup;   
//    setup.touch_min_x = data->min_x;   
//    setup.touch_max_x = data->max_x;   
//    setup.touch_min_y = data->min_y;   
//    setup.touch_max_y = data->max_y;   
//    setup_save(&setup);   
//}   
   
void Display (void)   
{   
    struct rtgui_app * app;   
    struct rtgui_win * win;   
    struct rtgui_box * box;   
    struct rtgui_rect rect = {0, 20, 240, 320};   
    app = rtgui_app_create("MyApp");   
    RT_ASSERT(app != RT_NULL);   
    win = rtgui_win_create(RT_NULL, "MyWindow", &rect, RTGUI_WIN_STYLE_DEFAULT);   
    box = rtgui_box_create(RTGUI_HORIZONTAL, 10);   
    rtgui_container_set_box(RTGUI_CONTAINER(win), box);   
    rtgui_container_layout(RTGUI_CONTAINER(win));   
    rtgui_win_show(win, RT_FALSE);   
    rtgui_app_run(app);   
    rtgui_win_destroy(win);   
    rtgui_app_destroy(app);   
    rt_kprintf("MyApp Quit.\n");   
}   
   
int rt_gui_init(void)   
{   
    rt_device_t device;   
   
    //rt_hw_lcd_init();                        //在各lcd程序中已经用新的初始化方式初始化了   
    device = rt_device_find("lcd");   
    if (device == RT_NULL)   
    {   
        rt_kprintf("no graphic device in the system.\n");   
        return -1;   
    }   
   
    /* re-set graphic device */   
    rtgui_graphic_set_device(device);   
   
   
//    {   
//        extern void application_init();   
//        application_init();   
//    }   
   
//    /* add calibrate*/   
//    calibration_set_restore(cali_setup);   
//    calibration_set_after(cali_store);   
//    {   
//        extern void calibration_init(void);   
//        calibration_init();   
//    }   
     //Display ();   
    return 0;   
}   
//INIT_APP_EXPORT(rt_gui_init);



 