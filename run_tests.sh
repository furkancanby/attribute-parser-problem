for i in {0..5}; do
    echo -n "Test Case ${i}: "

    input_file="tests/${i}.input"
    expected_output_file="tests/${i}.expected"

    # Run the program with input files and capture the output
    ./build/attribute_parser < $input_file > tests/output

    # Compare the actual output with the expected output
    if diff -q tests/output $expected_output_file >/dev/null; then
        echo "PASSED"
        rm tests/output
    else
        echo "FAILED"
        rm tests/output
    fi

done