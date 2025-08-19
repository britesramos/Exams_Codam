#!/bin/zsh

# Number of tests to run
N=32767

# Store the first output as reference
echo 'dxss' | ./a.out 3 3 $((RANDOM * 2)) | cat -e > ref_output.txt

for i in {1..$N}
do
    echo 'dxss' | ./a.out 3 3 $((i * 2)) | cat -e > test_output.txt
    if ! diff -q ref_output.txt test_output.txt > /dev/null; then
        echo "Test $i: Output differs!"
        diff ref_output.txt test_output.txt
        exit 1
    fi
done

echo "All $N tests produced the same output."
rm ref_output.txt test_output.txt