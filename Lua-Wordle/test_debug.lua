local ffi = require("ffi")

-- Mock love for the module
love = {
    filesystem = {
        getSource = function() return "/Users/macbookair/Programming/Lua/LuaWordle" end
    }
}

local m = require("wordle_core")
local count = m.LoadWords("allowed_words.txt", "possible_words.txt")

print("\n--- STANDALONE LOAD TEST ---")
print("Allowed Count: " .. count)

-- Find salet and roate indices
local salet_idx = -1
local roate_idx = -1

for i = 0, count - 1 do
    local w = m.GetWord(i)
    if w == "salet" then salet_idx = i end
    if w == "roate" then roate_idx = i end
end

if salet_idx >= 0 then
    print(string.format("salet Score: %d (Possible: %s)", m.GetWordScore(salet_idx), tostring(m.IsPossible(salet_idx))))
else
    print("salet NOT FOUND")
end

if roate_idx >= 0 then
    print(string.format("roate Score: %d (Possible: %s)", m.GetWordScore(roate_idx), tostring(m.IsPossible(roate_idx))))
else
    print("roate NOT FOUND")
end

print("\nCalculating First Best Move...")
local start = os.clock()
local b_idx, b_score = m.CalculateBestMove()
local end_t = os.clock()

print("Best Move: " .. m.GetWord(b_idx))
print("Best Score: " .. b_score)
print("Time Taken: " .. string.format("%.3fs", end_t - start))
