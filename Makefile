CC = gcc
CFLAGS = -O2 -g -Wall -Werror -Itest -Iclt -std=c11 -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections
TEST_OBJECTS = \
	test/external.o \
	test/runner.o \
	$(END)
EXAMPLE_OBJECTS = \
	example/main.o \
	example/algorithm.o \
	example/run.o\
	$(END)

ifneq ($(filter test,$(MAKECMDGOALS)),)
endif

default: test

.PHONY: example
example: clt-example

.PHONY: example-test
example-test: CFLAGS += -DCLT_TEST_ENABLE=1
example-test: clt-example
	./clt-example

clt-example: $(EXAMPLE_OBJECTS) clt/clt.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o clt-example

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test_runner: CFLAGS += -DCLT_TEST_ENABLE=1
test_runner: $(TEST_OBJECTS) clt/clt.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o test_runner

.PHONY: test
test: TEMPOBJFILE = $(shell mktemp).o
test: test_runner clt-example
	@./test_runner
	
	@readelf -S clt-example | grep clt_text && \
		{ echo "Error: .clt_text not stripped from clt-example"; exit 1; }; \
		echo "GC .clt_text: PASS"

	@readelf -S clt-example | grep clt_module_rodata && \
		{ echo "Error: .clt_module_rodata not stripped from clt-example"; exit 1; };\
		echo "GC .clt_module_rodata: PASS"

	@readelf -S clt-example | grep clt_data && \
		{ echo "Error: .clt_data not stripped from clt-example"; exit 1; };\
		echo "GC .clt_data: PASS"

.PHONY: clean
clean:
	rm -f clt-example
	rm -f test_runner
	rm -f clt/clt.o
	rm -f $(TEST_OBJECTS)
	rm -f $(EXAMPLE_OBJECTS)

define compile_command_object
	'$(shell jq -n --arg directory "$(PWD)" \
		--arg command "$(CC) $(CFLAGS) -c $(1) -o $(2)" \
		--arg file "$(1)" \
		--arg output "$(2)" \
		'$$ARGS.named')'
endef

compile_commands.json: $(TEST_OBJECTS:.o=.c) $(EXAMPLE_OBJECTS:.o=.c)
	@echo $(foreach filename,$^, $(call compile_command_object,$(filename:.o=.c), $(filename))) "" | jq -n '[inputs]' > compile_commands.json
