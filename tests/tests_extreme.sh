#!/bin/bash
# Extreme stress tests for Codexion
# These tests push concurrency limits and expose race conditions
# Usage: bash tests_extreme.sh [path_to_binary]

BIN="${1:-../codexion}"
PASS=0
FAIL=0
TOTAL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

pass() { ((PASS++)); ((TOTAL++)); echo -e "  ${GREEN}✓ PASS${NC}: $1"; }
fail() { ((FAIL++)); ((TOTAL++)); echo -e "  ${RED}✗ FAIL${NC}: $1"; [ -n "$2" ] && echo "    $2"; }
clean() { sed 's/\x1b\[[0-9;]*m//g' | sed 's/^  *//;s/  */ /g'; }

echo -e "${YELLOW}═══════════════════════════════════════════════════${NC}"
echo -e "${YELLOW}  CODEXION EXTREME STRESS TESTS${NC}"
echo -e "${YELLOW}═══════════════════════════════════════════════════${NC}"
echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[1] MASSIVE CONCURRENCY${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 1.1 200 coders
echo -n "  Running 200 coders... "
OUTPUT=$(timeout 10 $BIN 200 10000 50 50 50 2 10 fifo 2>&1 | clean)
EXIT=$?
[ "$EXIT" -eq 0 ] && pass "200 coders terminates without crash" || fail "200 coders crashed/timed out (exit $EXIT)"

# 1.2 100 coders with EDF
echo -n "  Running 100 coders EDF... "
OUTPUT=$(timeout 10 $BIN 100 10000 50 50 50 2 10 edf 2>&1 | clean)
EXIT=$?
[ "$EXIT" -eq 0 ] && pass "100 coders EDF terminates" || fail "100 coders EDF crashed/timed out"

# 1.3 50 coders, very short times (1ms each phase)
echo -n "  Running 50 coders, 1ms phases... "
OUTPUT=$(timeout 5 $BIN 50 500 1 1 1 5 0 fifo 2>&1 | clean)
EXIT=$?
[ "$EXIT" -eq 0 ] && pass "50 coders with 1ms phases" || fail "50 coders 1ms phases crashed"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[2] RACE CONDITION HUNTING (repetition)${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 2.1 Run 200 times with 4 coders, short times
CRASHES=0
HANGS=0
echo -n "  Running 200 iterations (4 coders, 50ms phases)... "
for i in $(seq 1 200); do
  timeout 2 $BIN 4 2000 50 50 50 3 10 fifo > /dev/null 2>&1
  RET=$?
  [ "$RET" -eq 124 ] && ((HANGS++))
  [ "$RET" -gt 124 ] && ((CRASHES++))
done
[ "$CRASHES" -eq 0 ] && [ "$HANGS" -eq 0 ] && pass "200 iterations: 0 crashes, 0 hangs" || fail "200 iterations" "Crashes: $CRASHES, Hangs: $HANGS"

# 2.2 Run 100 times with EDF
CRASHES=0
HANGS=0
echo -n "  Running 100 iterations (EDF, 5 coders)... "
for i in $(seq 1 100); do
  timeout 2 $BIN 5 2000 50 50 50 3 10 edf > /dev/null 2>&1
  RET=$?
  [ "$RET" -eq 124 ] && ((HANGS++))
  [ "$RET" -gt 124 ] && ((CRASHES++))
done
[ "$CRASHES" -eq 0 ] && [ "$HANGS" -eq 0 ] && pass "100 EDF iterations: 0 crashes, 0 hangs" || fail "100 EDF iterations" "Crashes: $CRASHES, Hangs: $HANGS"

# 2.3 Rapid fire: 500 runs with minimal params
CRASHES=0
echo -n "  Running 500 rapid iterations (2 coders, 1ms)... "
for i in $(seq 1 500); do
  timeout 1 $BIN 2 500 1 1 1 3 0 fifo > /dev/null 2>&1
  [ $? -gt 124 ] && ((CRASHES++))
done
[ "$CRASHES" -eq 0 ] && pass "500 rapid runs: 0 crashes" || fail "500 rapid runs" "Crashes: $CRASHES"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[3] BOUNDARY BURNOUT TIMING${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 3.1 Burnout exactly at boundary: with 2 coders, coder 2 waits ~200ms for dongle
# So burnout must be > compile + wait. 800ms should allow both to compile once.
# 410ms should NOT be enough for coder 2 (waits 200 + needs 200 to compile = 400+)
OUTPUT=$(timeout 10 $BIN 2 800 200 200 200 10 50 fifo 2>&1 | clean)
COMPILES=$(echo "$OUTPUT" | grep "is compiling" | wc -l)
[ "$COMPILES" -ge 10 ] && pass "Boundary burnout (800ms, 2 coders): survives to 10+ compiles" || fail "Boundary burnout: premature death" "Only $COMPILES compiles (expected ≥10)"

# 3.2 Burnout just barely too short
OUTPUT=$(timeout 2 $BIN 3 450 200 200 200 10 50 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && pass "Tight burnout (450ms, 3 coders): someone burns out" || fail "Should have burnout with 450ms deadline"

# 3.3 Burnout = compile time (impossible to survive after first)
OUTPUT=$(timeout 2 $BIN 2 200 200 200 200 10 50 fifo 2>&1 | clean)
BURNOUT_TIME=$(echo "$OUTPUT" | grep "burned out" | awk '{print $1}')
if [ -n "$BURNOUT_TIME" ]; then
  # One coder compiles, the other waits and burns out
  [ "$BURNOUT_TIME" -le 210 ] && pass "Burnout=compile time: burns at ${BURNOUT_TIME}ms" || fail "Burnout=compile: too late at ${BURNOUT_TIME}ms"
fi

# 3.4 Cooldown > burnout (guaranteed death)
OUTPUT=$(timeout 2 $BIN 2 300 100 100 100 10 500 fifo 2>&1 | clean)
echo "$OUTPUT" | grep -q "burned out" && pass "Cooldown(500) > burnout(300): guaranteed death" || fail "Should burn out when cooldown > burnout"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[4] OUTPUT INTEGRITY${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 4.1 No corrupted lines (partial writes, interleaving)
OUTPUT=$(timeout 5 $BIN 10 5000 20 20 20 5 5 fifo 2>&1 | clean)
CORRUPTED=$(echo "$OUTPUT" | grep -cvE '^[0-9]+ [0-9]+ (has taken a dongle|is compiling|is debugging|is refactoring|burned out)$')
[ "$CORRUPTED" -eq 0 ] && pass "10 coders, fast: no corrupted output lines" || fail "Corrupted output detected" "$CORRUPTED invalid lines"

# 4.2 No empty lines
EMPTY=$(echo "$OUTPUT" | grep -c '^$')
[ "$EMPTY" -eq 0 ] && pass "No empty lines in output" || fail "Empty lines found" "$EMPTY empty lines"

# 4.3 Coder IDs always in valid range
INVALID_IDS=$(echo "$OUTPUT" | awk '{if ($2 < 1 || $2 > 10) print}' | wc -l)
[ "$INVALID_IDS" -eq 0 ] && pass "All coder IDs in range [1,10]" || fail "Invalid coder IDs found" "$INVALID_IDS lines"

# 4.4 No negative timestamps
NEG_TS=$(echo "$OUTPUT" | awk '{if ($1 < 0) print}' | wc -l)
[ "$NEG_TS" -eq 0 ] && pass "No negative timestamps" || fail "Negative timestamps found"

# 4.5 Timestamps monotonically non-decreasing
DISORDER=$(echo "$OUTPUT" | awk 'NR>1 && $1 < prev {print NR": "$0} {prev=$1}' | wc -l)
[ "$DISORDER" -eq 0 ] && pass "Timestamps strictly non-decreasing" || fail "Timestamp ordering violated" "$DISORDER out-of-order lines"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[5] SEMANTIC CORRECTNESS UNDER STRESS${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 5.1 No coder compiles without 2 dongles (check per-coder sequence)
OUTPUT=$(timeout 5 $BIN 6 5000 30 30 30 4 10 fifo 2>&1 | clean)
SEMANTIC_FAIL=0
for CODER in $(seq 1 6); do
  ACTIONS=$(echo "$OUTPUT" | awk -v c="$CODER" '$2 == c {for(i=3;i<=NF;i++) printf "%s ", $i; print ""}')
  # Count: every "is compiling" must be preceded by exactly 2 "has taken a dongle"
  DONGLES=0
  while IFS= read -r line; do
    case "$line" in
      *"has taken a dongle"*) ((DONGLES++)) ;;
      *"is compiling"*)
        if [ "$DONGLES" -lt 2 ]; then ((SEMANTIC_FAIL++)); fi
        DONGLES=0 ;;
      *"is debugging"*|*"is refactoring"*) DONGLES=0 ;;
    esac
  done <<< "$(echo "$OUTPUT" | awk -v c="$CODER" '$2 == c')"
done
[ "$SEMANTIC_FAIL" -eq 0 ] && pass "6 coders: every compile preceded by 2 dongle takes" || fail "Compile without 2 dongles" "$SEMANTIC_FAIL violations"

# 5.2 No two adjacent coders compile simultaneously
OUTPUT=$(timeout 5 $BIN 4 5000 100 100 100 3 20 fifo 2>&1 | clean)
OVERLAP=0
for CODER in $(seq 1 4); do
  NEXT=$(( (CODER % 4) + 1 ))
  # Get compile start/end times for both
  while IFS= read -r line; do
    TS=$(echo "$line" | awk '{print $1}')
    # Coder is compiling from TS to TS+100
    while IFS= read -r line2; do
      TS2=$(echo "$line2" | awk '{print $1}')
      # Check overlap: [TS, TS+100] ∩ [TS2, TS2+100]
      if [ "$TS" -lt $((TS2 + 100)) ] && [ "$TS2" -lt $((TS + 100)) ]; then
        ((OVERLAP++))
      fi
    done <<< "$(echo "$OUTPUT" | awk -v c="$NEXT" '$2 == c && /is compiling/ {print}')"
  done <<< "$(echo "$OUTPUT" | awk -v c="$CODER" '$2 == c && /is compiling/ {print}')"
done
[ "$OVERLAP" -eq 0 ] && pass "No adjacent coders compile simultaneously" || fail "Adjacent coders overlap" "$OVERLAP overlaps (may share dongle)"

# 5.3 Only one "burned out" message ever
OUTPUT=$(timeout 2 $BIN 5 400 200 200 200 10 50 fifo 2>&1 | clean)
BURNOUTS=$(echo "$OUTPUT" | grep -c "burned out")
[ "$BURNOUTS" -le 1 ] && pass "At most 1 burnout message" || fail "Multiple burnout messages" "$BURNOUTS found"

# 5.4 After burnout, no other coder actions
BURNOUT_LINE=$(echo "$OUTPUT" | grep -n "burned out" | head -1 | cut -d: -f1)
if [ -n "$BURNOUT_LINE" ]; then
  TOTAL_LINES=$(echo "$OUTPUT" | wc -l)
  [ "$BURNOUT_LINE" -eq "$TOTAL_LINES" ] && pass "No actions after burnout" || fail "Actions continue after burnout" "Burnout at line $BURNOUT_LINE, total $TOTAL_LINES"
fi

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[6] MEMORY & SIGNAL STRESS${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 6.1 Segfault detection (run many times, check for signal 11)
SEGFAULTS=0
echo -n "  Running 100 iterations checking for segfaults... "
for i in $(seq 1 100); do
  timeout 2 $BIN 8 3000 20 20 20 5 5 edf > /dev/null 2>&1
  RET=$?
  [ "$RET" -eq 139 ] && ((SEGFAULTS++))  # 128+11=SIGSEGV
done
[ "$SEGFAULTS" -eq 0 ] && pass "100 runs: 0 segfaults" || fail "Segfaults detected" "$SEGFAULTS/100"

# 6.2 Double free detection (usually shows as signal 6)
ABORTS=0
echo -n "  Running 100 iterations checking for aborts... "
for i in $(seq 1 100); do
  timeout 2 $BIN 8 3000 20 20 20 5 5 fifo > /dev/null 2>&1
  RET=$?
  [ "$RET" -eq 134 ] && ((ABORTS++))  # 128+6=SIGABRT
done
[ "$ABORTS" -eq 0 ] && pass "100 runs: 0 aborts (no double-free)" || fail "Aborts detected" "$ABORTS/100"

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}[7] TIMING PRECISION UNDER LOAD${NC}"
# ─────────────────────────────────────────────────────────────────────────────

# 7.1 Burnout detection within 10ms even with many coders
PRECISION_FAILS=0
echo -n "  Checking burnout precision (10 runs, 10 coders)... "
for i in $(seq 1 10); do
  OUTPUT=$(timeout 3 $BIN 10 500 200 200 200 10 50 fifo 2>&1 | clean)
  BT=$(echo "$OUTPUT" | grep "burned out" | awk '{print $1}')
  if [ -n "$BT" ]; then
    DIFF=$((BT - 500))
    [ "$DIFF" -gt 10 ] && ((PRECISION_FAILS++))
  fi
done
[ "$PRECISION_FAILS" -eq 0 ] && pass "Burnout always within 10ms (10 coders)" || fail "Burnout precision exceeded 10ms" "$PRECISION_FAILS/10 runs"

# 7.2 Compile duration accuracy
OUTPUT=$(timeout 3 $BIN 2 5000 200 200 200 2 50 fifo 2>&1 | clean)
COMPILE_START=$(echo "$OUTPUT" | awk '/1 is compiling/ {print $1; exit}')
DEBUG_START=$(echo "$OUTPUT" | awk '/1 is debugging/ {print $1; exit}')
if [ -n "$COMPILE_START" ] && [ -n "$DEBUG_START" ]; then
  DURATION=$((DEBUG_START - COMPILE_START))
  DIFF=$((DURATION - 200))
  [ "$DIFF" -ge -5 ] && [ "$DIFF" -le 15 ] && pass "Compile duration: ${DURATION}ms (expected 200±15ms)" || fail "Compile duration off" "Got ${DURATION}ms, expected ~200ms"
fi

echo ""

# ─────────────────────────────────────────────────────────────────────────────
echo -e "${YELLOW}═══════════════════════════════════════════════════${NC}"
echo -e "  Results: ${GREEN}${PASS} passed${NC}, ${RED}${FAIL} failed${NC}, ${TOTAL} total"
echo -e "${YELLOW}═══════════════════════════════════════════════════${NC}"

exit $FAIL
