import argparse
import random
import subprocess
import sys

import matplotlib.pyplot as plt
import seaborn as sns


CALC_BIN = "./src/calc"
MODES = ["basic", "ast_rec", "ast_iter"]
LABELS = ["Basic", "AST recursive", "AST iterative"]
LINES_STYLES = ["solid", "dashed", "dashdot"]


def gen_num(boundary: int, use_float: bool, precision: int) -> int | float:
    if not use_float:
        return random.randint(1, boundary)
    return max(round(random.random() * boundary, precision), 1)


def gen_exp_easy(numberop: int) -> str:
    return "1+1*1" * (numberop // 2) + ("+1" * (numberop % 2))


def gen_exp_rand(
    numberop: int, boundary: int = 10, use_float: bool = True, precision: int = 3
) -> str:
    exp = "("
    parant_count = 0
    for _ in range(numberop):
        exp += str(gen_num(boundary, use_float, precision))
        while parant_count > 0 and random.random() < 0.15:
            exp += ")"
            parant_count -= 1
        exp += random.choice(["+", "*", "/"])
        while random.random() < 0.1:
            exp += "("
            parant_count += 1
    exp += str(gen_num(boundary, use_float, precision))
    exp += ")" * (parant_count + 1)
    return exp


def draw_progress(count: int, total: int, bar_len: int = 20) -> None:
    filled_len = round(bar_len * count / float(total))
    percents = int(round(100.0 * count / float(total), 1))
    bar = "#" * filled_len + "." * (bar_len - filled_len)
    sys.stdout.write(f"[{bar}] {percents}%\r")
    sys.stdout.flush()


def benchmark(cfg: argparse.Namespace, mode: str, sizes: list[int]) -> list[float]:
    time: list[float] = []
    for i, numberop in enumerate(sizes):
        if cfg.random:
            exp = gen_exp_rand(numberop)
        else:
            exp = gen_exp_easy(numberop)
        cmd = [CALC_BIN, "-t", "-p", mode, "-n", str(cfg.numcalc), exp]
        result = subprocess.run(cmd, capture_output=True)
        time.append(float(result.stderr.decode().strip()))
        print(f"Running {mode} version".ljust(25), end="")
        draw_progress(i, len(sizes) - 1)
    print()
    return time


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-r",
        "--random",
        help="random exressions",
        nargs="?",
        type=bool,
        const=True,
        default=False,
    )
    parser.add_argument(
        "-l",
        "--log-scale",
        help="use log scale",
        nargs="?",
        type=bool,
        const=True,
        default=False,
    )
    parser.add_argument("-s", "--seed", help="random seed", type=int, default=0)
    parser.add_argument(
        "-n", "--numcalc", help="number of calculations", type=int, default=50_000
    )
    parser.add_argument(
        "-m", "--min", help="minimal expression length", type=int, default=10
    )
    parser.add_argument(
        "-M", "--max", help="maximum expression length", type=int, default=250
    )
    parser.add_argument(
        "-o", "--output", help="output file", type=str, default="graph.png"
    )
    cfg = parser.parse_args()
    random.seed(cfg.seed)

    sizes = [x for x in range(cfg.min, cfg.max + 1, 10)]
    times = [benchmark(cfg, mode, sizes) for mode in MODES]

    ax = sns.lineplot()
    for mode_time, style in zip(times, LINES_STYLES):
        plt.plot(sizes, mode_time)
        ax.lines[-1].set_linestyle(style)

    if cfg.log_scale:
        plt.yscale("log")

    ax.set_xlabel("Number of operators")
    plt.legend(labels=LABELS)
    ax.set_ylabel("Time")
    ax.grid(visible=True, color="#DDDDDD")
    plt.savefig(cfg.output, dpi=600)
    print(f"Result has been saved into {cfg.output!r}")


if __name__ == "__main__":
    main()
