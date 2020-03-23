# Contributing code

If you are thinking about giving a hand with coding in Slicer, you've come to the right place, and thanks in advance :)

## Before coding

Make sure that your contribution is likely to be well received! If you aren't working on any of the already-posted [issues](https://github.com/junrrein/pdfslicer/issues), submit one so we can talk about what you wanna do beforehand. In this way, we can all be on the same page since day 1.

## Submitting

Just do a plain-old pull request through Github. I'll try to respond immediately, acknowledging your PR, but it may take me some days to review it.

## Requirements

* Your patch should compile without any warnings.

    When compiling with GCC, Slicer hard-codes the warning flags I care about as part of the CMake build process, so you won't miss any warnings by accident.

    Those flags are: `-Wall -Wextra -Wpedantic -Wshadow -Wduplicated-cond -Wduplicated-branches -Wlogical-op`.

## Things to keep in mind

* Slicer uses `clang-format` throughout all of its C++ source files, providing a `.clang-format` file.

    Try to use an IDE or extension that automatically applies `clang-format` on saving files so you don't have to think about it.

* Slicer provides a `.clang-tidy` file, that you can use for static analysis of your source and header files. To use it, run `clang-tidy -quiet yourfile.cpp`.

    If you think a specific instance of a warning should be ignored, add a `// NOLINT` comment at the end of the line that triggers it.

    If you think a class of warning should be disabled because it's not useful, file an issue and I'll take a look at it.

* If you introduce new strings or modify already existing ones, don't worry about updating translation templates (`.pot` / `.po` files). I'll do that for you after merging your PR.

## Got questions?

Don't hesitate to send me an email to junrrein at gmail dot com.
