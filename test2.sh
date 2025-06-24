#!/bin/bash

# ========= CONFIG =========
MINISHELL=./minishell
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

	# Pipe la commande dans minishell
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

# ========= END =========
echo -e "${GREEN}Tous les tests ont été lancés.${RESET}"

