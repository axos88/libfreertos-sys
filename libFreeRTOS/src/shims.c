/*
FreeRTOS.rs shim library

Include headers relevant for your platform.

STM32 example:

#include "stm32f4xx_hal.h"

*/

#include "xFreeRTOS.h"
#include <stdint.h>

//uint8_t freertos_rs_sizeof(uint8_t _type) {
//    switch (_type) {
//        case 0:
//            return sizeof(void*);
//            break;
//        case 1:
//            return sizeof(char*);
//            break;
//        case 2:
//            return sizeof(char);
//            break;
//
//        case 10:
//            return sizeof(portBASE_TYPE);
//            break;
//        case 11:
//            return sizeof(unsigned portBASE_TYPE);
//            break;
//        case 12:
//            return sizeof(short);
//            break;
//
//        case 20:
//            return sizeof(xTaskHandle);
//            break;
//        case 21:
//            return sizeof(xQueueHandle);
//            break;
//        case 22:
//            return sizeof(SemaphoreHandle_t);
//            break;
//        case 23:
//            return sizeof(pdTASK_CODE);
//            break;
//        case 24:
//            return sizeof(TimerHandle_t);
//            break;
//        case 25:
//            return sizeof(TimerCallbackFunction_t);
//            break;
//
//        case 30:
//            return sizeof(TaskStatus_t);
//            break;
//        case 31:
//            return sizeof(eTaskState);
//            break;
//        case 32:
//            return sizeof(unsigned long);
//            break;
//        case 33:
//            return sizeof(unsigned short);
//            break;
//
//
//            break;
//        default:
//            return 0;
//    }
//}

#if (INCLUDE_vTaskDelayUntil == 1)
void freertos_rs_vTaskDelayUntil(portTickType *pxPreviousWakeTime, short xTimeIncrement) {
	vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement);
}
#endif

#if (INCLUDE_vTaskDelay == 1)
void freertos_rs_vTaskDelay(short xTicksToDelay) {
	vTaskDelay(xTicksToDelay);
}
#endif

short freertos_rs_xTaskGetTickCount() {
    return xTaskGetTickCount();
}

#if (configUSE_TRACE_FACILITY == 1)
unsigned portBASE_TYPE freertos_rs_get_system_state(TaskStatus_t * const pxTaskStatusArray, const unsigned portBASE_TYPE uxArraySize, uint32_t * const pulTotalRunTime) {
	return uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, pulTotalRunTime);
}
#endif

short freertos_rs_get_portTICK_PERIOD_MS() {
    return portTICK_RATE_MS;
}

unsigned portBASE_TYPE freertos_rs_get_number_of_tasks() {
    return uxTaskGetNumberOfTasks();
}

#if (configUSE_RECURSIVE_MUTEXES == 1)
xQueueHandle freertos_rs_create_recursive_mutex() {
	return xSemaphoreCreateRecursiveMutex();
}

unsigned portBASE_TYPE freertos_rs_take_recursive_mutex(xQueueHandle mutex, unsigned portBASE_TYPE max) {
	if (xSemaphoreTakeRecursive(mutex, max) == pdTRUE) {
		return 0;
	}

	return 1;
}
unsigned portBASE_TYPE freertos_rs_give_recursive_mutex(xQueueHandle mutex) {
	if (xSemaphoreGiveRecursive(mutex) == pdTRUE) {
		return 0;
	} else {
		return 1;
	}
}
#endif

xQueueHandle freertos_rs_create_mutex() {
    return xSemaphoreCreateMutex();
}

xQueueHandle freertos_rs_create_binary_semaphore() {
    xQueueHandle s;
    vSemaphoreCreateBinary(s);
    return s;
}

xQueueHandle freertos_rs_create_counting_semaphore(unsigned portBASE_TYPE max, unsigned portBASE_TYPE initial) {
    return xSemaphoreCreateCounting(max, initial);
}

void freertos_rs_delete_semaphore(xQueueHandle semaphore) {
    vQueueDelete(semaphore);
}

unsigned portBASE_TYPE freertos_rs_take_mutex(xQueueHandle mutex, unsigned portBASE_TYPE max) {
    if (xSemaphoreTake(mutex, max) == pdTRUE) {
        return 0;
    }

    return 1;
}

unsigned portBASE_TYPE freertos_rs_give_mutex(xQueueHandle mutex) {
    if (xSemaphoreGive(mutex) == pdTRUE) {
        return 0;
    }

    return 1;
}

unsigned portBASE_TYPE freertos_rs_take_semaphore_isr(xQueueHandle semaphore, portBASE_TYPE* xHigherPriorityTaskWoken) {
    if (xQueueReceiveFromISR(semaphore, NULL, xHigherPriorityTaskWoken) == pdTRUE) {
        return 0;
    }

    return 1;
}

unsigned portBASE_TYPE freertos_rs_give_semaphore_isr(xQueueHandle semaphore, portBASE_TYPE* xHigherPriorityTaskWoken) {
    if (xSemaphoreGiveFromISR(semaphore, xHigherPriorityTaskWoken) == pdTRUE) {
        return 0;
    }

    return 1;
}


unsigned portBASE_TYPE freertos_rs_spawn_task(pdTASK_CODE entry_point, void* pvParameters, const char * const name, uint8_t name_len, uint16_t stack_size, unsigned portBASE_TYPE priority, xTaskHandle* task_handle) {
    char c_name[configMAX_TASK_NAME_LEN] = {0};
    for (int i = 0; i < name_len; i++) {
        c_name[i] = name[i];

        if (i == configMAX_TASK_NAME_LEN - 1) {
            break;
        }
    }

    portBASE_TYPE ret = xTaskCreate(entry_point, c_name, stack_size, pvParameters, priority, task_handle);

    if (ret != pdPASS) {
        return 1;
    }

    return 0;
}

#if (INCLUDE_vTaskDelete == 1)
void freertos_rs_delete_task(xTaskHandle task) {
	vTaskDelete(task);
}
#endif

unsigned portBASE_TYPE freertos_rs_get_stack_high_water_mark(xTaskHandle task) {
#if (INCLUDE_uxTaskGetStackHighWaterMark == 1)
    return uxTaskGetStackHighWaterMark(task);
#else
    return 0;
#endif
}


xQueueHandle freertos_rs_queue_create(unsigned portBASE_TYPE queue_length, unsigned portBASE_TYPE item_size) {
    return xQueueCreate(queue_length, item_size);
}

void freertos_rs_queue_delete(xQueueHandle queue) {
    vQueueDelete(queue);
}

unsigned portBASE_TYPE freertos_rs_queue_send(xQueueHandle queue, void* item, short max_wait) {
    if (xQueueSend(queue, item, max_wait ) != pdTRUE)
    {
        return 1;
    }

    return 0;
}

unsigned portBASE_TYPE freertos_rs_queue_send_isr(xQueueHandle queue, void* item, portBASE_TYPE* xHigherPriorityTaskWoken) {
    if (xQueueSendFromISR(queue, item, xHigherPriorityTaskWoken) == pdTRUE) {
        return 0;
    }
    return 1;
}

unsigned portBASE_TYPE freertos_rs_queue_receive(xQueueHandle queue, void* item, short max_wait) {
    if ( xQueueReceive( queue, item, max_wait ) != pdTRUE )
    {
        return 1;
    }

    return 0;
}

void freertos_rs_isr_yield() {
    portYIELD();
}

short freertos_rs_max_wait() {
    return portMAX_DELAY;
}

#if (INCLUDE_pcTaskGetTaskName == 1)
char* freertos_rs_task_get_name(xTaskHandle task) {
	return pcTaskGetTaskName(task);
}
#endif

//uint32_t freertos_rs_task_notify_take(uint8_t clear_count, short wait) {
//    return ulTaskNotifyTake(clear_count == 1 ? pdTRUE : pdFALSE, wait);
//}
//
//portBASE_TYPE freertos_rs_task_notify_wait(uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pulNotificationValue, short xTicksToWait) {
//    if (xTaskNotifyWait(ulBitsToClearOnEntry, ulBitsToClearOnExit, pulNotificationValue, xTicksToWait) == pdTRUE) {
//        return 0;
//    }
//
//    return 1;
//}

//eNotifyAction freertos_rs_task_notify_action(uint8_t action) {
//    switch (action) {
//        case 1:
//            return eSetBits;
//        case 2:
//            return eIncrement;
//        case 3:
//            return eSetValueWithOverwrite;
//        case 4:
//            return eSetValueWithoutOverwrite;
//        default:
//            return eNoAction;
//    }
//}

//portBASE_TYPE freertos_rs_task_notify(void* task, uint32_t value, uint8_t action) {
//    eNotifyAction eAction = freertos_rs_task_notify_action(action);
//
//    portBASE_TYPE v = xTaskNotify(task, value, eAction);
//    if (v != pdPASS) {
//        return 1;
//    }
//    return 0;
//}

//portBASE_TYPE freertos_rs_task_notify_isr(void* task, uint32_t value, uint8_t action, portBASE_TYPE* xHigherPriorityTaskWoken) {
//    eNotifyAction eAction = freertos_rs_task_notify_action(action);
//
//    portBASE_TYPE v = xTaskNotifyFromISR(task, value, eAction, xHigherPriorityTaskWoken);
//    if (v != pdPASS) {
//        return 1;
//    }
//    return 0;
//}

#if ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) )
xTaskHandle freertos_rs_get_current_task() {
	return xTaskGetCurrentTaskHandle();
}
#endif

#if (configUSE_TIMERS == 1)

TimerHandle_t freertos_rs_timer_create(const char * const name, uint8_t name_len, const short period,
		uint8_t auto_reload, void * const timer_id, TimerCallbackFunction_t callback)
{
	char c_name[configMAX_TASK_NAME_LEN] = {0};
	for (int i = 0; i < name_len; i++) {
		c_name[i] = name[i];

		if (i == configMAX_TASK_NAME_LEN - 1) {
			break;
		}
	}

	unsigned portBASE_TYPE timer_auto_reload = pdFALSE;
	if (auto_reload == 1) {
		timer_auto_reload = pdTRUE;
	}

	TimerHandle_t handle = xTimerCreate(c_name, period, timer_auto_reload, timer_id, callback);
	return handle;
}

portBASE_TYPE freertos_rs_timer_start(TimerHandle_t timer, short block_time) {
	if (xTimerStart(timer, block_time) != pdPASS) {
		return 1;
	}
	return 0;
}

portBASE_TYPE freertos_rs_timer_stop(TimerHandle_t timer, short block_time) {
	if (xTimerStop(timer, block_time) != pdPASS) {
		return 1;
	}
	return 0;
}

portBASE_TYPE freertos_rs_timer_delete(TimerHandle_t timer, short block_time) {
	if (xTimerDelete(timer, block_time) != pdPASS) {
		return 1;
	}
	return 0;
}

portBASE_TYPE freertos_rs_timer_change_period(TimerHandle_t timer, short block_time, short new_period) {
	if (xTimerChangePeriod(timer, new_period, block_time) != pdPASS) {
		return 1;
	}
	return 0;
}

void* freertos_rs_timer_get_id(TimerHandle_t timer) {
	return pvTimerGetTimerID(timer);
}

#endif

void freertos_rs_enter_critical() {
    taskENTER_CRITICAL();
}

void freertos_rs_exit_critical() {
    taskEXIT_CRITICAL();
}