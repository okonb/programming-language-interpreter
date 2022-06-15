#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

OUTDIR="${DIR}/build/coverage"
INFO="${OUTDIR}/tests.info"

rm -rf "${OUTDIR}"
mkdir -p "${OUTDIR}"

(cd "${DIR}/build" ; ./tests)
lcov --capture --no-external --directory "${DIR}" --output-file "${INFO}" --rc lcov_branch_coverage=1
lcov --zerocounters --no-external --directory "${DIR}"
lcov -r "${INFO}" "${DIR}/build/*" -o "${INFO}" --rc lcov_branch_coverage=1
genhtml --output-directory "${OUTDIR}/html" "${INFO}" --rc genhtml_branch_coverage=1
