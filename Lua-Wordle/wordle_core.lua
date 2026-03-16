local ffi = require("ffi")

ffi.cdef[[
    int load_words(const char *allowed_path, const char *possible_path);
    int filter_possible(int guess_idx, int target);
    int calculate_best_move(int *out_score);
    int get_word_score(int idx);
    const char* get_word(int idx);
    int get_is_possible(int idx);
    int get_is_answer(int idx);
]]

-- Load the compiled C library dynamically
-- Since we execute from the `/LuaWorlde` dir as `love .`, local paths apply
local lib
if ffi.os == "Windows" then
    lib = ffi.load("wordle_core.dll")
elseif ffi.os == "OSX" then
    lib = ffi.load(love.filesystem.getSource() .. "/wordle_core.dylib")
else
    lib = ffi.load(love.filesystem.getSource() .. "/wordle_core.so")
end

local M = {}

M.allowed_count = 0

function M.LoadWords(allowed_path, possible_path)
    -- Needs absolute or relative lookup depending on invocation
    local base = love.filesystem.getSource() .. "/"
    local p1 = base .. allowed_path
    local p2 = base .. possible_path
    print("Lua Info: Calling LoadWords with paths:")
    print("  -> " .. p1)
    print("  -> " .. p2)
    M.allowed_count = lib.load_words(p1, p2)
    return M.allowed_count
end

function M.FilterPossible(guess_idx, pattern)
    return lib.filter_possible(guess_idx, pattern)
end

function M.CalculateBestMove()
    local out_score = ffi.new("int[1]")
    local b_idx = lib.calculate_best_move(out_score)
    return b_idx, out_score[0]
end

function M.GetWordScore(idx)
    return lib.get_word_score(idx)
end

function M.GetWord(idx)
    return ffi.string(lib.get_word(idx))
end

function M.IsPossible(idx)
    return lib.get_is_possible(idx) == 1
end

function M.IsAnswer(idx)
    return lib.get_is_answer(idx) == 1
end

function M.FeedbackToPattern(feedback_str)
    local target = 0
    local m_array = {1, 3, 9, 27, 81}
    for i = 1, 5 do
        local c = tonumber(feedback_str:sub(i, i)) or 0
        target = target + c * m_array[i]
    end
    return target
end

return M
