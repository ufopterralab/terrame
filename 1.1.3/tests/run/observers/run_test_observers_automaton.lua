dofile (TME_PATH.."/tests/run/run_util.lua")
local executeFileName = "TerraME ".. TME_PATH .."/tests/src/observers/test_observers_automaton.lua"

tests={
    [1] = 6,
    [2] = 7,
    [3] = 8,
    [4] = 19,
    [5] = 6,
    [6] = 5,
    [7] = 6
    --[8] = 8
}

executeObservers(executeFileName, tests)
