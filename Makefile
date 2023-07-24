format_targets = include/args.h include/lambda.h

.PHONY: format format-check test

format-check:
	@clang-format -n $(format_targets)

format:
	@clang-format -i $(format_targets)
