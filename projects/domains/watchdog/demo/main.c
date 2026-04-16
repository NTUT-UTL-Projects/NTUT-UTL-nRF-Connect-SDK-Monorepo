#include "watchdog.h"

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define WDT_FEED_TRIES 5

#define WDT_MAX_WINDOW 3000U
#define WDT_MIN_WINDOW 0000U

#define WDG_FEED_INTERVAL (WDT_MIN_WINDOW + ((WDT_MAX_WINDOW - WDT_MIN_WINDOW) / 4))

int main(void)
{
	int err;

    /* Initialize watchdog. */
    err = watchdog0_init(WDT_MIN_WINDOW, WDT_MAX_WINDOW);
    if (err) {
        printk("Watchdog init failed\n");
        return err;
    }

	/* Feeding watchdog. */
	printk("Feeding watchdog %d times\n", WDT_FEED_TRIES);
	for (int i = 0; i < WDT_FEED_TRIES; ++i) {
		printk("Feeding watchdog...\n");
		watchdog0_feed();
		k_sleep(K_MSEC(WDG_FEED_INTERVAL));
	}

	/* Waiting for the SoC reset. */
	printk("Waiting for reset...\n");
	while (1) {
		k_yield();
	}

    return 0;
}