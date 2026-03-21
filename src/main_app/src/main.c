/* Main application file */


/***************************************************** Includes ******************************************************/

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/uart.h>

#ifdef CONFIG_GPIO_BUTTON
#include "gpio_button.h"
#endif /* CONFIG_GPIO_BUTTON */

/****************************************************** Config *******************************************************/

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define THREAD_0_STACK_SIZE 512
#define THREAD_1_STACK_SIZE 512
#define THREAD_0_PRIORITY 4
#define THREAD_1_PRIORITY 5
#define WORK_QUEUE_STACK_SIZE 1024
#define WORK_QUEUE_PRIORITY 6

/****************************************************** Defines ******************************************************/

#define SLEEP_TIME_MS 			(1000u)
#define UART_BUFFER_SIZE 		(64u)
#define UART_RECEIVE_TIMEOUT	(100)

/******************************************************* Types *******************************************************/

struct work_info
{
	struct k_work work;
	char name[25];
} my_work;

/***************************************************** Variables *****************************************************/

static K_THREAD_STACK_DEFINE(my_stack_area, WORK_QUEUE_STACK_SIZE);

const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart20));

static uint8_t rx_buffer[UART_BUFFER_SIZE] = {0};

static struct k_work_q offload_work_q = {0};

/******************************************** Local functions declarations *******************************************/

static inline void emulate_work()
{
	for(volatile int count_out = 0; count_out < 300000; count_out ++);
}

/******************************************** Local functions definitions ********************************************/

static void uart_callback(const struct device *dev, struct uart_event *evt, void *user_data)
{
	/* Handle UART events here if needed */

	switch (evt->type)
	{
		case UART_RX_RDY:
			LOG_INF("UART RX Ready: Received %d bytes", evt->data.rx.len);
			if (evt->data.rx.len > 0u)
			{
				if (evt->data.rx.buf[evt->data.rx.offset] == 't')
				{
					LOG_INF("Received 't', toggling LED");
					Gpio_SwitchLed();
				}
			}
			break;
		
		case UART_RX_DISABLED:
			LOG_INF("UART RX Disabled");
			uart_rx_enable(dev, rx_buffer, sizeof(rx_buffer), UART_RECEIVE_TIMEOUT);
			break;

		default:
			LOG_DBG("UART Event: %d", evt->type);
			break;
	}
}

/******************************************* Exported functions definitions ******************************************/

void offload_work_handler(struct k_work *work)
{
	emulate_work();
}

int main(void)
{
	LOG_INF("Main application started\n");

#ifdef CONFIG_GPIO_BUTTON
	LOG_DBG("GPIO Button Switching Enabled");
	Gpio_LedInit();
	Gpio_ButtonInit();
#else
	LOG_DBG("GPIO Button Switching Disabled");
#endif /* CONFIG_GPIO_BUTTON */

	if (!device_is_ready(uart_dev))
	{
		LOG_ERR("UART device %s is not ready", uart_dev->name);
		return -1;
	}

	if (uart_callback_set(uart_dev, uart_callback, NULL) != 0)
	{
		LOG_ERR("Failed to set UART callback");
		return -1;
	}

	if (uart_rx_enable(uart_dev, rx_buffer, sizeof(rx_buffer), UART_RECEIVE_TIMEOUT) != 0)
	{
		LOG_ERR("Failed to enable UART RX");
		return -1;
	}

	LOG_INF("Main application finished initialization");
	for (;;)
	{
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}

void thread0(void)
{
    uint64_t time_stamp;
    int64_t delta_time;

	k_work_queue_start(&offload_work_q, my_stack_area,
                   K_THREAD_STACK_SIZEOF(my_stack_area), WORK_QUEUE_PRIORITY,
                   NULL);

	strcpy(my_work.name, "Thread0 emulate_work()");
	k_work_init(&my_work.work, offload_work_handler);

    while (1) {
        time_stamp = k_uptime_get();
		k_work_submit_to_queue(&offload_work_q, &my_work.work);
        delta_time = k_uptime_delta(&time_stamp);

        printk("thread0 yielding this round in %lld ms\n", delta_time);
        k_msleep(20);
    }   
}

void thread1(void)
{
    uint64_t time_stamp;
    int64_t delta_time;

    while (1) {
        time_stamp = k_uptime_get();
        emulate_work();
        delta_time = k_uptime_delta(&time_stamp);

        printk("thread1 yielding this round in %lld ms\n", delta_time);
        k_msleep(20);
    }   
}

K_THREAD_DEFINE(thread0_id, THREAD_0_STACK_SIZE, thread0, NULL, NULL, NULL, THREAD_0_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, THREAD_1_STACK_SIZE, thread1, NULL, NULL, NULL, THREAD_1_PRIORITY, 0, 0);
