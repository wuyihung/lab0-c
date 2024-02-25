import subprocess
import tempfile


def extract_and_concatenate_strings(input_text):
    lines = input_text.strip().split('\n')
    concatenated_string = ''

    for line in lines[1:-2]:
        # Remove the leading 'l = [' and the trailing ']' from each line
        clean_line = line[5:-1]
        characters_to_remove = [' ', '.']
        for char in characters_to_remove:
            clean_line = clean_line.replace(char, '')
        concatenated_string += clean_line

    return concatenated_string


def test_string_randomness(input_string):
    # Write the input string to a temporary file
    with tempfile.NamedTemporaryFile(mode='w+') as temp_file:
        temp_file_name = temp_file.name
        temp_file.write(input_string)
        temp_file.flush()

        # Run the 'ent' command on the temporary file
        result = subprocess.run(['ent', temp_file_name],
                                capture_output=True, text=True)

        # Output the results
        if result.stderr:
            print("Error:", result.stderr)
        else:
            print(result.stdout)


BIG_LIST_SIZE = 30

# 測試 RAND 次數
test_count = 150000
input = "new\n"
for i in range(int(test_count / BIG_LIST_SIZE)):
    input += "ih RAND " + str(BIG_LIST_SIZE) + "\n"
modulo = test_count % BIG_LIST_SIZE
input += "ih RAND " + str(modulo) + "\n"
input += "free\nquit\n"

# 取得 stdout 的 RAND 結果
command = './qtest -v 3'
clist = command.split()
completedProcess = subprocess.run(
    clist, capture_output=True, text=True, input=input)
s = completedProcess.stdout

s_extracted = extract_and_concatenate_strings(s)
test_string_randomness(s_extracted)
