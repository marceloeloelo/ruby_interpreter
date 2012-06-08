echo "Running tests/others/blank.in..."
./interpreter < tests/others/blank.in > tests/output/others/blank.res

echo "Running tests/others/end_of_line.in..."
./interpreter < tests/others/end_of_line.in > tests/output/others/end_of_line.res

echo "Running tests/unit/comments/comment_1.in..."
./interpreter < tests/unit/comments/comments_1.in > tests/output/unit/comments_1.res

echo "Running tests/unit/operations/addition_1.in..."
./interpreter < tests/unit/operations/addition_1.in > tests/output/unit/addition_1.res

echo "Running tests/unit/operations/substraction_1.in..."
./interpreter < tests/unit/operations/substraction_1.in > tests/output/unit/substraction_1.res

echo "Running tests/unit/operations/multiplication_1.in..."
./interpreter < tests/unit/operations/multiplication_1.in > tests/output/unit/multiplication_1.res

echo "Running tests/unit/operations/division_1.in..."
./interpreter < tests/unit/operations/division_1.in > tests/output/unit/division_1.res

echo "Running tests/unit/operations/modulo_1.in..."
./interpreter < tests/unit/operations/modulo_1.in > tests/output/unit/modulo_1.res

echo "Running tests/unit/operations/exponentiation_1.in..."
./interpreter < tests/unit/operations/exponentiation_1.in > tests/output/unit/exponentiation_1.res

echo "Running tests/unit/literals/strings_1.in..."
./interpreter < tests/unit/literals/strings_1.in > tests/output/unit/strings_1.res

echo "Running tests/unit/literals/strings_2.in..."
./interpreter < tests/unit/literals/strings_2.in > tests/output/unit/strings_2.res

echo "Running tests/unit/literals/symbols_1.in..."
./interpreter < tests/unit/literals/symbols_1.in > tests/output/unit/symbols_1.res

echo "Running tests/unit/variables/identifier_1.in..."
./interpreter < tests/unit/variables/identifier_1.in > tests/output/unit/identifier_1.res

echo "Running tests/unit/variables/nil_1.in..."
./interpreter < tests/unit/variables/nil_1.in > tests/output/unit/nil_1.res

echo "Running tests/unit/variables/self_1.in..."
./interpreter < tests/unit/variables/self_1.in > tests/output/unit/self_1.res

