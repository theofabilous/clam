ansi_examples = $(wildcard examples/ansi/*.c)
gnu_examples = $(wildcard examples/gnu/*.c)
build_dir = build
ansi_basenames = $(basename $(notdir $(ansi_examples)))
gnu_basenames = $(basename $(notdir $(gnu_examples)))
ansi_examples_out = $(addprefix $(build_dir)/ansi/, $(ansi_basenames))
gnu_examples_out = $(addprefix $(build_dir)/gnu/, $(gnu_basenames))
format_targets = include/args.h include/lambda.h include/list.h include/common.h

.PHONY: format format-check clean

format-check:
	@clang-format -n $(format_targets)

format:
	@clang-format -i $(format_targets)

$(ansi_examples_out): $(build_dir)/ansi/% : examples/ansi/%.c
	@echo $@ $<

$(gnu_examples_out): $(build_dir)/gnu/% : examples/gnu/%.c
	@echo $@ $<

$(ansi_basenames): % : $(build_dir)/ansi/%

$(gnu_basenames): % : $(build_dir)/gnu/%

ansi: $(ansi_examples_out)

gnu: $(gnu_examples_out)

clean:
	@rm -rf $(build_dir) && mkdir $(build_dir)

