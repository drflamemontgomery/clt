CC = gcc
CFLAGS = -g -Wall -Werror -Itest -Iclit -DCLIT_TEST_ENABLE=1
OBJECTS = \
	test/main.o\
	clit/clit.o\
	$(END)

default: test_runner

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test_runner: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o test_runner

.PHONY: test
test: test_runner
	./test_runner

.PHONY: clean
clean:
	rm -f test_runner
	rm -f $(OBJECTS)

define compile_command_object
	'$(shell jq -n --arg directory "$(PWD)" \
		--arg command "$(CC) $(CFLAGS) -c $(1) -o $(2)" \
		--arg file "$(1)" \
		--arg output "$(2)" \
		'$$ARGS.named')'
endef

compile_commands.json: $(OBJECTS:.o=.c)
	@echo $(foreach filename,$(OBJECTS), $(call compile_command_object,$(filename:.o=.c), $(filename))) "" | jq -n '[inputs]' > compile_commands.json
