#ifndef CLT_INTERNAL_H
#define CLT_INTERNAL_H

#if defined(__GNUC__)
#define UNUSED __attribute__((unused))
#define CLT_TEXT_SECTION __attribute__((section(".clt_text")))
#define CLT_MODULE_SECTION __attribute__((section(".clt_module_rodata")))
#define CLT_DATA_SECTION __attribute__((section(".clt_data")))

#elif defined(_MSC_VER) && (_MSC_VER > 1911)
#pragma section(".clt_text", read)
#pragma section(".clt_data", read)
#pragma section(".clt_module_rodata", read)

#define UNUSED [[maybe_unused]]
#define CLT_TEXT_SECTION __declspec((section(".clt_text")))
#define CLT_MODULE_SECTION __declspec((section(".clt_module_rodata")))
#define CLT_DATA_SECTION __declspec((section(".clt_data")))

#else
#pragma section(".clt_text", read)
#pragma section(".clt_data", read)
#pragma section(".clt_module_rodata", read)

#define UNUSED
#define CLT_TEXT_SECTION __declspec((section(".clt_text")))
#define CLT_MODULE_SECTION __declspec((section(".clt_module_rodata")))
#define CLT_DATA_SECTION __declspec((section(".clt_data")))
#endif

/*
 * ANSI Terminal Escape Sequences
 */
#define ANSI_CLEAR_COLOR "\033[0m"
#define ANSI_BOLD "\033[1m"
#define ANSI_ITALIC "\033[3m"
#define ANSI_UNDERLINE "\033[4m"
#define ANSI_RED_FG "\033[31m"
#define ANSI_GREEN_FG "\033[32m"
#define ANSI_YELLOW_FG "\033[33m"

#endif
