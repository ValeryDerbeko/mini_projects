/*
 * awaitAll.c
 *
 *  Created on: Apr 5, 2023
 *      Author: Валерий
 */
#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
extern void doubleFunc();

struct argsForAwaitProcess {
	TaskHandle_t TaskHandle;
	SemaphoreHandle_t Semaphore;
	int input;
	int output;
};

int* awaitAll(struct argsForAwaitAll args[], int length) {

	struct argsForAwaitProcess *argsForProcess[length];

	//перебираем элементы полученного массива
	for (int i = 0; i < length; i++) {
		//создаем структуру аргументов для новой задачи
		argsForProcess[i] = (struct argsForAwaitProcess*) pvPortMalloc(
				length * sizeof(struct argsForAwaitProcess));

		//закладыываем входные данные в argsForProcess
		argsForProcess[i]->input = args[i].argument;

		//создаем бинарный семафор
		argsForProcess[i]->Semaphore = xSemaphoreCreateBinary();

		//создаем новую задачу
		xTaskCreate(args[i].function, NULL, configMINIMAL_STACK_SIZE,
				argsForProcess[i], 2, argsForProcess[i]->TaskHandle);

	}

/*	xSemaphoreTake(argsForProcess[0]->Semaphore, portMAX_DELAY);

	xSemaphoreTake(argsForProcess[1]->Semaphore, portMAX_DELAY);*/

	//захватываем поочередно все семафоры для каждой задачи, т.е. будем ждать пока выполняться все задачи
	 for (int i = 0; i < length; i++) {
	 xSemaphoreTake(argsForProcess[i]->Semaphore, portMAX_DELAY);
	 }

	//эта часть кода будет работать после того, как завершиться выполнение остальных потоков

	//выделяем память для массива результатов
	int *results = (int*) pvPortMalloc(length * sizeof(int));

	for (int i = 0; i < length; i++) {
		//заносим в массив результаты выполнения потоков
		results[i] = argsForProcess[i]->output;

		//удаление семафора
		vSemaphoreDelete(argsForProcess[i]->Semaphore);

		//очистка выделенной памяти для аргумента потока
		vPortFree(argsForProcess[i]);
	}

	return results;
}
