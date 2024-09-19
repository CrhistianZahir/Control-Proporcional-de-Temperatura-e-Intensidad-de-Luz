#include <16f877a.h>
#device adc=10  //Resolución de 10 bits
#fuses XT, NOWDT
#use delay (clock= 4MHz)
#include <lcd.c>
#include <kbd.c>
#include <stdlib.h>
#include <math.h>
#use standard_io(c)
#use standard_io(a)
#use standard_io(b)

void conversion();
void pwm1();
void ingreso();
void ingreso1();
void ingreso2();
void actuadores();
//declaración de variables
float volt, temp, R2,temp2;
float R1=10000.0;
float A=0.001129148, B=0.000234125, C=0.0000000876741;  
long valor1,valor2,valor,valor3;
int setpoint;
float calc1,calc2;
int tempf,ldr;
int Timer2,Poscaler;
int u,v;
char ingresado[3];
char ingresado1[3];
char ingresado2[3];

void main(){
setpoint=(READ_EEPROM(0)*10)+READ_EEPROM(1);
     lcd_init();
     port_b_pullups(true);
     kbd_init();
     setup_adc_ports(AN0_AN1_AN3);
     setup_adc(ADC_CLOCK_INTERNAL);
     //*+lcd_putc('\f');
     printf(lcd_putc,"BIENVENIDO\n");
     lcd_putc("Laboratorio 2");
     delay_ms(500);
     //señal cuadrada con frecuencia 1khz
     Timer2=255;
     //preescaler=4,t2_div_by_4
     Poscaler=1;
     setup_timer_2(t2_div_by_4,Timer2,Poscaler);
     setup_CCP1(CCP_PWM);
     setup_CCP2(CCP_PWM);         
     while(TRUE){
     set_adc_channel(1);
     delay_us(20);
     ldr=read_adc();
     delay_ms(100);
     if(ldr>150){
     output_high(PIN_C0);
     }
     else output_low(PIN_C0);
     delay_ms(100);
     if(input(PIN_A4)==1){
     ingreso2();
     } 
     if(input(PIN_A2)==1){
     pwm1();
     }
     actuadores();
     }    
}
void conversion(){
     set_adc_channel(0);
     delay_us(20);
     volt=read_adc();
     R2=R1*((1023.0/volt)-1.0);//CALCULO DE LA RESISTENCIA DEL TERMISTOR:R2
     float logR2 = log(R2);
     temp=1.0/(A+(B*logR2)+(C*(logR2*logR2*logR2)));//ecuación de steinhart-hart
     temp=temp-273.15;//conversión de kelvin a celcius
     tempf=temp/1;
}
void ingreso(){
     int i=0;
     char k;
     while(true){
     while(i<4){
     k=kbd_getc();
     printf(lcd_putc,"          sp:%d ",k);
     if(k!=0){
       ingresado[i]=k;
       i++;
       lcd_putc(k);
       }
     }
   }
}
void actuadores(){
     conversion();
     if (temp<setpoint)
        {
        calc1=0;
        temp2=setpoint-4;
        valor2=((-255.75*(temp-setpoint+4))+1023)/1;
        calc2=valor2*100.0/1023.0;
        set_pwm2_duty(valor2);
        set_pwm1_duty(0);
        if (temp<temp2)
        {
        valor2=1023;
        set_pwm2_duty(valor2);
        calc2=99.0;
        }
        }
     if (temp>setpoint)
        {
        calc2=0;
        valor1=(255.75*(temp-setpoint))/1;
        calc1=valor1*100.0/1023.0;
        set_pwm1_duty(valor1);
        set_pwm2_duty(0); 
        if (temp>(setpoint+4))
        {
        valor1=1023;
        set_pwm1_duty(valor1);
        calc1=99.0;
        }
        }
        lcd_putc('\f');
        printf(lcd_putc,"T:%d      sp:%d \nd1:%01.2f d2:%01.2f",tempf,setpoint,calc1,calc2);
        delay_ms(200);    
}
void ingreso1(){
   int i=0;
   char k,z;
   while(true){
   lcd_gotoxy(1,1);
   lcd_putc("\fduty cicle:\n");
   delay_ms(500);
   lcd_putc("\f");
   lcd_gotoxy(1,1);
   printf(lcd_putc,"d1:\n");
   lcd_gotoxy(11,1);
   printf(lcd_putc,"d2:\n");
   while(i<2){
         k=kbd_getc();
         if(k!=0){
         ingresado[i]=k;
         i++;
         lcd_putc(k);
         }
         if(k=='#'){ 
         break;
         }
         }
         i=0;
         int x=10;
         while(i<=2){
         z=kbd_getc();
         if(z!=0){
         ingresado1[i]=z;
         i++;
         x++;
         lcd_gotoxy(x,2);
         lcd_putc(z);
         }
         if(z=='#'){ 
         break;
         }
         }
         lcd_putc('\f');
         break;
  }
}
void pwm1(){
     while(1){
     ingreso1();
     delay_ms(500);
     lcd_putc('\f');
     long int x = atoi(ingresado);
     long int y= atoi(ingresado1);
     valor=x/100;
     valor=valor*1023;
     valor3=y/100;
     valor3=valor3*1023;
     float calc=x*1023.0/100.0;
     float calcx=y*1023.0/100.0;
     valor=calc/1;
     valor3=calcx/1;
     set_pwm1_duty(valor);
     set_pwm2_duty(valor3);
     while(input(PIN_A2)==1){break;}
     while(input(PIN_A3)==1) break;
     }
}

void ingreso2(){
      int i=0;
      char k,z;
      while(true){
      lcd_gotoxy(1,1);
      lcd_putc("\fsetpoint:\n");
      delay_ms(500);
   
      while(i<3){
      k=kbd_getc();
         if(k!=0){
         ingresado2[i]=k;
         i++;
         lcd_putc(k);
         }
         if(k=='#'){ 
         break;
         }
      }
      u=ingresado2[0]-48;
      v=ingresado2[1]-48;
      WRITE_EEPROM(0,u);
      WRITE_EEPROM(1,v);     
  }
}
