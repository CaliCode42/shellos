#!/bin/bash

# Always run from script directory
cd "$(dirname "$0")" || exit 1

# Colors
GREEN="\033[1;32m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
RESET="\033[0m"

# Executables
MINISHELL="$(cd "$(dirname "$0")" && pwd)/minishell"
BASH=/bin/bash

# Test function
run_test() {
    cmd="$1"
    expected="$2"

    echo -e "${YELLOW}Test: $cmd${RESET}"
    minishell_output=$(echo "$cmd" | $MINISHELL 2>&1)
    bash_output="minishell\$ $cmd"$'\n'"$($BASH -c "$cmd" 2>&1)"'\n''minishell$ exit\n'

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

# Initial cleanup
rm -f testfile out1 out2
rmdir testdir 2>/dev/null

###############################
#           TESTS            #
###############################

# Simple commands
run_test "echo hello" "same"
run_test "ls" "same"
run_test "pwd" "same"
run_test "true" ""
run_test "false" ""

# Simple redirections
run_test "echo line > testfile && cat testfile" "line"
run_test "echo add >> testfile && cat testfile" "line"$'\n'"add"
run_test "cat < testfile" "line"

# Redirection with error
run_test "cat < notfoundfile" "No such file or directory"

# Commands with pipes
run_test "echo hello | cat" "hello"
run_test "ls | cat" "same"
run_test "echo pipe | cat | cat | cat" "pipe"

# Redirection + pipe
run_test "echo start > out1 | echo ignored" "start"
run_test "cat < out1 | cat > out2 && cat out2" "start"

# Errors
run_test "commandeinconnue" "command not found"
run_test "echo error > /" "Is a directory"

# System cmd
run_test "mkdir testdir && ls | grep testdir" "testdir"
run_test "rmdir testdir && ls | grep testdir" ""

# Touch + redirection
run_test "touch file1 && echo hello > file1 && cat file1" "hello"

# sleep (long cmd test)
run_test "sleep 0.1 && echo done" "done"

# cd without quotes 
mkdir testdir
run_test "cd testdir && pwd" "$(pwd)/testdir"
cd ..

# exit not really tested here to avoid ending the prgm
run_test "exit" ""

###############################
#     SHORT EXPORT TESTS      #
###############################

# Testing the executable (debug script)
echo "MINISHELL='$MINISHELL'"

# New variable
run_test "export VAR1=hello && export | grep VAR1" "hello"

# Empty variable
run_test "export VAR2= && export | grep VAR2" ""

# Variable without value
run_test "export VAR3 && export | grep VAR3" "VAR3"

# Update existing variable
run_test "export VAR1=world && export | grep VAR1" "world"

# Invalid variable name
run_test "export 1INVALID=value" "not a valid identifier"

# Export without args (list environment)
run_test "export" "same"

# Cleanup
unset VAR1 VAR2 VAR3

# Cleanup
rm -f testfile out1 out2 file1
rmdir testdir 2>/dev/null

echo -e "${YELLOW}Tests terminés.${RESET}"
