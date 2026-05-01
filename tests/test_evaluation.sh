#!/bin/bash
# Test suite based on EVALUATION_SHEET.md for Codexion peer evaluation
# Usage: bash test_evaluation.sh [path_to_binary]

BIN="${1:-../codexion}"
PROJ_ROOT="${2:-..}"
PASS=0
FAIL=0
TOTAL=0
SECTION_PASS=0
SECTION_FAIL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

clean() { sed 's/\x1b\[[0-9;]*m//g' | sed 's/^  *//;s/  */ /g'; }
pass() { ((PASS++)); ((TOTAL++)); ((SECTION_PASS++)); echo -e "  ${GREEN}✓${NC} $1"; }
fail() { ((FAIL++)); ((TOTAL++)); ((SECTION_FAIL++)); echo -e "  ${RED}✗${NC} $1"; [ -n "$2" ] && echo -e "    ${RED}→ $2${NC}"; }
section() { SECTION_PASS=0; SECTION_FAIL=0; echo ""; echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"; echo -e "${YELLOW}  $1${NC}"; echo -e "${YELLOW}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"; }
section_result() { if [ "$SECTION_FAIL" -eq 0 ]; then echo -e "  ${GREEN}→ SECTION PASSED${NC}"; else echo -e "  ${RED}→ SECTION FAILED ($SECTION_FAIL failures)${NC}"; fi; }

echo -e "${CYAN}╔═══════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║     CODEXION — EVALUATION SHEET TEST SUITE       ║${NC}"
echo -e "${CYAN}╚═══════════════════════════════════════════════════╝${NC}"

# ═══════════════════════════════════════════════════════════════════════════
section "1. PRELIMINARIES — BASICS"
# ═══════════════════════════════════════════════════════════════════════════

# 1.1 Compilation
echo -e "\n${CYAN}  [1.1] Compilation${NC}"

# Makefile exists
[ -f "$PROJ_ROOT/Makefile" ] && pass "Makefile exists at project root" || fail "Makefile not found at project root"

# Required rules
for rule in all clean fclean re; do
  grep -qE "^${rule}:" "$PROJ_ROOT/Makefile" && pass "Makefile has '$rule' rule" || fail "Makefile missing '$rule' rule"
done

# NAME rule
grep -qE '^\$\(NAME\):' "$PROJ_ROOT/Makefile" && pass "Makefile has '\$(NAME)' rule" || fail "Makefile missing '\$(NAME)' rule"

# Compiles with correct flags
COMPILE_OUT=$(cd "$PROJ_ROOT" && make re 2>&1)
echo "$COMPILE_OUT" | grep -q "\-Wall" && echo "$COMPILE_OUT" | grep -q "\-Wextra" && echo "$COMPILE_OUT" | grep -q "\-Werror" && echo "$COMPILE_OUT" | grep -q "\-pthread" && pass "Compiles with -Wall -Wextra -Werror -pthread" || fail "Missing required compilation flags"

# No warnings (make re should succeed with -Werror)
echo "$COMPILE_OUT" | grep -qi "error:" && fail "Compilation errors" || pass "Compiles with no errors"

# Binary named codexion
[ -x "$PROJ_ROOT/codexion" ] && pass "Binary 'codexion' exists and is executable" || fail "Binary 'codexion' not found"

# No relink
RELINK_OUT=$(cd "$PROJ_ROOT" && make 2>&1)
echo "$RELINK_OUT" | grep -qE "^make.*Nothing to be done" && pass "No unnecessary relink" || fail "Makefile relinks unnecessarily" "$(echo "$RELINK_OUT" | head -2)"

# No libft
[ -d "$PROJ_ROOT/libft" ] && fail "libft directory found (not allowed)" || pass "No libft used"

# 1.2 Arguments
echo -e "\n${CYAN}  [1.2] Argument Validation${NC}"

$BIN 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "No arguments → should reject" || pass "No arguments → rejected"
$BIN 4 800 200 200 200 3 50 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "7 args → should reject" || pass "7 args (missing scheduler) → rejected"
$BIN 4 800 200 200 200 3 50 fifo extra 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "9 args → should reject" || pass "9 args (extra) → rejected"
$BIN 4 -1 200 200 200 3 50 fifo 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "Negative number → should reject" || pass "Negative number → rejected"
$BIN 4 800 abc 200 200 3 50 fifo 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "Non-integer → should reject" || pass "Non-integer → rejected"
$BIN 4 800 200 200 200 3 50 rr 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "Invalid scheduler 'rr' → should reject" || pass "Invalid scheduler 'rr' → rejected"
$BIN 4 800 200 200 200 3 50 FIFO 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "Uppercase 'FIFO' → should reject" || pass "Uppercase 'FIFO' → rejected (exact match)"

# 1.3 Allowed functions
echo -e "\n${CYAN}  [1.3] Allowed Functions${NC}"

FORBIDDEN=$(nm -u "$PROJ_ROOT/codexion" 2>/dev/null | grep -oP '(?<=U )\w+' | grep -vE '^(pthread_create|pthread_join|pthread_mutex_init|pthread_mutex_lock|pthread_mutex_unlock|pthread_mutex_destroy|pthread_cond_init|pthread_cond_wait|pthread_cond_timedwait|pthread_cond_broadcast|pthread_cond_destroy|gettimeofday|usleep|write|malloc|free|printf|fprintf|strcmp|strlen|atoi|memset|__.*|_.*)')
[ -z "$FORBIDDEN" ] && pass "No forbidden functions" || fail "Forbidden functions found" "$FORBIDDEN"

# 1.4 Log format
echo -e "\n${CYAN}  [1.4] Log Format${NC}"

OUTPUT=$(timeout 5 $BIN 3 5000 200 200 200 2 60 fifo 2>&1 | clean)
VALID=$(echo "$OUTPUT" | grep -cE '^[0-9]+ [0-9]+ (has taken a dongle|is compiling|is debugging|is refactoring|burned out)$')
TOTAL_LINES=$(echo "$OUTPUT" | wc -l)
[ "$VALID" -eq "$TOTAL_LINES" ] && pass "All lines match log format" || fail "Invalid log format" "Valid: $VALID / Total: $TOTAL_LINES"

DISORDER=$(echo "$OUTPUT" | awk 'NR>1 && $1 < prev {c++} {prev=$1} END {print c+0}')
[ "$DISORDER" -eq 0 ] && pass "Timestamps non-decreasing" || fail "Timestamps out of order"

section_result

# ═══════════════════════════════════════════════════════════════════════════
section "2. GLOBAL VARIABLES"
# ═══════════════════════════════════════════════════════════════════════════

GLOBALS=$(nm "$PROJ_ROOT/codexion" 2>/dev/null | grep -E ' [BD] ' | grep -vE '_DYNAMIC|_GLOBAL_OFFSET|_edata|_end|stderr|stdout|stdin|__')
[ -z "$GLOBALS" ] && pass "No mutable global variables" || fail "Mutable global variables found" "$GLOBALS"

section_result

# ═══════════════════════════════════════════════════════════════════════════
section "3. README.md"
# ═══════════════════════════════════════════════════════════════════════════

README="$PROJ_ROOT/README.md"
if [ -f "$README" ]; then
  pass "README.md exists"
  head -1 "$README" | grep -qi "this project has been created as part of the 42 curriculum" && pass "First line: 42 curriculum attribution" || fail "First line missing 42 curriculum attribution"
  grep -qi "description" "$README" && pass "Has Description section" || fail "Missing Description section"
  grep -qi "instruction" "$README" && pass "Has Instructions section" || fail "Missing Instructions section"
  grep -qi "resource" "$README" && pass "Has Resources section" || fail "Missing Resources section"
  grep -qi "blocking" "$README" && pass "Has Blocking cases section" || fail "Missing Blocking cases handled section"
  grep -qi "synchronization\|thread.*mechanism" "$README" && pass "Has Thread synchronization section" || fail "Missing Thread synchronization mechanisms section"
else
  fail "README.md not found"
fi

section_result

# ═══════════════════════════════════════════════════════════════════════════
section "4. EASY TESTS"
# ═══════════════════════════════════════════════════════════════════════════

# 4.1 Basic FIFO — 4 coders, comfortable timings
echo -e "\n${CYAN}  [4.1] ./codexion 4 410 200 200 200 5 60 fifo${NC}"
OUTPUT=$(timeout 10 $BIN 4 410 200 200 200 5 60 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "is compiling" && pass "4 coders FIFO: simulation runs" || fail "4 coders FIFO: no output"
EXIT=$?
# With 4 coders and 410ms burnout, burnout may or may not happen depending on scheduling
# Check that if no burnout, all coders compiled enough
if ! echo "$OUTPUT" | grep -q "burned out"; then
  for C in 1 2 3 4; do
    COUNT=$(echo "$OUTPUT" | awk -v c="$C" '$2 == c' | grep -c "is compiling")
    [ "$COUNT" -ge 5 ] && pass "Coder $C compiled $COUNT times (required: 5)" || fail "Coder $C compiled only $COUNT times (required: 5)"
  done
else
  pass "4 coders FIFO: burnout detected (tight params, acceptable)"
fi

# 4.2 Basic EDF — 5 coders
echo -e "\n${CYAN}  [4.2] ./codexion 5 850 200 200 100 6 80 edf${NC}"
OUTPUT=$(timeout 15 $BIN 5 850 200 200 100 6 80 edf 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && fail "5 coders EDF: unexpected burnout" || pass "5 coders EDF: no burnout"
for C in 1 2 3 4 5; do
  COUNT=$(echo "$OUTPUT" | awk -v c="$C" '$2 == c' | grep -c "is compiling")
  [ "$COUNT" -ge 6 ] && pass "Coder $C compiled $COUNT times (required: 6)" || fail "Coder $C compiled only $COUNT times (required: 6)"
done

# 4.3 Single coder — must burn out (only 1 dongle, needs 2)
echo -e "\n${CYAN}  [4.3] ./codexion 1 500 200 200 200 3 60 fifo${NC}"
OUTPUT=$(timeout 3 $BIN 1 500 200 200 200 3 60 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && pass "1 coder: burns out (only 1 dongle)" || fail "1 coder: should burn out"
BURNOUT_T=$(echo "$OUTPUT" | grep "burned out" | awk '{print $1}')
if [ -n "$BURNOUT_T" ]; then
  DIFF=$((BURNOUT_T - 500))
  [ "$DIFF" -ge -10 ] && [ "$DIFF" -le 10 ] && pass "Burnout at ${BURNOUT_T}ms (expected 500±10ms)" || fail "Burnout timing off: ${BURNOUT_T}ms (expected 500±10ms)"
fi

# 4.4 Log format: two dongles before each compile
echo -e "\n${CYAN}  [4.4] Lifecycle check${NC}"
OUTPUT=$(timeout 10 $BIN 3 5000 200 200 200 2 60 fifo 2>&1 | clean)
SEMANTIC_FAIL=0
for C in $(seq 1 3); do
  DONGLES=0
  while IFS= read -r line; do
    case "$line" in
      *"has taken a dongle"*) ((DONGLES++)) ;;
      *"is compiling"*) [ "$DONGLES" -lt 2 ] && ((SEMANTIC_FAIL++)); DONGLES=0 ;;
      *) DONGLES=0 ;;
    esac
  done <<< "$(echo "$OUTPUT" | awk -v c="$C" '$2 == c')"
done
[ "$SEMANTIC_FAIL" -eq 0 ] && pass "Every compile preceded by 2 dongle takes" || fail "Compile without 2 dongles" "$SEMANTIC_FAIL violations"

section_result

# ═══════════════════════════════════════════════════════════════════════════
section "5. LESS EASY TESTS"
# ═══════════════════════════════════════════════════════════════════════════

# 5.1 Tight burnout EDF — must NOT burn out
echo -e "\n${CYAN}  [5.1] ./codexion 5 850 200 200 100 6 80 edf${NC}"
OUTPUT=$(timeout 15 $BIN 5 850 200 200 100 6 80 edf 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && fail "Tight EDF: unexpected burnout" || pass "Tight EDF: no burnout"

# 5.2 Slightly looser
echo -e "\n${CYAN}  [5.2] ./codexion 5 980 200 200 100 6 80 edf${NC}"
OUTPUT=$(timeout 15 $BIN 5 980 200 200 100 6 80 edf 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && fail "Looser EDF: unexpected burnout" || pass "Looser EDF: no burnout"

# 5.3 3 coders moderate pressure
echo -e "\n${CYAN}  [5.3] ./codexion 3 850 200 200 100 6 80 edf${NC}"
OUTPUT=$(timeout 15 $BIN 3 850 200 200 100 6 80 edf 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && fail "3 coders EDF: unexpected burnout" || pass "3 coders EDF: no burnout"

# 5.4 Burnout scenario — should burn out
echo -e "\n${CYAN}  [5.4] ./codexion 4 300 200 200 200 5 60 fifo${NC}"
OUTPUT=$(timeout 5 $BIN 4 300 200 200 200 5 60 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && pass "Burnout scenario: burnout detected" || fail "Burnout scenario: should burn out"

# 5.5 Correct state transitions
echo -e "\n${CYAN}  [5.5] State transitions${NC}"
OUTPUT=$(timeout 10 $BIN 4 5000 200 200 200 3 60 fifo 2>&1 | clean)
for C in 1 2 3 4; do
  ACTIONS=$(echo "$OUTPUT" | awk -v c="$C" '$2 == c' | grep -oE '(is compiling|is debugging|is refactoring)' | head -3)
  EXPECTED=$(printf "is compiling\nis debugging\nis refactoring")
  [ "$ACTIONS" = "$EXPECTED" ] && pass "Coder $C: correct cycle (compile→debug→refactor)" || fail "Coder $C: wrong cycle order"
done

# 5.6 No simultaneous dongle use by adjacent coders
OVERLAP=0
for C in $(seq 1 4); do
  NEXT=$(( (C % 4) + 1 ))
  while IFS= read -r line; do
    TS=$(echo "$line" | awk '{print $1}')
    while IFS= read -r line2; do
      TS2=$(echo "$line2" | awk '{print $1}')
      [ "$TS" -lt $((TS2 + 200)) ] && [ "$TS2" -lt $((TS + 200)) ] && ((OVERLAP++))
    done <<< "$(echo "$OUTPUT" | awk -v c="$NEXT" '$2 == c && /is compiling/')"
  done <<< "$(echo "$OUTPUT" | awk -v c="$C" '$2 == c && /is compiling/')"
done
[ "$OVERLAP" -eq 0 ] && pass "No adjacent coders compile simultaneously" || fail "Adjacent coders overlap" "$OVERLAP overlaps"

section_result

# ═══════════════════════════════════════════════════════════════════════════
section "6. MEDIUM TESTS"
# ═══════════════════════════════════════════════════════════════════════════

# 6.1 Cooldown test
echo -e "\n${CYAN}  [6.1] ./codexion 4 800 200 200 200 5 200 fifo${NC}"
OUTPUT=$(timeout 15 $BIN 4 800 200 200 200 5 200 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "is compiling" && pass "Cooldown 200ms: runs correctly" || fail "Cooldown 200ms: failed to run"

# 6.2 FIFO vs EDF produce different scheduling
echo -e "\n${CYAN}  [6.2] FIFO vs EDF comparison${NC}"
OUT_FIFO=$(timeout 15 $BIN 5 850 200 200 100 6 80 fifo 2>&1 | clean)
OUT_EDF=$(timeout 15 $BIN 5 850 200 200 100 6 80 edf 2>&1 | clean)
FIFO_ORDER=$(echo "$OUT_FIFO" | grep "is compiling" | awk '{print $2}' | tr '\n' ' ')
EDF_ORDER=$(echo "$OUT_EDF" | grep "is compiling" | awk '{print $2}' | tr '\n' ' ')
[ "$FIFO_ORDER" != "$EDF_ORDER" ] && pass "FIFO and EDF produce different scheduling" || fail "FIFO and EDF produce identical scheduling"

# 6.3 Log serialization under load
echo -e "\n${CYAN}  [6.3] ./codexion 10 1000 200 200 200 5 80 fifo${NC}"
OUTPUT=$(timeout 20 $BIN 10 1000 200 200 200 5 80 fifo 2>&1 | clean)
CORRUPTED=$(echo "$OUTPUT" | grep -cvE '^[0-9]+ [0-9]+ (has taken a dongle|is compiling|is debugging|is refactoring|burned out)$')
[ "$CORRUPTED" -eq 0 ] && pass "10 coders: no interleaved log lines" || fail "Interleaved log lines detected" "$CORRUPTED invalid lines"

# 6.4 Refactoring timing
echo -e "\n${CYAN}  [6.4] ./codexion 3 1200 200 200 400 4 80 edf${NC}"
OUTPUT=$(timeout 15 $BIN 3 1200 200 200 400 4 80 edf 2>&1 | clean)
REFACTOR_START=$(echo "$OUTPUT" | awk '/1 is refactoring/ {print $1; exit}')
NEXT_DONGLE=$(echo "$OUTPUT" | awk -v rs="$REFACTOR_START" '$1 > rs && $2 == 1 && /has taken a dongle/ {print $1; exit}')
if [ -n "$REFACTOR_START" ] && [ -n "$NEXT_DONGLE" ]; then
  REFACTOR_DUR=$((NEXT_DONGLE - REFACTOR_START))
  [ "$REFACTOR_DUR" -ge 390 ] && [ "$REFACTOR_DUR" -le 450 ] && pass "Refactor duration: ${REFACTOR_DUR}ms (expected ~400ms)" || fail "Refactor duration off: ${REFACTOR_DUR}ms (expected 400±50ms)"
else
  fail "Could not measure refactor duration"
fi

section_result

# ═══════════════════════════════════════════════════════════════════════════
section "7. BURNOUT PRECISION"
# ═══════════════════════════════════════════════════════════════════════════

# Burnout must be within 10ms
echo -e "\n${CYAN}  [7.1] Burnout ≤10ms precision${NC}"
PRECISION_OK=0
PRECISION_FAIL=0
for i in $(seq 1 5); do
  OUTPUT=$(timeout 3 $BIN 5 600 200 200 200 10 100 fifo 2>&1 | clean)
  BT=$(echo "$OUTPUT" | grep "burned out" | awk '{print $1}')
  if [ -n "$BT" ]; then
    DIFF=$((BT - 600))
    [ "$DIFF" -ge -10 ] && [ "$DIFF" -le 10 ] && ((PRECISION_OK++)) || ((PRECISION_FAIL++))
  fi
done
[ "$PRECISION_FAIL" -eq 0 ] && pass "Burnout within 10ms in $PRECISION_OK/$((PRECISION_OK+PRECISION_FAIL)) runs" || fail "Burnout precision exceeded 10ms" "$PRECISION_FAIL/$((PRECISION_OK+PRECISION_FAIL)) runs off"

# No messages after burnout
echo -e "\n${CYAN}  [7.2] No messages after burnout${NC}"
OUTPUT=$(timeout 3 $BIN 5 400 200 200 200 10 60 fifo 2>&1 | clean)
BURNOUT_LINE=$(echo "$OUTPUT" | grep -n "burned out" | head -1 | cut -d: -f1)
TOTAL_LINES=$(echo "$OUTPUT" | wc -l)
if [ -n "$BURNOUT_LINE" ]; then
  [ "$BURNOUT_LINE" -eq "$TOTAL_LINES" ] && pass "No messages after burnout" || fail "Messages after burnout" "Burnout at line $BURNOUT_LINE, total $TOTAL_LINES"
else
  fail "No burnout detected"
fi

section_result

# ═══════════════════════════════════════════════════════════════════════════
section "8. MEMORY & SAFETY"
# ═══════════════════════════════════════════════════════════════════════════

# 8.1 Memory leaks
echo -e "\n${CYAN}  [8.1] Memory leaks (valgrind)${NC}"
LEAK_FAIL=0
for params in "4 410 200 200 200 3 60 fifo" "5 850 200 200 100 6 80 edf" "1 500 200 200 200 3 60 fifo" "5 600 200 200 200 10 100 fifo"; do
  LEAKS=$(valgrind --leak-check=full $BIN $params 2>&1 | grep "definitely lost" | grep -oP '\d+(?= bytes)')
  if [ -n "$LEAKS" ] && [ "$LEAKS" -gt 0 ]; then
    fail "Leak: $LEAKS bytes ($params)"
    ((LEAK_FAIL++))
  fi
done
[ "$LEAK_FAIL" -eq 0 ] && pass "No memory leaks in all scenarios"

# 8.2 Data races (helgrind)
echo -e "\n${CYAN}  [8.2] Data races (helgrind)${NC}"
RACE_FAIL=0
for params in "4 410 200 200 200 3 60 fifo" "4 410 200 200 200 3 60 edf" "5 850 200 200 100 6 80 edf"; do
  WORST=0
  for run in 1 2 3; do
    ERRORS=$(valgrind --tool=helgrind $BIN $params 2>&1 | grep "ERROR SUMMARY" | tail -1 | grep -oP '\d+(?= errors)')
    [ "$ERRORS" -gt "$WORST" ] 2>/dev/null && WORST=$ERRORS
  done
  if [ "$WORST" -gt 0 ]; then
    fail "Data race: $WORST errors ($params)"
    ((RACE_FAIL++))
  fi
done
[ "$RACE_FAIL" -eq 0 ] && pass "No data races in all scenarios"

# 8.3 Deadlock stress
echo -e "\n${CYAN}  [8.3] Deadlock stress${NC}"
DEADLOCKS=0
for i in $(seq 1 20); do
  timeout 3 $BIN 5 5000 100 100 100 3 20 fifo > /dev/null 2>&1
  [ $? -ne 0 ] && ((DEADLOCKS++))
done
[ "$DEADLOCKS" -eq 0 ] && pass "No deadlocks in 20 runs (5 coders FIFO)" || fail "Possible deadlock" "$DEADLOCKS/20 runs timed out"

DEADLOCKS=0
for i in $(seq 1 20); do
  timeout 3 $BIN 4 5000 100 100 100 3 20 edf > /dev/null 2>&1
  [ $? -ne 0 ] && ((DEADLOCKS++))
done
[ "$DEADLOCKS" -eq 0 ] && pass "No deadlocks in 20 runs (4 coders EDF)" || fail "Possible deadlock" "$DEADLOCKS/20 runs timed out"

# 8.4 Segfault / abort stress
echo -e "\n${CYAN}  [8.4] Crash stress${NC}"
CRASHES=0
for i in $(seq 1 50); do
  timeout 2 $BIN 8 3000 20 20 20 5 5 fifo > /dev/null 2>&1
  RET=$?
  [ "$RET" -eq 139 ] || [ "$RET" -eq 134 ] && ((CRASHES++))
done
[ "$CRASHES" -eq 0 ] && pass "50 runs: 0 crashes (segfault/abort)" || fail "Crashes detected" "$CRASHES/50"

section_result

# ═══════════════════════════════════════════════════════════════════════════
echo ""
echo -e "${CYAN}╔═══════════════════════════════════════════════════╗${NC}"
if [ "$FAIL" -eq 0 ]; then
  echo -e "${CYAN}║${NC}  ${GREEN}RESULTS: $PASS passed, $FAIL failed, $TOTAL total${NC}      ${CYAN}║${NC}"
else
  echo -e "${CYAN}║${NC}  ${GREEN}$PASS passed${NC}, ${RED}$FAIL failed${NC}, $TOTAL total              ${CYAN}║${NC}"
fi
echo -e "${CYAN}╚═══════════════════════════════════════════════════╝${NC}"

exit $FAIL
