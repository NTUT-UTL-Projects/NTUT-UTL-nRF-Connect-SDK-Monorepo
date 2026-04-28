/*
 * @note
 * Refers to
 * -
 * https://github.com/nrfconnect/sdk-zephyr/blob/ncs-v3.2.4/samples/drivers/watchdog/src/main.c
 * -
 * https://github.com/nrfconnect/sdk-zephyr/blob/ncs-v3.2.4/samples/subsys/task_wdt/src/main.c
 */
#include "watchdog.h"

#include <stdbool.h>
#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/*
 * To use this sample the devicetree's /aliases must have a 'watchdog0'
 * property.
 */
#if DT_HAS_COMPAT_STATUS_OKAY(st_stm32_window_watchdog)
#elif DT_HAS_COMPAT_STATUS_OKAY(nordic_nrf_wdt)
    /* Nordic supports a callback, but it has 61.2 us to complete before
     * the reset occurs, which is too short for this sample to do anything
     * useful.  Explicitly disallow use of the callback.
     */
    #define WDT_ALLOW_CALLBACK 0
#elif DT_HAS_COMPAT_STATUS_OKAY(raspberrypi_pico_watchdog)
    #define WDT_ALLOW_CALLBACK 0
#elif DT_HAS_COMPAT_STATUS_OKAY(gd_gd32_wwdgt)
#elif DT_HAS_COMPAT_STATUS_OKAY(intel_tco_wdt)
    #define WDT_ALLOW_CALLBACK 0
#elif DT_HAS_COMPAT_STATUS_OKAY(nxp_fs26_wdog)
    #define WDT_OPT 0
#elif DT_HAS_COMPAT_STATUS_OKAY(renesas_ra_wdt)
    #define WDT_ALLOW_CALLBACK 0
#elif DT_HAS_COMPAT_STATUS_OKAY(wch_iwdg)
    #define WDT_ALLOW_CALLBACK 0
    #define WDT_OPT            0
#endif

#ifndef WDT_ALLOW_CALLBACK
    #define WDT_ALLOW_CALLBACK 1
#endif

#ifndef WDT_OPT
    #define WDT_OPT WDT_OPT_PAUSE_HALTED_BY_DBG
#endif

#if WDT_ALLOW_CALLBACK
static void wdt_callback(const struct device *wdt_dev, int channel_id)
{
    static bool handled_event;

    if (handled_event)
    {
        return;
    }

    wdt_feed(wdt_dev, channel_id);

    printk("Handled things..ready to reset\n");
    handled_event = true;
}
#endif /* WDT_ALLOW_CALLBACK */

static int                        wdt_channel_id;
static const struct device *const wdt = DEVICE_DT_GET(DT_ALIAS(watchdog0));

int watchdog0_init(uint32_t window_min, uint32_t window_max)
{
    int err;

    printk("Watchdog sample application\n");

    if (!device_is_ready(wdt))
    {
        printk("%s: device not ready.\n", wdt->name);
        return 0;
    }

    struct wdt_timeout_cfg wdt_config = {
        /* Reset SoC when watchdog timer expires. */
        .flags = WDT_FLAG_RESET_SOC,

        /* Expire watchdog after max window */
        .window.min = window_min,
        .window.max = window_max,
    };

#if WDT_ALLOW_CALLBACK
    /* Set up watchdog callback. */
    wdt_config.callback = wdt_callback;

    printk("Attempting to test pre-reset callback\n");
#else  /* WDT_ALLOW_CALLBACK */
    printk("Callback in RESET_SOC disabled for this platform\n");
#endif /* WDT_ALLOW_CALLBACK */

    wdt_channel_id = wdt_install_timeout(wdt, &wdt_config);
    if (wdt_channel_id == -ENOTSUP)
    {
        /* IWDG driver for STM32 doesn't support callback */
        printk("Callback support rejected, continuing anyway\n");
        wdt_config.callback = NULL;
        wdt_channel_id      = wdt_install_timeout(wdt, &wdt_config);
    }
    if (wdt_channel_id < 0)
    {
        printk("Watchdog install error\n");
        return 0;
    }

    err = wdt_setup(wdt, WDT_OPT);
    if (err < 0)
    {
        printk("Watchdog setup error\n");
        return 0;
    }

    return 0;
}

int watchdog0_feed(void) { return wdt_feed(wdt, wdt_channel_id); }
