#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>

xTaskHandle priorityset_handle=NULL;
xTaskHandle matrix_handle = NULL;
xTaskHandle communication_handle = NULL;
#define SIZE 10
#define ROW SIZE
#define COL SIZE
 uint32_t Count = 0;
 uint32_t CountMatrix = 0;
boolean  active = FALSE; 

void vApplicationTickHook( void );
static void matrixtask(void);
static void communication_task(void);
static void prioritysettask(void);


int main(void)
{

 /* This demo uses heap_5.c, so start by defining some heap regions.  heap_5
	is only used for test and example reasons.  Heap_4 is more appropriate.  See
	http://www.freertos.org/a00111.html for an explanation. */
	//prvInitialiseHeap();

	/* Initialise the trace recorder.  Use of the trace recorder is optional.
	See http://www.FreeRTOS.org/trace for more information. */
	xTaskCreate((pdTASK_CODE)matrixtask, (signed char *)"Matrix", 1000, NULL, 3, &matrix_handle);
	xTaskCreate((pdTASK_CODE)communication_task, (signed char *)"Communication", configMINIMAL_STACK_SIZE, NULL, 1, &communication_handle);
	xTaskCreate((pdTASK_CODE)prioritysettask, (signed char *)"Priorityset", configMINIMAL_STACK_SIZE, NULL, 2 , &priorityset_handle);

    vTaskStartScheduler();
    for(;;);
    return 0;

}


static void matrixtask(void)
{
	printf("Entering Matrix Task \n");
   int i;
	double **a = (double **)pvPortMalloc(ROW * sizeof(double*));
	for (i = 0; i < ROW; i++) a[i] = (double *)pvPortMalloc(COL * sizeof(double));
	double **b = (double **)pvPortMalloc(ROW * sizeof(double*));
	for (i = 0; i < ROW; i++) b[i] = (double *)pvPortMalloc(COL * sizeof(double));
	double **c = (double **)pvPortMalloc(ROW * sizeof(double*));
	for (i = 0; i < ROW; i++) c[i] = (double *)pvPortMalloc(COL * sizeof(double));

	double sum = 0.0;
	int j, k, l;

	for (i = 0; i < SIZE; i++) {
		for (j = 0; j < SIZE; j++) {
			a[i][j] = 1.5;
			b[i][j] = 2.6;
		}
	}

	while (1) {
		/*
		* In an embedded systems, matrix multiplication would block the CPU for a long time
		* but since this is a PC simulator we must add one additional dummy delay.
		*/
		long simulationdelay;
		for (simulationdelay = 0; simulationdelay<1000000000; simulationdelay++);
		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				c[i][j] = 0.0;
			}
		}

		for (i = 0; i < SIZE; i++) {
			for (j = 0; j < SIZE; j++) {
				sum = 0.0;
				for (k = 0; k < SIZE; k++) {
					for (l = 0; l<10; l++) {
						sum = sum + a[i][k] * b[k][j];
					}
				}
				c[i][j] = sum;
			}
		}
		vTaskDelay(100);
		printf("Matrix Count is -- %d\n",CountMatrix);     //print the no of ticks here to see the period of this task
		fflush(stdout);
		CountMatrix= 0;
	}
}

static void communication_task()
{
	while (1) {
		int priority = uxTaskPriorityGet(communication_handle);
		printf("Priority now is %d \n",priority);
		fflush(stdout);
		Count = 0;
		active = TRUE ;
		printf("Sending data...\n");
		fflush(stdout);
		vTaskDelay(100);
		printf("Data sent!\n");
		fflush(stdout);
		vTaskDelay(100);
		printf("Count is -- %d\n", Count);
		fflush(stdout);
		active = FALSE ;
	}
}

static void prioritysettask()
{

	while(1)
	{
		if(Count > 200)
		{
			printf("increasing priority to communication task to 4\n");
			fflush(stdout);
			vTaskPrioritySet(communication_handle, 4);
		}
		else if (Count <100)
		{
			printf("increasing priority of the communication task to 2\n");
			fflush(stdout);
			vTaskPrioritySet(communication_handle, 2);
		}
		vTaskDelay(200);
	}
}

void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
	CountMatrix++;
	Count++;
	// #if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	// {
	// 	vFullDemoTickHookFunction();
	// }
	// #endif /* mainCREATE_SIMPLE_BLINKY_DEMO_ONLY */
}