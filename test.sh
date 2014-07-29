#!/bin/bash 

(./manage/fast_compile_and_test_all.sh || echo ERROR) 2>&1 | tee log | manage/check.sh -efc

echo "===== HIBAK: =====" ; cat log | manage/check.sh -c

