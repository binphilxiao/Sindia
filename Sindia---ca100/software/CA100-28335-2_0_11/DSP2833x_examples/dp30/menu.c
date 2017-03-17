#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
void display_menu(void);
void adjust(void);

Uint32 screen_cnt,ad_cnt,alarm_cnt,light_cnt,temp_cnt,flow_cnt,clock_cnt,dot_cnt,sht11_cnt,linetab,measure_second_cnt;
Uint16 run_screen_flag,run_second_flag,run_ad_flag,run_light_flag,run_half_flag,mirror_flag,run_temp_flag,run_flow_flag,run_clock_flag,run_dot_flag,run_sht11_flag;
Uint16 instruction_cnt,instruction_amount;

float humid_data[240];//240个实时数据缓存区
Uint16 ha100_dot_flag,ha100_dot_cnt,ha100_data_cnt,ha100_redo_flag;
struct TESTDATA ha100_store_data,ha100_hold_data;

struct MEASURE_TIME measure_time;
struct MEASURE_DATA_0 hold_data;

Uint16 hold_flag;
Uint16 calib_flag[3],store_number,gas;

Uint32 key_cnt;

void gpa100(void);


void parameter_initi(void)
{
        Uint16 *pt,j,x;
        float fp;

        x=fm22l16_rd(ADD_CHECK1);//验证仪器是否是第一次上电
        if(x!=0x5555)
        {
                fm22l16_wr(ADD_CHECK1,0x5555);
                fm22l16_wr(ADD_CNT,0);
                fm22l16_wr(ADD_GAS,0);
                fm22l16_wr(ADD_USB_NUM,0);
        }
        store_number=fm22l16_rd(ADD_CNT);
        gas=fm22l16_rd(ADD_GAS);


        h2o_1.flag=fm22l16_rd(ADD_CHECK2);//验证仪器是否校准过
        if(h2o_1.flag==0x5555)
        {
                fm22l16_rds(ADD_CALIB_H2O,(Uint16*)&h2o,24);
        }

        gpa_1.flag=fm22l16_rd(ADD_CHECK3);//验证仪器是否校准过
        if(gpa_1.flag==0x5555)
        {
                fm22l16_rds(ADD_CALIB_GPA,(Uint16*)&gpa,24);
        }
        jiaocha_SO2_flag = fm22l16_rd(JIAOCHA_SO2_FLAG);
        jiaocha_CO_flag  = fm22l16_rd(JIAOCHA_CO_FLAG);
        if( jiaocha_SO2_flag == 0x5555 )
        {
                fm22l16_float_rds(ADD_SO2_H2S,&jiaocha_SO2_k,1);
        }
        if( jiaocha_CO_flag == 0x5555 )
        {
                fm22l16_float_rds(ADD_CO_H2S,&jiaocha_CO_k,1);
        }
        u_key_ini();
        u_key_cnt=0;
        u_key_enter=0;
        key_cnt=0;
        hold_flag=0;
        kk3=0;

        run_sht11_flag=0;
        run_ad_flag=0;
        run_light_flag=0;
        run_temp_flag=0;
        temp_cnt=0;
        run_flow_flag=0;
        flow_cnt=0;
        run_clock_flag=0;
        clock_cnt=0;
        run_screen_flag=0;
        screen_cnt=0;
        sht11_cnt=0;

        ha100_dot_flag=0;
        ha100_redo_flag=0;
        ha100_dot_cnt=0;
        ha100_data_cnt=0;

        clean_flag=0;


        parameter.gas=fm22l16_rd(ADD_GAS);
        if(parameter.gas>1)parameter.gas=1;

        key_alarm_flag=0;
        key_alarm_cnt=0;

        fp32_ad_get(H2O);
        h2oold=ad8401_value[0]*(4.096/65536.0);

        fp32_ad_get(SO2);
        so2old=ad8401_value[0]*0.03125;//4.096/65536.0;

        fp32_ad_get(H2S);
        h2sold=ad8401_value[0]*0.03125;//4.096/65536.0;

        fp32_ad_get(HF);
        hfold=ad8401_value[0]*0.0625;//4.096/65536.0;

        fp32_ad_get(FLOW);
        flwold=ad8401_value[0]*(4.096/65536.0);


        measure_buffer_ini(SO2);
        measure_buffer_ini(H2S);
        measure_buffer_ini(HF);

        so2_measure_min=10;
        h2s_measure_min=50;
        hf_measure_min=5;
        so2_spy_min=5;
        h2s_spy_min=10;
        hf_spy_min=2;
        so2_zero_count=0;
        h2s_zero_count=0;
        hf_zero_count=0;

//      fm22l16_rds(ADD_CALIB1_H2O_K,(Uint16*)&h2o_1,39);
        fm22l16_rds(ADD_CALIB1_H2S_K,(Uint16*)&h2s_1,39);
        fm22l16_rds(ADD_CALIB1_HF_K,(Uint16*)&hf_1,39);
        fm22l16_rds(ADD_CALIB1_SO2_K,(Uint16*)&so2_1,39);
        fm22l16_rds(ADD_GPA_0,(Uint16*)&gpa_zero,2);

}

float display_sht11(void);
char *y_table[]= {"+40","+20","  0","-20","-40","-60","-80"};
void display_menu_0(void)
{
        Uint16 i,ver;


        color_change(0x001f,0x0000);
        cls();
        display_jxy(0,0,319,20);
        display_jxy(0,220,319,239);


        display_ascii(M12,0xffe0,0x0,1,0,207,"环境温度：");
        display_ascii(M12,0xffe0,0x0,1,148,207,"环境湿度：");
        display_ascii(M12,0xffe0,0x0,1,298,192,"L/H");
        color_change(0x07E0,0x0000);
        display_line(0,205,319,205);
        display_line(294,20,294,205);
//      display_line(295,189,319,189);

        if(instrument==0)
        {
                display_ascii(M16,0xffff,0x001f,1,0,222," 设置       查询        存储       调零");
                color_change(0xf800,0x0000);
                display_line(295,189,319,189);
                display_line(48,24,48,201);
                if (sub_ver==1)
                {
                        ver=1;
                }
                else
                {
                        display_ascii(M12,E_color,0x0,1,2,31,"露点");
                        display_ascii(M12,E_color,0x0,1,2,45,"温度 ：");
                        ver=2;
                }

                display_ascii(M12,E_color,0x0,1,2,31+46*(ver-1),"气体");
                display_ascii(M12,E_color,0x0,1,2,45+46*(ver-1),"纯度 ：");
                for(i=0; i<sensor_number; i++)
                {
                        if(sensor[i]==0xa0)
                        {
                                display_ascii(M12,E_color,0x0,1,4,31+46*(i+ver),"H2S");
                                display_ascii(M12,E_color,0x0,1,2,45+46*(i+ver),"含量 ：");
                        }
                        else if(sensor[i]==0xa4)
                        {
                                display_ascii(M12,E_color,0x0,1,4,31+46*(i+ver),"SO2");
                                display_ascii(M12,E_color,0x0,1,2,45+46*(i+ver),"含量 ：");
                        }
                        else if(sensor[i]==0xa8)
                        {
                                display_ascii(M12,E_color,0x0,1,4,31+46*(i+ver)," CO");
                                display_ascii(M12,E_color,0x0,1,2,45+46*(i+ver),"含量 ：");
                        }
                }
        }
        else if(instrument==1)
        {
                display_ascii(M16,0xffff,0x001f,1,0,222," 设置       查询        存储       保持");
                color_change(0xf800,0x0000);
                display_line(295,189,319,189);
                color_change(0x07E0,0x0000);
                display_line(0,80,293,80);
                display_line(0,140,293,140);
                display_ascii(M16,0xffff,0x0,1,5,42,"露点温度:");
                display_ascii(M16,0xffff,0x0,1,5,42+60,"微水含量:");
                display_ascii(M16,0xffff,0x0,1,5,42+120,"折算(+20℃):");

        }
        else if(instrument==2)
        {
                display_ascii(M16,0xffff,0x001f,1,0,222," 设置       查询        存储       调零");
                color_change(0xf800,0x0000);
                display_line(295,189,319,189);
                display_line(48,24,48,201);
                for(i=0; i<sensor_number; i++)
                {
                        if(sensor[i]==0xa0)
                        {
                                display_ascii(M12,E_color,0x0,1,4,33+65*(i),"H2S");
                                display_ascii(M12,E_color,0x0,1,2,45+65*(i),"含量：");
                        }
                        else if(sensor[i]==0xa4)
                        {
                                display_ascii(M12,E_color,0x0,1,4,33+65*(i),"SO2");
                                display_ascii(M12,E_color,0x0,1,2,45+65*(i),"含量：");
                        }
                        else if(sensor[i]==0xa8)
                        {
                                display_ascii(M12,E_color,0x0,1,4,33+65*(i)," CO");
                                display_ascii(M12,E_color,0x0,1,2,45+65*(i),"含量：");
                        }
                }
        }
        else if(instrument==3)
        {
                display_ascii(M16,0xffff,0x001f,1,0,222," 设置       查询        存储       调零");
                color_change(0xf800,0x0000);
                display_line(295,189,319,189);
                color_change(0xf81f,0x0000);
                display_jx(3,40,291,124);
        }
}

void data_get(struct MEASURE_DATA *data)
{
        Uint16 i;

        data->in8=in8;
        data->h2o_calib_value=h2o_calib_value;
        data->h2s_calib_value=h2s_calib_value;
        data->so2_calib_value=so2_calib_value;
        data->c_humid=c_humid;
        data->c_temp=c_temp_calib;
        rx8025_clock_read();
        for(i=0; i<7; i++)data->clock[i]=i2c[i];
        data->h2oppm=ppm_backup;
        data->h2oppm20=ppm_backup20;
        data->number[0]=0;
        data->number[1]=0;
        data->number[2]=0;
        data->number[3]=0;

}

void data_get_0(struct MEASURE_DATA_0 *data)
{
        Uint16 i;

        data->in8=in8;
        data->h2o_calib_value=h2o_calib_value;
        data->h2s_calib_value=h2s_calib_value;
        data->so2_calib_value=so2_calib_value;
        data->hf_calib_value =hf_calib_value;
        data->gpa_calib_value=gpa_calib_value;
        data->c_humid=c_humid;
        data->c_temp=c_temp_calib;
        rx8025_clock_read();
        for(i=0; i<7; i++)data->clock[i]=i2c[i];
        data->h2oppm=ppm_backup;
        data->h2oppm20=ppm_backup20;
        data->number[0]=0;
        data->number[1]=0;
        data->number[2]=0;
        data->number[3]=0;
        data->instrument=instrument;
        data->sensor[0]=sensor[0];
        data->sensor[1]=sensor[1];
        data->sensor[2]=sensor[2];
        data->sensor_number=sensor_number;

}

void ppm_clr_0(void)
{
        display_ascii(M24,0x0,0x00,1,56,21,"器器器器器器器器器器");
        color_change(0x07E0,0x0000);
        display_line(294,20,294,205);
}

void ca100(void);
void ha100(void);
void da100(void);

void menu(void)
{
        usb_check();
        if(instrument==0)ca100();
        else if(instrument==1)ha100();
        else if(instrument==2)da100();
        else if(instrument==3)gpa100();
        while(1);
}

void ha100(void)
{
        Uint16 error,i,kill=0,g,h,t=0;
start:
        //display_picture_all(1);
        hold_flag=0;
        km=1;
        display_menu_0();
        if(parameter.gas==0)
        {
                display_ascii(M16,0xffff,0x001f,1,244,1,"SF6");
        }
        else
        {
                display_ascii(M16,0xffff,0x001f,1,244,1,"N2 ");
        }

        key_clr();
        display_sht11();
        u_key_enter=0;
        u_key_cnt=0;
        while(1)
        {
                u_key_draw();
                if (run_ad_flag==1)
                {
                        run_ad_flag=0;
                }
                if (run_light_flag==1)
                {
                        run_light_flag=0;
                }
                if (run_temp_flag==1&&hold_flag==0)
                {
                        ha100_do(hold_flag);
                        run_temp_flag=0;
                }
                if (run_flow_flag==1)
                {
                        flow_do();
                        run_flow_flag=0;
                }
                if (run_clock_flag==1)
                {
                        display_clock(M16,0xffff,0x001f,1,5,0);
                        u_key_draw();
                        run_clock_flag=0;
                        t++;
                        if(t>=1)
                        {
                                t=0;
                                h++;
                                g=h&0x01;
                                display_battery(290,3,g);
                        }
                }
                if (run_sht11_flag==1)
                {
                        display_sht11();
                        run_sht11_flag=0;
                }
                if(ha100_dot_flag==1)
                {
                        ha100_line();
                }
                if(ha100_redo_flag==1)
                {
                        ha100_redo();
                }
                if (run_screen_flag==1)
                {
                        color_change(0xffff,0x0);
                        cls();
                        display_ascii(M16,0xf800,0x0,1,45,200,"**********兴迪仪器**********");
                        do
                        {
                                if (run_clock_flag==1)
                                {
                                        display_clock(M16,0xffe0,0x0,1,45,80);
                                        run_clock_flag=0;
                                }
                        }
                        while(run_screen_flag);
                        goto start;
                }
                if (keynew[0]+keynew[1]+keynew[2]+keynew[3]+keynew[4])
                {
                        if (keynew[0])
                        {
                                HEATER(1);
                                set();
                                goto start;
                        }
                        else if (keynew[1])
                        {
                                HEATER(1);
                                ha100_search();
                                goto start;
                        }
                        else if (keynew[2])
                        {
                                if(hold_flag==0)data_get_0(&store_data);
                                HEATER(1);
                                store_0();
                                goto start;
                        }
                        else if (keynew[3])
                        {

                                if(hold_flag==0)
                                {
                                        hold_flag=1;
                                        ha100_do(hold_flag);
                                        display_ascii(M16,0xf800,0x001f,1,280,222,"保持");
                                        data_get_0(&hold_data);
                                }
                                else
                                {
                                        hold_flag=0;
                                        display_ascii(M16,0xffff,0x001f,1,280,222,"保持");
                                }
                        }
                        key_clr();
                }
        }
}

Uint16 switch_h2o;
void ca100(void)
{
        Uint16 error,kill=0,g,h,t=0,i;
        hold_flag=0;
        switch_h2o=1;
start:
        //display_picture_all(1);
        km=1;
        display_menu_0();
        if(parameter.gas==0)display_ascii(M16,0xffff,0x001f,1,244,1,"SF6");
        else display_ascii(M16,0xffff,0x001f,1,244,1,"N2 ");
        key_clr();
        display_sht11();
        u_key_enter=0;
        u_key_cnt=0;
        while(1)
        {
                u_key_draw();
                if (run_ad_flag==1)
                {
                        run_ad_flag=0;
                }
                if (run_light_flag==1)
                {
                        run_light_flag=0;
                }
                if (run_temp_flag==1)
                {
                        if(sub_ver==0)
                        {
                                h2o_do_0(0);
                                ppm_do_0(h2o_calib_value*10.0,switch_h2o,0);
                                gpa_do_0(1);
                                for(i=0; i<sensor_number; i++)
                                {
                                        if(sensor[i]==0xa0)h2s_do_0(i+2);
                                        else if(sensor[i]==0xa4)so2_do_0(i+2);
                                        else if(sensor[i]==0xa8)hf_do_0(i+2);
                                        u_key_draw();
                                }
                        }
                        else if(sub_ver==1)
                        {
                                gpa_do_0(0);
                                for(i=0; i<sensor_number; i++)
                                {
                                        if(sensor[i]==0xa0)h2s_do_0(i+1);
                                        else if(sensor[i]==0xa4)so2_do_0(i+1);
                                        else if(sensor[i]==0xa8)hf_do_0(i+1);
                                        u_key_draw();
                                }
                        }
                        u_key_draw();
                        run_temp_flag=0;

                }
                if (run_flow_flag==1)
                {
                        flow_do();
                        run_flow_flag=0;
                }
                if (run_clock_flag==1)
                {
                        display_clock(M16,0xffff,0x001f,1,5,0);
                        u_key_draw();
                        run_clock_flag=0;
                        t++;
                        if(t>=1)
                        {
                                t=0;
                                h++;
                                g=h&0x01;
                                display_battery(290,3,g);
                        }
                }
                if (run_sht11_flag==1)
                {
                        display_sht11();
                        run_sht11_flag=0;
                }
                if (run_screen_flag==1)
                {
                        color_change(0xffff,0x0);
                        cls();
                        display_ascii(M16,0xf800,0x0,1,45,200,"**********兴迪仪器**********");
                        do
                        {
                                if (run_clock_flag==1)
                                {
                                        display_clock(M16,0xffe0,0x0,1,45,80);
                                        run_clock_flag=0;
                                }
                        }
                        while(run_screen_flag);
                        goto start;
                }
                if (keynew[0]+keynew[1]+keynew[2]+keynew[3]+keynew[4]+keynew[5])
                {
                        if (keynew[0])
                        {
                                HEATER(1);
                                set();
                                goto start;
                        }
                        else if (keynew[1])
                        {
                                HEATER(1);
                                search_0();
                                goto start;
                        }
                        else if (keynew[2])
                        {
                                data_get_0(&store_data);
                                HEATER(1);
                                store_0();
                                goto start;
                        }
                        else if (keynew[3])
                        {
                                zero_set_ca100();
                                goto start;
                        }
                        else if (keynew[5] && sub_ver==0)
                        {
                                if(switch_h2o==0)switch_h2o=1;
                                else switch_h2o=0;
                                ppm_do_0(h2o_calib_value*10.0,switch_h2o,0);
                                //      goto start;
                        }
                        key_clr();
                }
        }
}
void da100(void)
{
        Uint16 error,i,kill=0,g,h,t=0;
start:
        //display_picture_all(1);
        hold_flag=0;
        km=1;
        display_menu_0();
        if(parameter.gas==0)
        {
                display_ascii(M16,0xffff,0x001f,1,244,1,"SF6");
        }
        else
        {
                display_ascii(M16,0xffff,0x001f,1,244,1,"N2 ");
        }
        key_clr();
        display_sht11();
        u_key_enter=0;
        u_key_cnt=0;
        while(1)
        {
                u_key_draw();
                if (run_ad_flag==1)
                {
                        run_ad_flag=0;
                }
                if (run_light_flag==1)
                {
                        run_light_flag=0;
                }
                if (run_temp_flag==1&&hold_flag==0)
                {
                        for(i=0; i<sensor_number; i++)
                        {
                                if(sensor[i]==0xa0)
                                {
                                        h2s_do_1(i);
                                }
                                else if(sensor[i]==0xa4)
                                {
                                        so2_do_1(i);
                                }
                                else if(sensor[i]==0xa8)
                                {
                                        hf_do_1(i);
                                }
                                u_key_draw();
                        }
                        run_temp_flag=0;

                }
                if (run_flow_flag==1)
                {
                        flow_do();
                        run_flow_flag=0;
                }
                if (run_clock_flag==1)
                {
                        display_clock(M16,0xffff,0x001f,1,5,0);
                        u_key_draw();
                        run_clock_flag=0;
                        t++;
                        if(t>=1)
                        {
                                t=0;
                                h++;
                                g=h&0x01;
                                display_battery(290,3,g);
                        }
                }
                if (run_sht11_flag==1)
                {
                        display_sht11();
                        run_sht11_flag=0;
                }
                if (run_screen_flag==1)
                {
                        color_change(0xffff,0x0);
                        cls();
                        display_ascii(M16,0xf800,0x0,1,45,200,"**********兴迪仪器**********");
                        do
                        {
                                if (run_clock_flag==1)
                                {
                                        display_clock(M16,0xffe0,0x0,1,45,80);
                                        run_clock_flag=0;
                                }
                        }
                        while(run_screen_flag);
                        goto start;
                }
                if (keynew[0]+keynew[1]+keynew[2]+keynew[3]+keynew[4])
                {
                        if (keynew[0])
                        {
                                HEATER(1);
                                set();
                                goto start;
                        }
                        else if (keynew[1])
                        {
                                HEATER(1);
                                search_1();
                                goto start;
                        }
                        else if (keynew[2])
                        {
                                if(hold_flag==0)data_get_0(&store_data);
                                HEATER(1);
                                store_0();
                                goto start;
                        }
                        else if (keynew[3])
                        {
                                zero_set_da100();
                                goto start;
                        }
                        key_clr();
                }
        }
}

void gpa100(void)
{
        Uint16 error,i,kill=0,g,h,t=0;
start:
        //display_picture_all(1);
        hold_flag=0;
        km=1;
        display_menu_0();
        if(parameter.gas==0)display_ascii(M16,0xffff,0x001f,1,244,1,"SF6");
        else display_ascii(M16,0xffff,0x001f,1,244,1,"N2 ");
        key_clr();
        display_sht11();
        u_key_enter=0;
        u_key_cnt=0;
        while(1)
        {
                u_key_draw();
                if (run_ad_flag==1)
                {
                        run_ad_flag=0;
                }
                if (run_light_flag==1)
                {
                        run_light_flag=0;
                }
                if (run_temp_flag==1&&hold_flag==0)
                {
                        //ha100_do(hold_flag);

                        run_temp_flag=0;
                        gpa100_do();

                }
                if (run_flow_flag==1)
                {
                        flow_do();
                        run_flow_flag=0;
                }
                if (run_clock_flag==1)
                {
                        display_clock(M16,0xffff,0x001f,1,5,0);
                        u_key_draw();
                        run_clock_flag=0;
                        t++;
                        if(t>=1)
                        {
                                t=0;
                                h++;
                                g=h&0x01;
                                display_battery(290,3,g);
                        }
                }
                if (run_sht11_flag==1)
                {
                        display_sht11();
                        run_sht11_flag=0;
                }
                if(ha100_dot_flag==1)
                {
                        // ha100_line();
                }
                if(ha100_redo_flag==1)
                {
                        // ha100_redo();
                }
                if (run_screen_flag==1)
                {
                        color_change(0xffff,0x0);
                        cls();
                        display_ascii(M16,0xf800,0x0,1,45,200,"**********兴迪仪器**********");
                        do
                        {
                                if (run_clock_flag==1)
                                {
                                        display_clock(M16,0xffe0,0x0,1,45,80);
                                        run_clock_flag=0;
                                }
                        }
                        while(run_screen_flag);
                        goto start;
                }
                if (keynew[0]+keynew[1]+keynew[2]+keynew[3]+keynew[4])
                {
                        if (keynew[0])
                        {
                                HEATER(1);
                                set();
                                goto start;
                        }
                        else if (keynew[1])
                        {
                                HEATER(1);
                                gpa100_search();
                                goto start;
                        }
                        else if (keynew[2])
                        {
                                data_get_0(&store_data);
                                store_0();
                                goto start;
                        }
                        else if (keynew[3])
                        {
                                zero_set_gpa100();
                                goto start;
                        }
                        key_clr();
                }
        }
}

float display_sht11(void)
{
        Uint16 ret;

        ret=sht71_ht(&c_humid,&c_temp);
        if (ret!=0)
        {
                c_temp=100.0;
                c_humid=100.0;
        }

        temperature_flag = fm22l16_rd(TEMPERATURE_FLAG) ;
        if( temperature_flag == 0x5555 )
        {
                fm22l16_rds(TEMP_MEASURE_INPUT,(Uint16*)in1,4);
                fm22l16_rds(TEMP_REAL_INPUT,(Uint16*)in2,4);

                if (in1[0]!=in1[1])
                        c_temp_calib=(in2[1]-in2[0])*(c_temp-in1[0])/(in1[1]-in1[0])+in2[0];
                else
                        c_temp_calib=c_temp;
        }
        else
                c_temp_calib=c_temp;
        store_data.c_temp=c_temp_calib;


        display_fp_ascii(M12,0xffff,0x0000,1,60,207,c_temp_calib,"℃");//x<999.999
        display_fp_ascii(M12,0xffff,0x0000,1,200,207,c_humid,"%");//x<999.999
}













