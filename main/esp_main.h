#include "sdkconfig.h"

// Enable this to get cpu stats
#define COLLECT_CPU_STATS 1

// Enable display support if BSP is enabled in menuconfig
#if (CONFIG_TFLITE_USE_BSP)
#define DISPLAY_SUPPORT 1
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void run_inference(void *ptr);
#ifdef __cplusplus
}
#endif
