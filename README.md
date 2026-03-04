# Wordle Algorithm Solver

A high-performance Wordle solver implemented in C, based on the **3Blue1Brown Information Theory** (Shannon Entropy) framework.

## 🚀 Features
- **Information-Theoretic Engine**: Uses Shannon Entropy $H(X) = -\sum p(x) \log_2 p(x)$ to find the mathematically optimal guess.
- **Code Golfed**: The core solver (`wordleunderTwoKB.c`) is designed for an extremely small binary footprint (< 4KB compiled) and minimal source size (~2KB).
- **Fixed-Point Math**: Replaces floating-point entropy with integer-scaled fixed-point arithmetic using `__builtin_clz` for speed and size.
- **Dynamic Pruning**: Efficiently narrows down the list of ~13,000 allowed words based on user feedback.


https://github.com/user-attachments/assets/a6535eab-90c0-4369-9067-c63674188ad5


## 🛠 Compilation & Usage

### Prerequisites
- A C compiler (e.g., `clang` or `gcc`).

### Build
To compile the highly optimized version:
```bash
clang -Os wordleunderTwoKB.c -o wordle_solver
strip wordle_solver  # Further reduces binary size
```

### Run
```bash
./wordle_solver
```

## 🧠 How it Works
1. **Entropy Calculation**: For every possible guess, the solver evaluates the expected information gain across all remaining possible answers.
2. **Frequency Prior**: It incorporates a simulated frequency prior (3b1b sigmoid style) to prioritize common English words when entropy scores are tied.
3. **Feedback Patterns**: Feedback is entered as a 5-digit string:
   - `0` (or `B`): Gray (Wrong)
   - `1` (or `Y`): Yellow (Wrong Position)
   - `2` (or `G`): Green (Correct)

Example: If your guess was `SALET` and the feedback was `01002`, you enter `01002`.

## 📂 Project Structure
- `wordleunderTwoKB.c`: The primary code-golfed C solver.
- `allowed_words.txt`: The list of ~12,972 valid Wordle guesses.
- `possible_words.txt`: The list of ~2,315 actual potential Wordle answers.

## ⚖️ License
MIT
