print("=== File I/O Test ===")

ok1 = write_file("nearoh_file_test.txt", "Hello")
ok2 = append_file("nearoh_file_test.txt", " Nearoh")

print(ok1)
print(ok2)

text = read_file("nearoh_file_test.txt")
print(text)

number_text = str(123)
write_file("nearoh_number_test.txt", number_text)

read_number = read_file("nearoh_number_test.txt")
print(num(read_number) + 7)

print("=== File I/O Test Complete ===")