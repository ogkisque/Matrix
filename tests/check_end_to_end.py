from subprocess import run, Popen, PIPE
from sys import executable

num_test = 1
is_ok = True
for i in range(1, 9):
    str_data =  "tests/end_to_end/" + str(i) + ".dat"
    file_in = open(str_data, "r")
    str_ans = "tests/end_to_end/" + str(i) + ".dat.ans"

    ans = []
    for i in open(str_ans):
        ans.append(float(i.strip()))
	
    result = run(["build/src/main"], capture_output = True, encoding='cp866', stdin=file_in)
    print("Test: " + str(num_test).strip())

    res = list(map(float, result.stdout.split()))
    is_ok &= (abs(res[0] - ans[0]) <= 0.00001)
    if abs(res[0] - ans[0]) <= 0.00001:
        print("OK")
    else:
        print("ERROR\nExpect:", ans[0], "\nGive:  ", res[0])
    print("-------------------------------------------------")
    num_test += 1

if is_ok:
	print("TESTS PASSED")
else:
	print("TESTS FAILED")