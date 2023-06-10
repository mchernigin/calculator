import argparse
import random
import subprocess
import sys

import matplotlib.pyplot as plt
import seaborn as sns


BIN = "./src/calc"


def gen_num(boundary, use_float, precision):
    if not use_float:
        return random.randint(1, boundary)
    return max(round(random.random() * boundary, precision), 1)


def gen_exp_easy(numberop):
    return "1+1*1" * (numberop // 2) + ("+1" * (numberop % 2))


def gen_exp_rand(numberop, boundary=10, use_float=True, precision=3):
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


def draw_progress(count, total, bar_len=20):
    filled_len = round(bar_len * count / float(total))
    percents = int(round(100.0 * count / float(total), 1))
    bar = "#" * filled_len + "." * (bar_len - filled_len)
    sys.stdout.write(f"[{bar}] {percents}%\r")
    sys.stdout.flush()


def benchmark(cfg, mode, sizes):
    time = []
    for i, numberop in enumerate(sizes):
        if cfg.random:
            exp = gen_exp_rand(numberop)
        else:
            exp = gen_exp_easy(numberop)
        cmd = [BIN, "-t", "-p", mode, "-n", str(cfg.numcalc), exp]
        result = subprocess.run(cmd, capture_output=True)
        time.append(float(result.stderr.decode().strip()))
        print(f"Running {mode} version".ljust(25), end="")
        draw_progress(i, len(sizes) - 1)
    print()
    return time


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-r", "--random", help="random exressions", nargs="?", type=bool,
        const=True, default=False,
    )
    parser.add_argument(
        "-s", "--seed", help="random seed", type=int, default=0
    )
    parser.add_argument(
        "-n", "--numcalc", help="number of calculations", type=int,
        default=50_000
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

    sizes = [x for x in range(cfg.min, cfg.max + 1, (cfg.max - cfg.min) // 25)]

    random.seed(cfg.seed)
    basic_time = benchmark(cfg, "basic", sizes)
    ast_rec_time = benchmark(cfg, "ast_rec", sizes)
    ast_iter_time = benchmark(cfg, "ast_iter", sizes)

    plt.plot(sizes, basic_time)
    plt.plot(sizes, ast_rec_time)
    plt.plot(sizes, ast_iter_time)
    ax = sns.lineplot()

    ax.set_xlabel("Number of operators")
    ax.set_ylabel("Time")
    plt.legend(labels=["Basic parser", "AST rec parser", "AST iter parser"])

    plt.yscale("log")

    ax.grid(visible=True, color="#DDDDDD")
    plt.savefig(cfg.output, dpi=600)
    print(f"Result has been saved into {cfg.output!r}")


if __name__ == "__main__":
    main()

