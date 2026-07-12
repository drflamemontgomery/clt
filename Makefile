CC = gcc
CFLAGS = -g -Wall -Werror -Itest -Iclt -std=c11
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
example: $(EXAMPLE_OBJECTS) clt/clt.o
	$(CC) $(CFLAGS) $^ -o clt-example

.PHONY: example-test
example-test: CFLAGS += -DCLT_TEST_ENABLE=1
example-test: example
	./clt-example

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test_runner: $(TEST_OBJECTS) clt/clt.o
	$(CC) $(CFLAGS) $^ -o test_runner

.PHONY: test
test: CFLAGS += -DCLT_TEST_ENABLE=1
test: test_runner
	./test_runner

.PHONY: clean
clean:
	rm -f test_runner
	rm -f cev_matchlt/clt.o
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
