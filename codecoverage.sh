#!/bin/bash

lcov -v -t "tst_vilogger" --ignore mismatch --include "core*" --include "tests*" -o tst_vilogger.info -c -d . && \
genhtml -o report tst_vilogger.info
