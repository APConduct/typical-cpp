#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Get project directory and name dynamically
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_NAME="$(basename "$PROJECT_DIR")"

echo -e "${BLUE}==================================================${NC}"
echo -e "${BLUE}Building ${PROJECT_NAME} (Debug)${NC}"
echo -e "${BLUE}==================================================${NC}"

# Use Homebrew LLVM with specific include and lib paths
cmake \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_MAKE_PROGRAM=ninja \
-DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang \
-DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DCMAKE_CXX_FLAGS="-Wall -Wextra -Wshadow -Wpedantic -Wno-variadic-macros -I/opt/homebrew/opt/llvm/include -fsanitize=address" \
-DCMAKE_EXE_LINKER_FLAGS="-L/opt/homebrew/opt/llvm/lib/c++" \
-DCMAKE_OSX_SYSROOT="$(xcrun --show-sdk-path)" \
-G Ninja \
-S . \
-B ./cmake-build-debug

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ CMake configuration failed${NC}"
    exit 1
fi

echo -e "\n${BLUE}Building project...${NC}"
ninja -C ./cmake-build-debug

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Build failed${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Build succeeded${NC}"

# Create symlink for compile_commands.json
ln -sf ./cmake-build-debug/compile_commands.json .

# Run tests
echo -e "\n${BLUE}==================================================${NC}"
echo -e "${BLUE}Running Tests${NC}"
echo -e "${BLUE}==================================================${NC}"

cd ./cmake-build-debug
ctest --output-on-failure

if [ $? -eq 0 ]; then
    echo -e "\n${GREEN}==================================================${NC}"
    echo -e "${GREEN}✓ All tests passed!${NC}"
    echo -e "${GREEN}==================================================${NC}"
else
    echo -e "\n${RED}==================================================${NC}"
    echo -e "${RED}✗ Some tests failed${NC}"
    echo -e "${RED}==================================================${NC}"
    exit 1
fi
