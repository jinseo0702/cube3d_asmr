#!/bin/sh
# Test suite for cub3D: parser validation + headless render smoke tests.
# Run from the repository root: sh tests/run_tests.sh (or make test)

BIN=./cub3D
TMP=$(mktemp -d)
PASS=0
FAIL=0

if [ ! -x "$BIN" ]; then
	echo "error: $BIN not found; run make first" >&2
	exit 1
fi

expect_ok() {
	desc=$1; shift
	if "$@" >"$TMP/out" 2>"$TMP/err"; then
		PASS=$((PASS + 1)); echo "PASS  $desc"
	else
		FAIL=$((FAIL + 1)); echo "FAIL  $desc"; sed 's/^/      /' "$TMP/err"
	fi
}

expect_error() {
	desc=$1; shift
	if "$@" >"$TMP/out" 2>"$TMP/err"; then
		FAIL=$((FAIL + 1)); echo "FAIL  $desc (expected failure, got success)"
	elif ! grep -q '^Error$' "$TMP/err"; then
		FAIL=$((FAIL + 1)); echo "FAIL  $desc (no 'Error' banner on stderr)"
	else
		PASS=$((PASS + 1)); echo "PASS  $desc"
	fi
}

echo "== rejected scenes =="
expect_error "wrong file extension"     "$BIN" maps/invalid/wrong_ext.txt
expect_error "nonexistent file"         "$BIN" maps/invalid/does_not_exist.cub
expect_error "map not closed"           "$BIN" maps/invalid/open_map.cub --screenshot "$TMP/x.ppm"
expect_error "two players"              "$BIN" maps/invalid/two_players.cub --screenshot "$TMP/x.ppm"
expect_error "no player"                "$BIN" maps/invalid/no_player.cub --screenshot "$TMP/x.ppm"
expect_error "invalid map character"    "$BIN" maps/invalid/bad_char.cub --screenshot "$TMP/x.ppm"
expect_error "blank line inside map"    "$BIN" maps/invalid/blank_line_in_map.cub --screenshot "$TMP/x.ppm"
expect_error "missing config element"   "$BIN" maps/invalid/missing_element.cub --screenshot "$TMP/x.ppm"
expect_error "color channel out of range" "$BIN" maps/invalid/bad_color.cub --screenshot "$TMP/x.ppm"

echo "== headless rendering =="
expect_ok "demo map renders"  "$BIN" maps/demo.cub --screenshot "$TMP/demo.ppm"
expect_ok "small map renders" "$BIN" maps/small.cub --screenshot "$TMP/small.ppm"

for shot in demo small; do
	if [ -f "$TMP/$shot.ppm" ] && head -c 2 "$TMP/$shot.ppm" | grep -q P6; then
		PASS=$((PASS + 1)); echo "PASS  $shot.ppm is a valid P6 image"
	else
		FAIL=$((FAIL + 1)); echo "FAIL  $shot.ppm is not a valid P6 image"
	fi
done

echo "== windowed smoke test (dummy video driver) =="
if SDL_VIDEODRIVER=dummy "$BIN" maps/demo.cub --frames 30 >"$TMP/out" 2>"$TMP/err"; then
	PASS=$((PASS + 1)); echo "PASS  30 frames on dummy driver"
else
	echo "SKIP  dummy driver unavailable on this backend"
fi

rm -rf "$TMP"
echo ""
echo "$PASS passed, $FAIL failed"
[ "$FAIL" -eq 0 ]
