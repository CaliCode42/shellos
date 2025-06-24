#!/bin/bash

# Couleurs
GREEN="\033[1;32m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
RESET="\033[0m"

# Exécutables
MINISHELL=./minishell
BASH=/bin/bash

# Fonction de test
run_test() {
	cmd="$1"
	expected="$2"

	echo -e "${YELLOW}Test: $cmd${RESET}"
	minishell_output=$(echo "$cmd" | $MINISHELL 2>&1 | sed 's/^minishell> //')
	bash_output=$($BASH -c "$cmd" 2>&1)

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

# Nettoyage initial
rm -f testfile out1 out2
rmdir testdir 2>/dev/null

###############################
#           TESTS            #
###############################

# Commandes simples
run_test "echo hello" "same"
run_test "ls" "same"
run_test "pwd" "same"
run_test "true" ""
run_test "false" ""

# Redirections simples
run_test "echo line > testfile && cat testfile" "line"
run_test "echo add >> testfile && cat testfile" "line"$'\n'"add"
run_test "cat < testfile" "line"

# Redirection avec erreur
run_test "cat < notfoundfile" "No such file or directory"

# Commandes avec pipes
run_test "echo hello | cat" "hello"
run_test "ls | cat" "same"
run_test "echo pipe | cat | cat | cat" "pipe"

# Redirection + pipe
run_test "echo start > out1 | echo ignored" "start"
run_test "cat < out1 | cat > out2 && cat out2" "start"

# Erreurs
run_test "commandeinconnue" "command not found"
run_test "echo error > /" "Is a directory"

# Commandes système
run_test "mkdir testdir && ls | grep testdir" "testdir"
run_test "rmdir testdir && ls | grep testdir" ""

# Touch + redirection
run_test "touch file1 && echo hello > file1 && cat file1" "hello"

# sleep (test de commande longue)
run_test "sleep 0.1 && echo done" "done"

# cd sans quotes (à adapter si ton shell gère `cd`)
mkdir testdir
run_test "cd testdir && pwd" "$(pwd)/testdir"
cd ..

# exit ne sera pas vraiment testé ici pour ne pas quitter le script
run_test "exit" ""

# Nettoyage
rm -f testfile out1 out2 file1
rmdir testdir 2>/dev/null

echo -e "${YELLOW}Tests terminés.${RESET}"

