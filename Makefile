build_dir = build
examples = $(wildcard examples/*.c)
examples_basenames = $(basename $(notdir $(examples)))
examples_out = $(addprefix $(build_dir)/, $(examples_basenames))
format_targets = include/args.h include/lambda.h include/list.h include/common.h

.PHONY: format format-check clean examples

format-check:
	@clang-format -n $(format_targets)

format:
	@clang-format -i $(format_targets)

$(examples_out): $(build_dir)/% : examples/%.c
	$(CC) -o $@ $<

$(examples_basenames): % : $(build_dir)/%

examples: $(examples_out)

clean:
	@rm -rf $(build_dir) && mkdir $(build_dir)

