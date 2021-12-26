/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

CY_ISR(int_reloj);
CY_ISR(int_seleccionar);
CY_ISR(int_ajustar);
CY_ISR(int_Alarma);
void parpadeo(void);

unsigned char cont100ms = 0;
unsigned char seg = 0;
unsigned char min = 10;
unsigned char hora = 10;
unsigned char option = 0;
unsigned char hora_alarma= 05;
unsigned char min_alarma = 00;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_reloj_StartEx(int_reloj);
    isr_seleccionar_StartEx(int_seleccionar);
    isr_ajustar_StartEx(int_ajustar);
    
    Counter_Start();
    Display7segmentos_Start();
    Display7segmentos_Write7SegNumberDec((hora*100+min),0,4,Display7segmentos_ZERO_PAD);
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        if(seg == 60){
            seg = 0;
            min++;
            Display7segmentos_Write7SegNumberDec((hora*100+min),0,4,Display7segmentos_ZERO_PAD);
            if (min == 60){
                hora++;
                min = 0;
                if(hora == 24){
                    hora = 0;
                }
            }    
        }
        
        if (option == 0){       // Visualizar Hora
            Leds_Write(0b0000);
            Display7segmentos_Write7SegNumberDec((hora*100+min),0,4,Display7segmentos_ZERO_PAD);
        }
        if (option == 1){       //  Ajustar minutos
            parpadeo();
            Leds_Write(0b0011);
            Display7segmentos_Write7SegNumberDec((hora*100+min),0,4,Display7segmentos_ZERO_PAD);
        }
        if (option == 2){       //  Ajustar horas
            parpadeo();
            Leds_Write(0b1100);
            Display7segmentos_Write7SegNumberDec((hora*100+min),0,4,Display7segmentos_ZERO_PAD);
        }
        if (option==3){         // Ajustar minutos alarma
            Leds_Write(0b0011);
            Display7segmentos_Write7SegNumberDec((hora_alarma*100+min_alarma),0,4,Display7segmentos_ZERO_PAD);
        }
        if (option==4){         // Ajustar horas alarma
            Leds_Write(0b1100);
            Display7segmentos_Write7SegNumberDec((hora_alarma*100+min_alarma),0,4,Display7segmentos_ZERO_PAD);
        }
        if (hora==hora_alarma && min==min_alarma)Leds_Write(0b1111);;
    }
}

CY_ISR(int_reloj){
    cont100ms++;
    if (cont100ms == 10){
        cont100ms = 0;
        seg++;
        P2_Write(!P2_Read());             
    }
}

CY_ISR(int_seleccionar){        // Cambio de estados
    option++;
    if (option == 5)
    {
        option=0;
    }
}
CY_ISR(int_ajustar)
{        
    if (option == 1)                    // Sumar minutos
    {
        min++; 
        Leds_Write(0b0011);
        if (min==60)
        {
            min=0;
        }
    }
    
    if (option == 2)                    // Sumar horas
    {
        hora++;
        Leds_Write(0b1100);
        if (hora==24)
        {
            hora=0;
        }
    }
    if (option == 3)
    {
        min_alarma++;
        if (min_alarma==60)
        {
            min_alarma=0;
        }
    }
    
    if (option == 4)
    {
        hora_alarma++;
        if (hora_alarma==24)
        {
            hora_alarma=0;
        }

    } 
}
void parpadeo(void){
    Display7segmentos_Sleep();
    CyDelay(350);
    Display7segmentos_Wakeup();
    CyDelay(350);  
}
/* [] END OF FILE */

// DUDAS
//Display7segmentos_ToggleRC(4,4);               // como funcionan los argumentos que recibe toggle?
// Puede encenderse el buzzer sin usar un pwm?