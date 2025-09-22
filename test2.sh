#!/bin/bash

# ========= CONFIG =========
MINISHELL="$(cd "$(dirname "$0")" && pwd)/minishell"
BASH=/bin/bash

# ========= COLORS =========
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
RESET="\033[0m"

# ========= TEST FUNCTION =========
run_test() {
	cmd="$1"
	expected="$2"

	echo -e "${YELLOW}Test: $cmd${RESET}"

	# Pipe the command in minishell
	minishell_output=$(echo "$cmd" | $MINISHELL 2>&1 | sed '/^debugshell/d')
	bash_output=$(echo "$cmd" | $BASH 2>&1)

	if [ "$expected" = "same" ]; then
		if [ "$minishell_output" = "$bash_output" ]; then
			echo -e "${GREEN}✅ OK${RESET}"
		else
			echo -e "${RED}❌ FAIL${RESET}"
			echo -e "📝 Attendu (bash) :\n$bash_output"
			echo -e "🔴 Reçu (minishell) :\n$minishell_output"
			echo -e "# Ce test compare la sortie à celle de bash."
		fi
	elif [[ "$minishell_output" == *"$expected"* ]]; then
		echo -e "${GREEN}✅ OK (contient: '$expected')${RESET}"
	else
		echo -e "${RED}❌ FAIL${RESET}"
		echo -e "🔴 Reçu :\n$minishell_output"
		echo -e "# Ce test vérifie si la sortie contient : '$expected'"
	fi
	echo ""
}

# ========= CLEANUP =========
rm -f testfile

# ========= TESTS =========
run_test "echo hello" same
run_test "ls" same
run_test "pwd" same
run_test "true" ""
run_test "false" ""
run_test "echo line > testfile && cat testfile" "line"
run_test "echo add >> testfile && cat testfile" "line
add"
run_test "cat < testfile" "line"
run_test "cat < notfoundfile" "No such file or directory"
run_test "echo hello | cat" "hello"
run_test "ls | cat" same
run_test "echo pipe | cat | cat | cat" "pipe"
run_test "echo test1 > testfile && echo test2 >> testfile && cat testfile" "test1
test2"
run_test "cat testfile | grep test2" "test2"
run_test "echo abc | tr a-z A-Z" "ABC"
run_test "echo abc | rev" "cba"
run_test "echo line1 > testfile && cat < testfile | cat | cat" "line1"

###############################
#         TEST EXPORT         #
###############################

# Simple export (new variable)
run_test "export VAR1=hello && echo \$VAR1" "hello"

# Export of an empty variable
run_test "export VAR2= && echo \$VAR2" ""

# Export of a variable without value (only the name)
run_test "export VAR3 && env | grep VAR3" "VAR3"

# Update an existing variable
run_test "export VAR1=world && echo \$VAR1" "world"

# Export with += (concatenation) if implemented
run_test "export VAR1+=_42 && echo \$VAR1" "world_42"

# Export multiple variables in one command
run_test "export VAR4=foo VAR5=bar && export | grep VAR" "VAR4=foo
VAR5=bar"

# Display the sorted environment (simulate 'export' without arguments)
run_test "export | grep VAR1" "VAR1"

# Variable with an invalid name (starts with a digit)
run_test "export 1INVALID=value" "not a valid identifier"

# Variable with a forbidden character
run_test "export INV-ALID=value" "not a valid identifier"

# Export without arguments (should not crash, must display the list)
run_test "export" "same"

# Export of a variable containing '=' in its value
run_test "export VAR6=foo=bar && echo \$VAR6" "foo=bar"

# Redefine a variable multiple times
run_test "export VAR7=init && export VAR7=updated && echo \$VAR7" "updated"

# Cleanup after export tests
unset VAR1 VAR2 VAR3 VAR4 VAR5 VAR6 VAR7

# ========= END =========
echo -e "${GREEN}Tous les tests ont été lancés.${RESET}"

