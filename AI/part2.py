import heapq, random, csv, time
from typing import Tuple, Dict, List

# --- Puzzle setup (4x4 grid for the 15-puzzle) ---
GRID = 4
N = GRID * GRID
GOAL = tuple(range(1, N)) + (0,)
POS: Dict[int, Tuple[int, int]] = {v: (i // GRID, i % GRID) for i, v in enumerate(GOAL)}

# Precompute valid blank-swap neighbors for each position
NEI: List[List[int]] = [[] for _ in range(N)]
for i in range(N):
    r, c = divmod(i, GRID)
    if r > 0: NEI[i].append(i - GRID)
    if r < GRID - 1: NEI[i].append(i + GRID)
    if c > 0: NEI[i].append(i - 1)
    if c < GRID - 1: NEI[i].append(i + 1)

# --- Heuristics ---
def h1(s: Tuple[int, ...]) -> int:
    # h1: number of misplaced tiles (ignoring the blank)
    return sum(1 for i, v in enumerate(s) if v and v != GOAL[i])

def h2(s: Tuple[int, ...]) -> int:
    # h2: sum of Manhattan distances between each tile and its goal position
    t = 0
    for i, v in enumerate(s):
        if v:
            r, c = divmod(i, GRID)
            gr, gc = POS[v]
            t += abs(r - gr) + abs(c - gc)
    return t

def h3(s: Tuple[int, ...]) -> int:
    # h3: linear-conflict heuristic = Manhattan + 2 × (# of linear conflicts)
    m = h2(s)
    c = 0
    # Row conflicts
    for r in range(GRID):
        row = [s[r * GRID + k] for k in range(GRID)]
        inrow = [t for t in row if t and POS[t][0] == r]
        gc = {t: POS[t][1] for t in inrow}
        for i in range(len(inrow)):
            for j in range(i + 1, len(inrow)):
                if gc[inrow[i]] > gc[inrow[j]]: c += 1
    # Column conflicts
    for col in range(GRID):
        colv = [s[r * GRID + col] for r in range(GRID)]
        incol = [t for t in colv if t and POS[t][1] == col]
        gr = {t: POS[t][0] for t in incol}
        for i in range(len(incol)):
            for j in range(i + 1, len(incol)):
                if gr[incol[i]] > gr[incol[j]]: c += 1
    return m + 2 * c

# --- Generate random reachable (solvable) states ---
def rand_state(lo=30, hi=80) -> Tuple[int, ...]:
    # Randomly move the blank 30–80 times from the goal; guarantees solvability
    s = list(GOAL)
    z = s.index(0)
    p = None
    for _ in range(random.randint(lo, hi)):
        opts = [n for n in NEI[z] if n != p] or NEI[z]
        n = random.choice(opts)
        s[z], s[n] = s[n], s[z]
        p, z = z, n
    return tuple(s)

# --- A* search ---
def astar(start: Tuple[int, ...], H) -> Tuple[int, int]:
    # Returns (solution_cost, nodes_expanded)
    if start == GOAL: return 0, 0
    opened = [(H(start), 0, start)]
    g = {start: 0}
    closed = set()
    exp = 0
    while opened:
        f, dist, s = heapq.heappop(opened)
        if s in closed: continue
        if s == GOAL: return dist, exp
        closed.add(s); exp += 1
        z = s.index(0)
        for nz in NEI[z]:
            lst = list(s); lst[z], lst[nz] = lst[nz], lst[z]; nxt = tuple(lst)
            nd = dist + 1
            if nxt in closed: continue
            if nd < g.get(nxt, 10**9):
                g[nxt] = nd
                heapq.heappush(opened, (nd + H(nxt), nd, nxt))
    return -1, exp

# --- Run 100 test cases, write CSV, and print averages ---
if __name__ == "__main__":
    random.seed(7)
    K = 1
    HS = [("h1", h1), ("h2", h2), ("h3", h3)]
    seen = set()
    states: List[Tuple[int, ...]] = []
    while len(states) < K:
        s = rand_state()
        if s not in seen:
            seen.add(s)
            states.append(s)

    rows = []
    for sid, s in enumerate(states, 1):
        for name, H in HS:
            print(f"[{sid}/{K}] {name}: solving...", flush=True)
            t0 = time.time()
            cost, exp = astar(s, H)
            dt = time.time() - t0
            print(f"[{sid}/{K}] {name}: done in {dt:.2f}s (cost={cost}, expanded={exp})", flush=True)
            rows.append({"state_id": sid, "heuristic": name, "cost": cost, "expanded": exp})

    with open("results_15p.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=["state_id", "heuristic", "cost", "expanded"])
        w.writeheader(); w.writerows(rows)

    # Compute and print averages for comparison
    agg = {name: {"c": 0, "e": 0, "n": 0} for name, _ in HS}
    for r in rows:
        if r["cost"] >= 0:
            a = agg[r["heuristic"]]
            a["c"] += r["cost"]; a["e"] += r["expanded"]; a["n"] += 1
    for name in [n for n, _ in HS]:
        a = agg[name]
        print(name, "avg_cost=%.2f" % (a["c"]/a["n"]), "avg_expanded=%.0f" % (a["e"]/a["n"]))
