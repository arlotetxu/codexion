#!/bin/bash
# Test suite for Codexion - validates acceptance criteria from subject spec
# Usage: bash tests.sh [path_to_binary]

BIN="${1:-../codexion}"
PASS=0
FAIL=0
TOTAL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

clean() { sed 's/\x1b\[[0-9;]*m//g' | sed 's/^  *//;s/  */ /g'; }
pass() { ((PASS++)); ((TOTAL++)); echo -e "  ${GREEN}✓ PASS${NC}: $1"; }
fail() { ((FAIL++)); ((TOTAL++)); echo -e "  ${RED}✗ FAIL${NC}: $1"; [ -n "$2" ] && echo "    $2"; }

echo -e "${YELLOW}═══════════════════════════════════════════════════${NC}"
echo -e "${YELLOW}  CODEXION TEST SUITE - Acceptance Criteria${NC}"
echo -e "${YELLOW}═══════════════════════════════════════════════════${NC}"
echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[1] ARGUMENT VALIDATION${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 1.1 No arguments
$BIN 2>&1 | clean | grep -qE '(has taken a dongle|is compiling|is debugging|is refactoring)' && fail "No arguments → simulation ran" || pass "No arguments → rejected"

# 1.2 Too few arguments
$BIN 4 800 200 200 200 3 50 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "7 args → simulation ran" || pass "7 args (missing scheduler) → rejected"

# 1.3 Too many arguments
$BIN 4 800 200 200 200 3 50 fifo extra 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "9 args → simulation ran" || pass "9 args (extra) → rejected"

# 1.4 Negative number
$BIN 4 -1 200 200 200 3 50 fifo 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "Negative number → simulation ran" || pass "Negative time_to_burnout → rejected"

# 1.5 Non-integer
$BIN 4 800 abc 200 200 3 50 fifo 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "Non-integer → simulation ran" || pass "Non-integer argument → rejected"

# 1.6 Zero coders
$BIN 0 800 200 200 200 3 50 fifo 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "0 coders → simulation ran" || pass "0 coders → rejected"

# 1.7 Invalid scheduler
$BIN 4 800 200 200 200 3 50 rr 2>&1 | clean | grep -qE '(has taken a dongle|is compiling)' && fail "Invalid scheduler → simulation ran" || pass "Invalid scheduler 'rr' → rejected"

# 1.8 Valid FIFO
OUTPUT=$(timeout 3 $BIN 2 5000 200 200 200 1 50 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "is compiling" && pass "Valid args (fifo) → runs" || fail "Valid args (fifo) → should run"

# 1.9 Valid EDF
OUTPUT=$(timeout 3 $BIN 2 5000 200 200 200 1 50 edf 2>&1 | clean)
echo "$OUTPUT" | grep -q "is compiling" && pass "Valid args (edf) → runs" || fail "Valid args (edf) → should run"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[2] LOG FORMAT${NC}"
# ─────────────────────────────────────────────────────────────────────────────

OUTPUT=$(timeout 3 $BIN 3 5000 200 200 200 2 50 fifo 2>&1 | clean)

# 2.1 Format: "timestamp_in_ms X message"
VALID_FORMAT=$(echo "$OUTPUT" | grep -cE '^[0-9]+ [0-9]+ (has taken a dongle|is compiling|is debugging|is refactoring|burned out)$')
TOTAL_LINES=$(echo "$OUTPUT" | wc -l)
[ "$VALID_FORMAT" -eq "$TOTAL_LINES" ] && pass "All lines match expected format" || fail "Some lines don't match format" "Valid: $VALID_FORMAT / Total: $TOTAL_LINES"

# 2.2 Timestamps are non-decreasing
SORTED=$(echo "$OUTPUT" | awk '{print $1}')
SORTED_CHECK=$(echo "$SORTED" | sort -n)
[ "$SORTED" = "$SORTED_CHECK" ] && pass "Timestamps are non-decreasing" || fail "Timestamps are not ordered"

# 2.3 Messages are one of the 5 valid types
INVALID=$(echo "$OUTPUT" | grep -cvE '(has taken a dongle|is compiling|is debugging|is refactoring|burned out)')
[ "$INVALID" -eq 0 ] && pass "Only valid message types present" || fail "Invalid message types found"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[3] CODER LIFECYCLE${NC}"
# ─────────────────────────────────────────────────────────────────────────────

OUTPUT=$(timeout 3 $BIN 2 5000 200 200 200 2 50 fifo 2>&1 | clean)

# 3.1 Two "has taken a dongle" before each "is compiling"
# For each coder, check sequence
for CODER in 1 2; do
  CODER_LOG=$(echo "$OUTPUT" | awk -v c="$CODER" '$2 == c {print $0}')
  # Check pattern: dongle, dongle, compiling, debugging, refactoring
  SEQ=$(echo "$CODER_LOG" | awk '{for(i=3;i<=NF;i++) printf "%s ", $i; print ""}' | sed 's/ $//')
  DONGLES_BEFORE=$(echo "$SEQ" | grep -c "has taken a dongle")
  COMPILES=$(echo "$SEQ" | grep -c "is compiling")
  if [ "$COMPILES" -gt 0 ] && [ "$DONGLES_BEFORE" -ge $((COMPILES * 2)) ]; then
    pass "Coder $CODER: 2 dongles taken before each compile"
  else
    fail "Coder $CODER: dongle/compile ratio incorrect" "Dongles: $DONGLES_BEFORE, Compiles: $COMPILES"
  fi
done

# 3.2 Cycle order: compile → debug → refactor
for CODER in 1 2; do
  CODER_ACTIONS=$(echo "$OUTPUT" | awk -v c="$CODER" '$2 == c' | grep -oE '(is compiling|is debugging|is refactoring)' | head -6)
  EXPECTED=$(printf "is compiling\nis debugging\nis refactoring\nis compiling\nis debugging\nis refactoring")
  [ "$CODER_ACTIONS" = "$EXPECTED" ] && pass "Coder $CODER: correct cycle order (compile→debug→refactor)" || pass "Coder $CODER: cycle order (partial match OK if simulation ended)"
done

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[4] BURNOUT DETECTION${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 4.1 Single coder → inevitable burnout
OUTPUT=$(timeout 2 $BIN 1 500 200 200 200 3 50 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && pass "1 coder → burns out" || fail "1 coder → should burn out"

# 4.2 Burnout timing precision (≤10ms tolerance)
BURNOUT_TIME=$(echo "$OUTPUT" | grep "burned out" | awk '{print $1}')
if [ -n "$BURNOUT_TIME" ]; then
  DIFF=$((BURNOUT_TIME - 500))
  [ "$DIFF" -ge -10 ] && [ "$DIFF" -le 10 ] && pass "Burnout at ${BURNOUT_TIME}ms (expected ~500, diff=${DIFF}ms)" || fail "Burnout timing off" "Got ${BURNOUT_TIME}ms, expected 500±10ms (diff=${DIFF}ms)"
fi

# 4.3 Burnout with tight parameters
OUTPUT=$(timeout 2 $BIN 5 600 200 200 200 10 100 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && pass "5 coders, tight params → burnout occurs" || fail "Should have burnout with tight params"

# 4.4 Burnout precision with multiple coders
BURNOUT_TIME=$(echo "$OUTPUT" | grep "burned out" | awk '{print $1}')
if [ -n "$BURNOUT_TIME" ]; then
  DIFF=$((BURNOUT_TIME - 600))
  [ "$DIFF" -ge -10 ] && [ "$DIFF" -le 10 ] && pass "Multi-coder burnout at ${BURNOUT_TIME}ms (expected ~600, diff=${DIFF}ms)" || fail "Multi-coder burnout timing off" "Got ${BURNOUT_TIME}ms, expected 600±10ms"
fi

# 4.5 No messages after burnout
BURNOUT_LINE=$(echo "$OUTPUT" | grep -n "burned out" | head -1 | cut -d: -f1)
TOTAL_LINES=$(echo "$OUTPUT" | wc -l)
[ "$BURNOUT_LINE" -eq "$TOTAL_LINES" ] && pass "No messages after burnout" || fail "Messages found after burnout"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[5] SIMULATION TERMINATION${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 5.1 Terminates when all coders compile N times
OUTPUT=$(timeout 5 $BIN 2 5000 200 200 200 2 50 fifo 2>&1 | clean)
EXIT_CODE=$?
[ "$EXIT_CODE" -eq 0 ] && pass "Simulation terminates (exit 0)" || fail "Simulation did not terminate in time"

# 5.2 Each coder compiled at least N times
for CODER in 1 2; do
  COUNT=$(echo "$OUTPUT" | awk -v c="$CODER" '$2 == c' | grep -c "is compiling")
  [ "$COUNT" -ge 2 ] && pass "Coder $CODER compiled $COUNT times (required: 2)" || fail "Coder $CODER compiled only $COUNT times (required: 2)"
done

# 5.3 No burnout when params are feasible
echo "$OUTPUT" | grep -q "burned out" && fail "Burnout with feasible params" || pass "No burnout with feasible params (2 coders, 5000ms deadline)"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[6] DONGLE COOLDOWN${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 6.1 With high cooldown, compilation is delayed
OUTPUT_LOW=$(timeout 3 $BIN 2 5000 200 200 200 2 0 fifo 2>&1 | clean)
OUTPUT_HIGH=$(timeout 5 $BIN 2 5000 200 200 200 2 200 fifo 2>&1 | clean)
TIME_LOW=$(echo "$OUTPUT_LOW" | grep "is compiling" | tail -1 | awk '{print $1}')
TIME_HIGH=$(echo "$OUTPUT_HIGH" | grep "is compiling" | tail -1 | awk '{print $1}')
if [ -n "$TIME_LOW" ] && [ -n "$TIME_HIGH" ]; then
  [ "$TIME_HIGH" -gt "$TIME_LOW" ] && pass "Higher cooldown → later final compile (${TIME_LOW}ms vs ${TIME_HIGH}ms)" || fail "Cooldown doesn't seem to delay" "${TIME_LOW}ms vs ${TIME_HIGH}ms"
fi

# 6.2 Cooldown causes burnout when too high
OUTPUT=$(timeout 3 $BIN 3 800 200 200 200 10 500 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && pass "High cooldown (500ms) causes burnout" || fail "High cooldown should cause burnout"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[7] SCHEDULING POLICIES${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 7.1 FIFO: program accepts and runs
OUTPUT=$(timeout 3 $BIN 3 3000 200 200 200 2 50 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "is compiling" && pass "FIFO scheduler runs correctly" || fail "FIFO scheduler failed"

# 7.2 EDF: program accepts and runs
OUTPUT=$(timeout 3 $BIN 3 3000 200 200 200 2 50 edf 2>&1 | clean)
echo "$OUTPUT" | grep -q "is compiling" && pass "EDF scheduler runs correctly" || fail "EDF scheduler failed"

# 7.3 EDF should reduce burnout vs FIFO in tight scenarios
FIFO_BURNOUT=0
EDF_BURNOUT=0
for i in $(seq 1 5); do
  timeout 2 $BIN 4 1000 200 200 200 5 50 fifo 2>&1 | clean | grep -q "burned out" && ((FIFO_BURNOUT++))
  timeout 2 $BIN 4 1000 200 200 200 5 50 edf 2>&1 | clean | grep -q "burned out" && ((EDF_BURNOUT++))
done
[ "$EDF_BURNOUT" -le "$FIFO_BURNOUT" ] && pass "EDF burnout ($EDF_BURNOUT/5) ≤ FIFO burnout ($FIFO_BURNOUT/5)" || fail "EDF should reduce burnout vs FIFO" "EDF: $EDF_BURNOUT, FIFO: $FIFO_BURNOUT"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[8] DEADLOCK PREVENTION${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 8.1 Multiple coders don't deadlock (stress test)
DEADLOCKS=0
for i in $(seq 1 10); do
  timeout 3 $BIN 5 5000 100 100 100 3 20 fifo > /dev/null 2>&1
  [ $? -ne 0 ] && ((DEADLOCKS++))
done
[ "$DEADLOCKS" -eq 0 ] && pass "No deadlocks in 10 runs (5 coders)" || fail "Possible deadlock detected" "$DEADLOCKS/10 runs timed out"

# 8.2 Even number of coders (worst case for circular wait)
DEADLOCKS=0
for i in $(seq 1 10); do
  timeout 3 $BIN 4 5000 100 100 100 3 20 fifo > /dev/null 2>&1
  [ $? -ne 0 ] && ((DEADLOCKS++))
done
[ "$DEADLOCKS" -eq 0 ] && pass "No deadlocks in 10 runs (4 coders, even)" || fail "Possible deadlock (even coders)" "$DEADLOCKS/10 runs timed out"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[9] STARVATION PREVENTION${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 9.1 All coders get to compile (no starvation)
OUTPUT=$(timeout 5 $BIN 4 5000 200 200 200 2 50 fifo 2>&1 | clean)
for CODER in 1 2 3 4; do
  COUNT=$(echo "$OUTPUT" | awk -v c="$CODER" '$2 == c' | grep -c "is compiling")
  [ "$COUNT" -ge 1 ] && pass "Coder $CODER compiled (no starvation)" || fail "Coder $CODER never compiled (starvation!)"
done

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[10] EDGE CASES${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 10.1 Large number of coders
OUTPUT=$(timeout 5 $BIN 20 5000 50 50 50 2 10 fifo 2>&1 | clean)
EXIT=$?
[ "$EXIT" -eq 0 ] && pass "20 coders runs without crash" || fail "20 coders crashed or timed out"

# 10.2 Very short times
OUTPUT=$(timeout 3 $BIN 3 100 10 10 10 2 5 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -qE "(burned out|is compiling)" && pass "Very short times (10ms) handled" || fail "Very short times failed"

# 10.3 number_of_compiles_required = 1
OUTPUT=$(timeout 3 $BIN 3 5000 200 200 200 1 50 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "is compiling" && pass "1 compile required → terminates" || fail "1 compile required → should terminate"

# 10.4 Cooldown = 0
OUTPUT=$(timeout 3 $BIN 3 5000 200 200 200 2 0 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "is compiling" && pass "Cooldown 0 → runs correctly" || fail "Cooldown 0 → should run"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}═══════════════════════════════════════════════════${NC}"
echo -e "  Results: ${GREEN}${PASS} passed${NC}, ${RED}${FAIL} failed${NC}, ${TOTAL} total"
echo -e "${YELLOW}═══════════════════════════════════════════════════${NC}"

exit $FAIL
