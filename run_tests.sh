#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Get project directory and name
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_NAME="$(basename "$PROJECT_DIR")"
BUILD_DIR="${PROJECT_DIR}/cmake-build-debug"

# Default options
VERBOSE=0
REBUILD=0
FILTER=""
LIST_ONLY=0

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--verbose)
            VERBOSE=1
            shift
            ;;
        -r|--rebuild)
            REBUILD=1
            shift
            ;;
        -f|--filter)
            FILTER="$2"
            shift 2
            ;;
        -l|--list)
            LIST_ONLY=1
            shift
            ;;
        -h|--help)
            echo -e "${BLUE}Usage:${NC} $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  -v, --verbose     Show verbose test output"
            echo "  -r, --rebuild     Clean and rebuild before testing"
            echo "  -f, --filter NAME Run only tests matching NAME (regex)"
            echo "  -l, --list        List available tests without running"
            echo "  -h, --help        Show this help message"
            echo ""
            echo "Examples:"
            echo "  $0                    # Run all tests"
            echo "  $0 -v                 # Run with verbose output"
            echo "  $0 -f lambda          # Run only lambda tests"
            echo "  $0 -f church -v       # Run church tests verbosely"
            echo "  $0 -r                 # Rebuild and test"
            echo "  $0 -l                 # List all available tests"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Build directory not found. Running initial build...${NC}"
    REBUILD=1
fi

# Rebuild if requested or necessary
if [ $REBUILD -eq 1 ]; then
    echo -e "${BLUE}==================================================${NC}"
    echo -e "${BLUE}Cleaning and rebuilding ${PROJECT_NAME}${NC}"
    echo -e "${BLUE}==================================================${NC}"

    if [ -d "$BUILD_DIR" ]; then
        echo -e "${YELLOW}Removing old build directory...${NC}"
        rm -rf "$BUILD_DIR"
    fi

    bash "${PROJECT_DIR}/debug.sh"

    if [ $? -ne 0 ]; then
        echo -e "${RED}✗ Build failed${NC}"
        exit 1
    fi

    # debug.sh already runs tests, so we're done
    exit 0
fi

# Check if tests are built
if [ ! -f "${BUILD_DIR}/CTestTestfile.cmake" ]; then
    echo -e "${YELLOW}Tests not built. Building now...${NC}"
    bash "${PROJECT_DIR}/debug.sh"
    if [ $? -ne 0 ]; then
        exit 1
    fi
    exit 0
fi

# Change to build directory
cd "$BUILD_DIR" || exit 1

# List tests if requested
if [ $LIST_ONLY -eq 1 ]; then
    echo -e "${BLUE}==================================================${NC}"
    echo -e "${BLUE}Available Tests${NC}"
    echo -e "${BLUE}==================================================${NC}"
    ctest -N
    exit 0
fi

# Run tests
echo -e "${BLUE}==================================================${NC}"
echo -e "${BLUE}Running Tests for ${PROJECT_NAME}${NC}"
if [ -n "$FILTER" ]; then
    echo -e "${CYAN}Filter: ${FILTER}${NC}"
fi
echo -e "${BLUE}==================================================${NC}"

# Build ctest command
CTEST_CMD="ctest --output-on-failure"

if [ $VERBOSE -eq 1 ]; then
    CTEST_CMD="$CTEST_CMD --verbose"
fi

if [ -n "$FILTER" ]; then
    CTEST_CMD="$CTEST_CMD -R $FILTER"
fi

# Execute tests
eval $CTEST_CMD
TEST_RESULT=$?

# Print summary
echo ""
echo -e "${BLUE}==================================================${NC}"

if [ $TEST_RESULT -eq 0 ]; then
    echo -e "${GREEN}✓ All tests passed!${NC}"

    # Show test summary
    TEST_COUNT=$(ctest -N ${FILTER:+-R $FILTER} 2>/dev/null | grep "Total Tests:" | awk '{print $3}')
    if [ -n "$TEST_COUNT" ]; then
        echo -e "${GREEN}  Tests run: ${TEST_COUNT}${NC}"
    fi

    # Show execution time
    echo -e "${GREEN}  Build directory: ${BUILD_DIR}${NC}"
else
    echo -e "${RED}✗ Some tests failed${NC}"
    echo -e "${YELLOW}  Tip: Use -v for verbose output${NC}"
    echo -e "${YELLOW}  Tip: Use -r to rebuild and test${NC}"
fi

echo -e "${BLUE}==================================================${NC}"

exit $TEST_RESULT
