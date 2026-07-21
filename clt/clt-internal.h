#ifndef CLT_INTERNAL_H
#define CLT_INTERNAL_H

#if defined(__GNUC__)
#define UNUSED __attribute__((unused))
#define CLT_TEXT_SECTION __attribute__((section (".clt_text")))
#define CLT_MODULE_SECTION __attribute__((section (".clt_module_rodata")))
#define CLT_DATA_SECTION __attribute__((section (".clt_data")))

#elif defined(_MSC_VER) && (_MSC_VER > 1911)
#pragma section(".clt_text", read)
#pragma section(".clt_data", read)
#pragma section(".clt_module_rodata", read)

#define UNUSED [[maybe_unused]]
#define CLT_TEXT_SECTION __declspec((section (".clt_text")))
#define CLT_MODULE_SECTION __declspec((section (".clt_module_rodata")))
#define CLT_DATA_SECTION __declspec((section (".clt_data")))

#else
#pragma section(".clt_text", read)
#pragma section(".clt_data", read)
#pragma section(".clt_module_rodata", read)

#define UNUSED
#define CLT_TEXT_SECTION __declspec((section (".clt_text")))
#define CLT_MODULE_SECTION __declspec((section (".clt_module_rodata")))
#define CLT_DATA_SECTION __declspec((section (".clt_data")))

#endif

#endif
