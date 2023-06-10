import subprocess
import pytest


CALC_BIN = './src/calc'
MODES = ['basic', 'ast_rec', 'ast_iter']


def run_calc(params):
    res = subprocess.run([CALC_BIN] + params, stdout=subprocess.PIPE)
    assert res.returncode == 0
    return res.stdout.decode().strip()


@pytest.mark.parametrize('mode', MODES)
def test_simple(mode):
    expr = '1+2-3*4/5+(7-8)*(9/10)'
    res = run_calc(["-p", mode, expr])
    assert res == '-0.3'

