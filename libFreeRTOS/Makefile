PROJECT=FreeRTOS

TYPE=lib

#TYPE=app
#LINKER_FILES=link_a link_b link_c


SOURCE = 						\
	RTOS_hooks.c 				\
	croutine.c 					\
	heap.c 						\
	list.c 						\
	port.c 						\
	queue.c 					\
	tasks.c 					\

INCLUDE =						\
	FreeRTOS.h					\
	FreeRTOSConfig.h			\
	RTOS_hooks.h				\
	StackMacros.h				\
	croutine.h					\
	event.h						\
	list.h						\
	mpu_wrappers.h				\
	portable.h					\
	portmacro.h					\
	projdefs.h					\
	queue.h						\
	rtos_startup.inc			\
	semphr.h					\
	task.h						\
	xFreeRTOS.h					\

LIBS= 							\


INCLUDE_DIRS= 										\
	./include 				\
	../libakos/include							


LIB_DIRS=											\

OPTIMIZATION=`cat .optimization || echo "s"`
CPU=cortex-m3

include common.mk
