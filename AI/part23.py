import heapq, random, time, csv
from typing import Tuple, List, Dict, Callable, Optional

GRID_SIZE = 4
N = GRID_SIZE * GRID_SIZE
GOAL_STATE: Tuple[int, ...] = tuple(range(1, N)) + (0,)
GOAL_POS: Dict[int, Tuple[int, int]] = {num: divmod(i, GRID_SIZE) for i, num in enumerate(GOAL_STATE)}

def build_neighbors(grid_size: int = GRID_SIZE) -> List[List[int]]:
    nbrs = [[] for _ in range(grid_size * grid_size)]
    for i in range(grid_size * grid_size):
        r, c = divmod(i, grid_size)
        if r > 0: nbrs[i].append(i - grid_size)
        if r < grid_size - 1: nbrs[i].append(i + grid_size)
        if c > 0: nbrs[i].append(i - 1)
        if c < grid_size - 1: nbrs[i].append(i + 1)
    return nbrs

NEIGHBORS = build_neighbors(GRID_SIZE)

def generate_random_solvable_state(min_moves: int = 30, max_moves: int = 80, seed: Optional[int] = None) -> Tuple[int, ...]:
    if seed is not None: random.seed(seed)
    s = list(GOAL_STATE); z = s.index(0); p = None
    for _ in range(random.randint(min_moves, max_moves)):
        choices = [n for n in NEIGHBORS[z] if n != p] or NEIGHBORS[z]
        nz = random.choice(choices)
        s[z], s[nz] = s[nz], s[z]
        p, z = z, nz
    return tuple(s)

def h1(state: Tuple[int, ...]) -> int:
    return sum(1 for i, v in enumerate(state) if v and v != GOAL_STATE[i])

def h2(state: Tuple[int, ...]) -> int:
    t = 0
    for i, v in enumerate(state):
        if v:
            r, c = divmod(i, GRID_SIZE)
            gr, gc = GOAL_POS[v]
            t += abs(r - gr) + abs(c - gc)
    return t

def h3(state: Tuple[int, ...]) -> int:
    m = h2(state); conf = 0
    for r in range(GRID_SIZE):
        row = [state[r*GRID_SIZE+c] for c in range(GRID_SIZE)]
        inrow = [t for t in row if t and GOAL_POS[t][0] == r]
        gc = {t: GOAL_POS[t][1] for t in inrow}
        for i in range(len(inrow)):
            for j in range(i+1, len(inrow)):
                if gc[inrow[i]] > gc[inrow[j]]: conf += 1
    for c in range(GRID_SIZE):
        col = [state[r*GRID_SIZE+c] for r in range(GRID_SIZE)]
        incol = [t for t in col if t and GOAL_POS[t][1] == c]
        gr = {t: GOAL_POS[t][0] for t in incol}
        for i in range(len(incol)):
            for j in range(i+1, len(incol)):
                if gr[incol[i]] > gr[incol[j]]: conf += 1
    return m + 2*conf

def a_star(start: Tuple[int, ...], heuristic: Callable[[Tuple[int, ...]], int]) -> Tuple[int, int, float]:
    open_set = [(heuristic(start), 0, start)]
    g_cost = {start: 0}
    parent = {start: None}
    closed = set()
    expanded = 0
    t0 = time.time()
    while open_set:
        _, g, s = heapq.heappop(open_set)
        if s in closed: continue
        closed.add(s); expanded += 1
        if s == GOAL_STATE:
            steps = 0; cur = s
            while parent[cur] is not None:
                steps += 1; cur = parent[cur]
            return steps, expanded, time.time() - t0
        z = s.index(0)
        for nz in NEIGHBORS[z]:
            lst = list(s); lst[z], lst[nz] = lst[nz], lst[z]; ns = tuple(lst)
            ng = g + 1
            if ng < g_cost.get(ns, 10**9):
                g_cost[ns] = ng
                parent[ns] = s
                heapq.heappush(open_set, (ng + heuristic(ns), ng, ns))
    return -1, expanded, time.time() - t0

def run_experiment(num_states=100, seed=7, csv_path="results_15p.csv"):
    random.seed(seed)
    heuristics = [("h1", h1), ("h2", h2), ("h3", h3)]

    # Accumulators: name -> [steps_sum, nodes_sum, time_sum]
    results: Dict[str, List[float]] = {name: [0.0, 0.0, 0.0] for name, _ in heuristics}

    for _ in range(num_states):
        s = generate_random_solvable_state(min_moves=5, max_moves=10)
        for name, H in heuristics:
            t0 = time.time()
            steps, expanded, _ = a_star(s, H)
            dt = time.time() - t0
            if steps != -1:
                res = results[name]
                res[0] += steps
                res[1] += expanded
                res[2] += dt

    # Optional: write CSV of per-run data if needed later
    with open(csv_path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["heuristic","avg_steps","avg_expanded","avg_time"])
        for name, _ in heuristics:
            avg_steps = results[name][0] / num_states
            avg_exp = results[name][1] / num_states
            avg_time = results[name][2] / num_states
            w.writerow([name, f"{avg_steps:.2f}", f"{avg_exp:.1f}", f"{avg_time:.4f}"])

    print("15-Puzzle A* Results")
    print("Heuristic | Steps | Nodes | Time")
    print("-" * 35)
    for name, _ in heuristics:
        avg_steps = results[name][0] / num_states
        avg_nodes = results[name][1] / num_states
        avg_time = results[name][2] / num_states
        print(f"{name:<9}| {avg_steps:5.1f} | {avg_nodes:6.1f} | {avg_time:.4f}")

if __name__ == "__main__":
    run_experiment()
