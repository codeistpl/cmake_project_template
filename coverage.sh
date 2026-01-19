#!/bin/bash

# Generate test coverage report using gcovr
# Requires: gcovr, cmake with USE_GCOV enabled

if [ ! -d ".build" ]; then
    mkdir -p .build
fi

cd .build

# Configure with coverage flags
cmake .. -DCMAKE_BUILD_TYPE=Debug -DUSE_GCOV=ON

# Build with coverage instrumentation
cmake --build .

# Run tests to generate coverage data
make test

# Generate coverage report
echo "Generating coverage report..."
gcovr -r .. --object-dir=. --print-summary --html-details coverage.html
echo ""
echo "=== Coverage Summary ==="
gcovr -r .. --object-dir=.
echo ""
echo "Coverage report generated: .build/coverage.html"

