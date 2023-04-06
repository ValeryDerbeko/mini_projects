/*
 * triple.c
 *
 *  Created on: Apr 5, 2023
 *      Author: Валерий
 */
#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"

struct argsForAwaitProcess {
	TaskHandle_t TaskHandle;
	SemaphoreHandle_t Semaphore;
	int input;
	int output;
};

void tripleFunc(struct argsForAwaitProcess *argsForProcess) {
	while (1) {
		vTaskDelay(pdMS_TO_TICKS(3000));
		argsForProcess->output = 3 * argsForProcess->input;
		xSemaphoreGive(argsForProcess->Semaphore);
		vTaskDelete(NULL);
	}
}
;
