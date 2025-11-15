# Build and Test Scripts

This document describes the build and test scripts available for the `typical` project.

## Overview

The project provides two main scripts for building and testing:

1. **`debug.sh`** - Complete build and test workflow
2. **`run_tests.sh`** - Flexible test runner with filtering options

## debug.sh

### Purpose

Performs a complete build and test cycle with debug configuration. This is the main script for development.

### Usage

```bash
./debug.sh
```

### What It Does

1. **Configures CMake** with debug settings
   - Uses Homebrew LLVM toolchain
   - Enables address sanitizer (`-fsanitize=address`)
   - Sets comprehensive warning flags
   - Enables compile commands export
   - Uses Ninja generator (required for C++ modules)

2. **Builds the project** using Ninja

3. **Runs all tests** with CTest

4. **Creates symlink** for `compile_commands.json` (for IDE/LSP support)

### Output

The script provides colored output:
- 🔵 Blue: Section headers and info
- 🟢 Green: Success messages
- 🔴 Red: Error messages

### Exit Codes

- `0` - Build and all tests successful
- `1` - Build failed or tests failed

### Example Output

```
==================================================
Building typical (Debug)
==================================================

[CMake configuration...]
✓ Build succeeded

==================================================
Running Tests
==================================================
Test project /path/to/typical/cmake-build-debug
    Start 1: lambda_tests
1/3 Test #1: lambda_tests .....................   Passed    0.03 sec
    Start 2: lambda_advanced_tests
2/3 Test #2: lambda_advanced_tests ............   Passed    0.03 sec
    Start 3: church_tests
3/3 Test #3: church_tests .....................   Passed    0.03 sec

100% tests passed, 0 tests failed out of 3

==================================================
✓ All tests passed!
==================================================
```

## run_tests.sh

### Purpose

Flexible test runner with filtering, verbose output, and rebuild options. Use this when you want to run tests without a full rebuild.

### Usage

```bash
./run_tests.sh [OPTIONS]
```

### Options

| Option | Description |
|--------|-------------|
| `-v`, `--verbose` | Show verbose test output including all assertions |
| `-r`, `--rebuild` | Clean and rebuild before testing |
| `-f`, `--filter NAME` | Run only tests matching NAME (regex pattern) |
| `-l`, `--list` | List available tests without running them |
| `-h`, `--help` | Show help message |

### Examples

#### Run all tests (quick)
```bash
./run_tests.sh
```

#### Run with verbose output
```bash
./run_tests.sh -v
# or
./run_tests.sh --verbose
```

#### Run only lambda tests
```bash
./run_tests.sh -f lambda
# Runs: lambda_tests and lambda_advanced_tests
```

#### Run only church tests
```bash
./run_tests.sh -f church
# Runs: church_tests
```

#### Run specific test with verbose output
```bash
./run_tests.sh -f lambda_advanced -v
```

#### List available tests
```bash
./run_tests.sh -l
```

#### Clean rebuild and test
```bash
./run_tests.sh -r
```

### Output

Colored output similar to `debug.sh`:
- 🔵 Blue: Headers and section info
- 🟢 Green: Success messages
- 🔴 Red: Failure messages
- 🟡 Yellow: Warnings and tips
- 🔷 Cyan: Filter information

### Exit Codes

- `0` - All tests passed
- `1` - Some tests failed or build error

### Example Outputs

#### Successful test run
```
==================================================
Running Tests for typical
==================================================
Test project /path/to/typical/cmake-build-debug
    Start 1: lambda_tests
1/3 Test #1: lambda_tests .....................   Passed    0.03 sec
    Start 2: lambda_advanced_tests
2/3 Test #2: lambda_advanced_tests ............   Passed    0.03 sec
    Start 3: church_tests
3/3 Test #3: church_tests .....................   Passed    0.03 sec

100% tests passed, 0 tests failed out of 3

==================================================
✓ All tests passed!
  Tests run: 3
  Build directory: /path/to/typical/cmake-build-debug
==================================================
```

#### Filtered test run
```
==================================================
Running Tests for typical
Filter: lambda
==================================================
Test project /path/to/typical/cmake-build-debug
    Start 1: lambda_tests
1/2 Test #1: lambda_tests .....................   Passed    0.03 sec
    Start 2: lambda_advanced_tests
2/2 Test #2: lambda_advanced_tests ............   Passed    0.03 sec

100% tests passed, 0 tests failed out of 2

==================================================
✓ All tests passed!
  Tests run: 2
  Build directory: /path/to/typical/cmake-build-debug
==================================================
```

#### Listing tests
```
==================================================
Available Tests
==================================================
Test project /path/to/typical/cmake-build-debug
  Test #1: lambda_tests
  Test #2: lambda_advanced_tests
  Test #3: church_tests

Total Tests: 3
```

## When to Use Which Script

### Use `debug.sh` when:
- Starting fresh development session
- After pulling changes from repository
- After modifying CMakeLists.txt
- Need to ensure everything rebuilds correctly
- Want complete build + test cycle

### Use `run_tests.sh` when:
- Code already built, just want to run tests
- Testing specific module (use `-f` filter)
- Need verbose test output (use `-v`)
- Want to quickly verify tests pass
- Debugging specific test failures

## Workflow Examples

### Daily Development
```bash
# Morning: Start with full rebuild
./debug.sh

# During development: Quick test runs
./run_tests.sh

# Testing specific changes to lambda module
./run_tests.sh -f lambda -v

# Before committing: Full test suite
./run_tests.sh
```

### Debugging Test Failures
```bash
# First, identify which test fails
./run_tests.sh

# Run failing test with verbose output
./run_tests.sh -f <test_name> -v

# If needed, clean rebuild
./run_tests.sh -r
```

### Continuous Integration
```bash
# In CI pipeline, always use full rebuild
./debug.sh
```

## Requirements

Both scripts require:
- **macOS** (uses Homebrew LLVM and xcrun)
- **Homebrew LLVM** installed at `/opt/homebrew/opt/llvm/`
- **Ninja** build system
- **CMake** 3.28 or later
- **Bash** shell

### Installing Requirements

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install llvm cmake ninja
```

## Troubleshooting

### "Build directory not found"
Run `./debug.sh` first to create initial build.

### "ninja: command not found"
```bash
brew install ninja
```

### "CMake 3.28 or higher required"
```bash
brew upgrade cmake
```

### Module compilation errors
Ensure using LLVM from Homebrew:
```bash
which clang++
# Should output: /opt/homebrew/opt/llvm/bin/clang++
```

### Tests fail after code changes
Try clean rebuild:
```bash
./run_tests.sh -r
```

## Advanced Usage

### Custom Build Directory
Both scripts use `cmake-build-debug` by default. To use a different directory, modify the scripts or use CMake directly.

### Different Compiler
Edit `debug.sh` to change compiler paths:
```bash
-DCMAKE_C_COMPILER=/path/to/clang \
-DCMAKE_CXX_COMPILER=/path/to/clang++ \
```

### Additional Flags
Edit `debug.sh` to add custom flags:
```bash
-DCMAKE_CXX_FLAGS="... -Dyour_flag_here" \
```

## Integration with IDEs

### Visual Studio Code
The scripts generate `compile_commands.json` which is automatically used by:
- clangd LSP
- C/C++ IntelliSense
- Various linting tools

### CLion
CLion can use the Ninja build system directly. Point it to the `cmake-build-debug` directory.

## Performance Notes

- **`debug.sh`**: ~2-3 seconds (full rebuild)
- **`run_tests.sh`**: <0.1 seconds (tests only)
- All tests are compile-time, so execution is nearly instant

## See Also

- `tests/README.md` - Comprehensive test documentation
- `tests/QUICK_REFERENCE.md` - Quick test reference
- `tests/EXAMPLES.md` - Test writing examples
- `CMakeLists.txt` - Build configuration