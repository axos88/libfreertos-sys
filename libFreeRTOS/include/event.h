/*
 *		FreeRTOS Evnet Hanlding Mechanism - Copyright (C) 2011 by Vandra Ákos
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published
 *    by the Free Software Foundation, version 3 of the License.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 */

#ifndef INC_FREERTOS_H
	#error "#include FreeRTOS.h" must appear in source files before "#include semphr.h"
#endif

#include "queue.h"

typedef xQueueHandle xEventHandle;

/**
 * <pre>xEventCreate( int uxInitialState, int uxAutoReseting )</pre>
 *
 * <i>Macro</i> that implements a waitable event using the existing queue
 * mechanism.
 *
 * Events are objects that can be set or reset, and can be
 * waited for. If a task calls wait for an unset event, it will be blocked
 * until another task or ISR sets it. They can also be auto-reseting, in
 * which case they are automatically reset after unblocking the waiting
 * tasks. Unlike a semaphore setting an event
 * unblocks <i>ALL</i> tasks that are waiting for it, thus it is ideal for
 * implementing randez-vous points, or notifying multiple tasks of an event.
 *
 *
 * @param uxInitialState Should be 1 if the event is set when created
 * @param uxAutoReseting Should be 1 if the event should autoreset.
 *
 * @returns Handle to the created event. Null if the event could not be created.
 *
 * Example usage:
 * <pre>
 *   xEventHandle xEvent = NULL;
 *
 *   struct xLedxParams
 * 	{
 *			int xLedNr;
 *			int xWaittime;
 *			xEventHandle xEvent;
 *	};
 *
 *	void vLedxTask(struct xLedxParams* p)
 *	{
 *		vTaskDelay(p->xWaittime / portMAX_DELAY);
 *		xEventWait(p->xEvent, portMAX_DELAY);
 *
 *		Led_on(1 << p->xLedNr);
 *
 *		vTaskDelete(NULL);
 *	}
 *
 *	void vStartLedx(int xLedNr, int xWait, xEventHandle xEvent)
 *	{
 *		struct xLedxParams* p = malloc(sizeof(struct xLedxParams));
 *		p->xLedNr = xLedNr;
 *		p->xWaittime = xWait;
 *		p->xEvent = xEvent;
 *
 *		xTaskCreate(vLedxTask, NULL, 256, p, 2, NULL);
 *	}
 *
 *	void eventTask(void* p)
 *	{
 *		int i;
 *
 *		Led_init();
 *		Led_off(0xFF);
 *
 *		xEvent = xEventCreate(0, 1);
 *
 *		for(i=0;i<4;i++)
 *			vStartLedx(i,(i+5) * 100, xEvent);
 *		for(;i<8;i++)
 *			vStartLedx(i,(i+35) * 100, xEvent);
 *
 *		vTaskDelay(3000 / portMAX_DELAY);
 *		xEventSet(xEvent);
 *		vTaskDelayMS(3000 / portMAX_DELAY);
 *		xEventSet(xEvent);
 *
 *		vTaskDelete(NULL);
 *
 *	}
 *
 * </pre>
 *
 */
#define xEventCreate(uxInitialState,uxAutoReseting) xQueueCreateEvent(uxInitialState, uxAutoReseting)

/**
 * <pre>xEventSet( xEventHandle pxEvent )</pre>
 *
 * <i>Macro</i> that sets an event.
 *
 * Tasks that are waiting will be unblocked, and depending on the type of the
 * evnet, it will automatically reset, or keep its set state. If it is not
 * an autoreseting event, all succeeding calls to xEventWait() will do nothing
 * until the event is manually reset with xEventReset()
 *
 * @param pxEvent Handle to the event to be set.
 *
 * @return pdTRUE if the event was set. pdFALSE if it failed.
 *
 * For example usage see xEventCreate()
 *
 */
#define xEventSet(pxEvent) xQueueGenericSend(pxEvent, NULL, 0, 0)


/**
 * <pre>xEventReset( xEventHandle pxEvent )</pre>
 *
 * <i>Macro</i> that reset a manually reseting event.
 *
 * Any future calls to xEventWait will block until a call to xEventSet is made.
 * Does not create an error, but it is ignored for autoreseting events.
 *
 * @param pxEvent Handle to the event to be reset
 *
 * For example usage see xEventCreate()
 */
#define xEventReset(pxEvent) xQueueGenericReceive(pxEvent, NULL, 0, pdFALSE)

/**
 * <pre>xEventSetFromISR(
                          xEventHandle pxEvent,
                          signed portBASE_TYPE *pxHigherPriorityTaskWoken
                      )</pre>
 *
 * <i>Macro</i> that sets an event from an ISR.
 * See xEventSet for further details.
 * @see xEventSet
 *
 * @param pxEvent Handle to the event to set
 *
 * @param pxHigherPriorityTaskWoken xEventSetFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if setting the event caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xEventSetFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE if the event was successfully set, otherwise errQUEUE_FULL.
 *
 * For example usage see xEventCreate()int
 */
#define xEventSetFromISR(pxEvent, pxHigherPriorityTaskWoken) xQueueSendFromISR(pxEvent, NULL, pxHigherPriorityTaskWoken)


/**
 * <pre>xEventResetFromISR(
                          xEventHandle pxEvent,
                          signed portBASE_TYPE *pxHigherPriorityTaskWoken
                      )</pre>
 *
 * <i>Macro</i> that resets an event from an ISR.
 * See xEventReset for further details.
 * @see xEventReset
 *
 * @param pxEvent Handle to the event to reset
 *
 * @param pxHigherPriorityTaskWoken xEventResetFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if resetting the event caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xEventResetFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE if the event was successfully set, otherwise errQUEUE_FULL.
 *
 * For example usage see xEventCreate()
 */
#define xEventResetFromISR(pxEvent, pxHigherPriorityTaskWoken) xQueueReceiveFromISR(pxEvent, NULL, pxHigherPriorityTaskWoken)



/**
 * <pre>xEventWait(xEventHandle* pxEvent, portTickType xTicksToWait)</pre>
 *
 * <i>Function</i> that waits for an event to fire.
 * If the event is manually resetting and set, it returns immediatly. Otherwise
 * it waits for a call to xEventSet or xEventSetForISR before returning.
 * A timeout can be specified for the waiting using xTicksToWait. If a value of
 * portMAX_DELAY is passed for xTicksToWait, xEventWait will not time out.
 *
 * @param pxEvent Handle to the event to wait for
 *
 * @param xBlockTime The time in ticks to wait for the event to become
 * set.  The macro portTICK_RATE_MS can be used to convert this to a
 * real time.  A block time of zero can be used to poll the event.
 *
 * For example usage see xEventCreate()
 */
portBASE_TYPE xEventWait(xEventHandle* pxEvent, portTickType xBlockTime);

xEventHandle* xQueueCreateEvent(int uxInitialState, int uxAutoReseting);
