local ffi = require("ffi")
local m = require("wordle_core")
m.LoadWords("allowed_words.txt", "possible_words.txt")

-- Find indices
local g_idx, a_idx
for i = 0, m.allowed_count - 1 do
    local w = ffi.string(m.allowed_words[i], 5)
    if w == "salet" then g_idx = i end
    if w == "apple" then a_idx = i end
end

print("Comparing 'salet' against 'apple': " .. m.Compare(g_idx, a_idx))
