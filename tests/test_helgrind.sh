#!/bin/bash
# Test de data races con Helgrind — parámetros variados para maximizar contención

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
PASS=0
FAIL=0

run_test() {
	desc="$1"
	shift
	max_errors=0
	for _ in 1 2 3; do
		output=$(valgrind --tool=helgrind ../codexion "$@" 2>&1)
		errors=$(echo "$output" | grep "ERROR SUMMARY" | tail -1 | grep -oP '\d+(?= errors)')
		[ "$errors" -gt "$max_errors" ] 2>/dev/null && max_errors=$errors
	done
	if [ "$max_errors" = "0" ]; then
		echo -e "${GREEN}[OK]${NC} $desc"
		((PASS++))
	else
		echo -e "${RED}[FAIL]${NC} $desc — $max_errors errors (worst of 3 runs)"
		((FAIL++))
	fi
}

echo "=== Helgrind data race tests ==="
echo ""

# Burnout rápido, alta contención
run_test "4 coders, burnout ajustado, FIFO"       4 410 200 200 200 5 60 fifo
run_test "4 coders, burnout ajustado, EDF"         4 410 200 200 200 5 60 edf

# Muchos coders, mucha competencia por dongles
run_test "10 coders, FIFO"                         10 500 200 200 200 3 50 fifo
run_test "10 coders, EDF"                          10 500 200 200 200 3 50 edf

# Tiempos mínimos, máxima frecuencia de lock/unlock
run_test "2 coders, tiempos cortos, FIFO"          2 300 100 100 100 5 10 fifo
run_test "3 coders, tiempos cortos, EDF"           3 300 100 100 100 5 10 edf

# Cooldown alto, más espera en dongles
run_test "4 coders, cooldown alto, FIFO"           4 2000 200 200 200 3 300 fifo
run_test "4 coders, cooldown alto, EDF"            4 2000 200 200 200 3 300 edf

# Caso 1 coder (path especial en main)
run_test "1 coder, burnout inevitable"             1 500 200 200 200 3 50 fifo

# Parámetros holgados (baseline)
run_test "4 coders, holgado, FIFO"                 4 2000 200 200 200 3 50 fifo
run_test "4 coders, holgado, EDF"                  4 2000 200 200 200 3 50 edf

echo ""
echo "=== Resultado: ${PASS} OK, ${FAIL} FAIL ==="
exit $FAIL
