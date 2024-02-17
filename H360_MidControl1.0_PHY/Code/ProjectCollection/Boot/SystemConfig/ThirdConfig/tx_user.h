#ifndef TX_USER_H
#define TX_USER_H

//最快速的系统配置
/**
** TX_MAX_PRIORITIES                       32
** TX_DISABLE_PREEMPTION_THRESHOLD
** TX_DISABLE_REDUNDANT_CLEARING
** TX_DISABLE_NOTIFY_CALLBACKS
** TX_NOT_INTERRUPTABLE
** TX_TIMER_PROCESS_IN_ISR
** TX_REACTIVATE_INLINE
** TX_DISABLE_STACK_FILLING
** TX_INLINE_THREAD_RESUME_SUSPEND
**/

//生成代码空间占用最小的系统配置
/**
** TX_MAX_PRIORITIES                       32
** TX_DISABLE_PREEMPTION_THRESHOLD
** TX_DISABLE_REDUNDANT_CLEARING
** TX_DISABLE_NOTIFY_CALLBACKS
** TX_NO_FILEX_POINTER
** TX_NOT_INTERRUPTABLE
** TX_TIMER_PROCESS_IN_ISR
**/

//系统支持的最大任务数量
#define TX_MAX_PRIORITIES                       32
//系统任务堆栈最小尺寸
#define TX_MINIMUM_STACK                        512
//任务堆栈中存放的用于扩展TCB标识符
//#define TX_THREAD_USER_EXTENSION                ????
//系统定时任务的堆栈尺寸
#define TX_TIMER_THREAD_STACK_SIZE              1024
//系统定时任务的优先级
#define TX_TIMER_THREAD_PRIORITY                0

//系统定时任务的执行周期
#define TX_TIMER_TICKS_PER_SECOND               100UL

//任务TCB中是否包含指向FileX的指针
//#define TX_NO_FILEX_POINTER

//是否使用中断掩码来执行中断,定义了这个的情况下需要定义 
//操作系统内核中断屏蔽级别,在中断处理的时候,大于等于该值的中断将会被屏蔽,操作系统会知道该中断的发生
//低于该值的中断将不会被屏蔽,操作系统不会认识到该中断的发生.也就是说,低于该值的中断是快速中断,他不会因
//为系统调度而被屏蔽,依然可以发生,这样可以防止丢中断,但是,不要在这些中断中调用系统相关的函数大于等于该
//值的中断是系统中断,系统会感知到这些中断的发生,所以也就可以调用系统API函数如果这个值设置的不对的话,中
//断时间次数多了,系统会出错
//因为系统的中断四位,所以,慎重起见,将中断感知级别设置为3 ,也就是抢占优先级0,子优先级3以及高于这个的任务
//将会被操作系统感知,其余不会,例如抢占优先级0,子优先级2就不会
#define TX_PORT_USE_BASEPRI
//CM7只用到高四位,低四位不用
#define TX_PORT_BASEPRI                        0X30

//使能调试插件的性能分析
#define TX_EXECUTION_PROFILE_ENABLE
//当使能了调试插件的性能分析而且使用的MCU是CORTEXM系列的时候,需要定义该宏
#define TX_CORTEX_M_EPK


//确定定时器是否到期的处理，比如应用定时器，溢出时间和函数tx_thread_sleep调用等，是在系统定时器任务里面还是在定时器中断里面调用。
//默认是在定时任务里面，当定义了下面函数后，将直接在定时器中断里面处理，可以去掉定时器任务所消耗资源
//#define TX_TIMER_PROCESS_IN_ISR

//用于设置定时器激活是否采用内联方式，默认此功能是关闭的。如果使能后，内联方式的执行速度快，但增加代码量
#define TX_REACTIVATE_INLINE

//用于设置是否关闭栈填充，默认情况下是使能的，所有任务的栈空间全部填充为0xEF，
//带有ThreadX调试组件或者运行时栈检测会用到。
//#define TX_DISABLE_STACK_FILLING

//用于使能栈检测，默认是关闭的。此选项使能后，而TX_DISABLE_STACK_FILLING没使能时，栈填充将开启，方便栈检测
#define TX_ENABLE_STACK_CHECKING

//用于设置是否关闭抢占阀值，默认是开启的。如果应用程序不需要此功能，关闭后可以降低代码需求，提升性能
#define TX_DISABLE_PREEMPTION_THRESHOLD

//用于设置是否清零ThreadX全局变量，如果编译器启动代码在ThreadX运行前清除了.bss段，那么可以关闭不必要的清零
//#define TX_DISABLE_REDUNDANT_CLEARING

//确定是否不需要定时器组，禁止后需要用户注释掉tx_initialize_low_level文件里面tx_timer_interrupt的调用。
//另外，禁止后，必须使能TX_TIMER_PROCESS_IN_ISR
//#define TX_NO_TIMER
//#ifndef TX_TIMER_PROCESS_IN_ISR
//#define TX_TIMER_PROCESS_IN_ISR
//#endif

//用于设置是否关闭通知回调，默认是使能的。如果应用程序没有用到消息回调，关闭掉后可以减小代码，并且可以提升性能
//#define TX_DISABLE_NOTIFY_CALLBACKS

//使能tx_thread_resume和tx_thread_suspend使用内联代码，优势是提升这两个函数的执行性能，劣势是增加代码量
//#define TX_INLINE_THREAD_RESUME_SUSPEND

//设置TreadX内核不可中断，好处是降低处理负担，并且产生的代码小。但增加锁时间
//#define TX_NOT_INTERRUPTABLE

//使能事件Trace，会稍微增加点代码 
#define TX_ENABLE_EVENT_TRACE

//使能BLOCK_POOL信息获取
#define TX_BLOCK_POOL_ENABLE_PERFORMANCE_INFO

//使能BYTE_POOL信息获取
#define TX_BYTE_POOL_ENABLE_PERFORMANCE_INFO

//使能事件标志信息获取
#define TX_EVENT_FLAGS_ENABLE_PERFORMANCE_INFO

//使能互斥信号量信息获取
#define TX_MUTEX_ENABLE_PERFORMANCE_INFO

//使能消息对象信息获取
#define TX_QUEUE_ENABLE_PERFORMANCE_INFO

//使能信号量信息获取
#define TX_SEMAPHORE_ENABLE_PERFORMANCE_INFO

//使能任务信息获取
#define TX_THREAD_ENABLE_PERFORMANCE_INFO

//使能定时器信息获取
#define TX_TIMER_ENABLE_PERFORMANCE_INFO

//多次字节池查找阈值
//#define TX_BYTE_POOL_MULTIPLE_BLOCK_SEARCH    20

//定义字节池清空的时候,延时查找的时间
//#define TX_BYTE_POOL_DELAY_VALUE              3

#endif

