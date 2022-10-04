#include "FreeRTOS.h"
#include "task.h"


void Task1(void)
{
    while (1)
    {
        printf("This is task 1 \n");
         vTaskDelay(100);
    }
    
    
    
}

void Task2(void)
{
    while (1)
    {
        printf("This is task 2 \n");
        vTaskDelay(500);
    }
    
    

}
int main(void)
{
    xTaskHandle HT ;
    xTaskCreate(Task1,"HelloTask",1000,NULL,3,&HT);
    xTaskCreate(Task2,"HelloTask",100,NULL,1,&HT);
    vTaskStartScheduler();
    for(;;);
    return 0;
}