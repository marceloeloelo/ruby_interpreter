echo "Running tests/others/blank.in..."
./interpreter < tests/others/blank.in > tests/output/blank.out

echo "Running tests/others/end_of_line.in..."
./interpreter < tests/others/end_of_line.in > tests/output/end_of_line.out

echo "Running tests/comments/comment_1.in..."
./interpreter < tests/comments/comments_1.in > tests/output/comments_1.out

echo "Running tests/operations/addition.in..."
./interpreter < tests/operations/addition.in > tests/output/addition.out

echo "Running tests/operations/substraction.in..."
./interpreter < tests/operations/substraction.in > tests/output/substraction.out

