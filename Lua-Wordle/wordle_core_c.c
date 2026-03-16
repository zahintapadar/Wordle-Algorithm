#include <stdio.h>
#include <string.h>

char w[13000][6], v[13000], q[13000];
int N = 0, L[13000];

int cmp(char *g, char *a) {
  int p = 0, au = 0, gu = 0, m[5] = {1, 3, 9, 27, 81};
  for (int i = 0; i < 5; ++i)
    if (g[i] == a[i])
      p += 2 * m[i], au |= 1 << i, gu |= 1 << i;
  for (int i = 0; i < 5; ++i)
    if (!(gu & (1 << i)))
      for (int j = 0; j < 5; ++j)
        if (!(au & (1 << j)) && g[i] == a[j]) {
          p += m[i];
          au |= 1 << j;
          break;
        }
  return p;
}

void init_tables() {
  for (int i = 1; i < 13000; i++) {
    int l = 31 - __builtin_clz(i);
    L[i] = i * (l * 256 + ((i - (1 << l)) * 256) / (1 << l));
  }
}

int load_words(const char *allowed_path, const char *possible_path) {
  init_tables();
  N = 0;
  memset(v, 0, sizeof(v));
  memset(q, 0, sizeof(q));
  
  FILE *f = fopen(allowed_path, "r");
  if (!f) {
    printf("C Error: Could not open allowed_words at %s\n", allowed_path);
    return 0;
  }
  char word_buf[16];
  while (fscanf(f, "%15s", word_buf) == 1) {
    // Basic trimming and length safety
    word_buf[5] = '\0';
    strcpy(w[N], word_buf);
    N++;
  }
  fclose(f);
  printf("C Info: Loaded %d allowed words\n", N);
  
  f = fopen(possible_path, "r");
  if (!f) {
    printf("C Error: Could not open possible_words at %s\n", possible_path);
    return 0;
  }
  int p_count = 0;
  while (fscanf(f, "%15s", word_buf) == 1) {
    word_buf[5] = '\0';
    for (int i = 0; i < N; i++) {
      if (!strcmp(w[i], word_buf)) {
        v[i] = q[i] = 1;
        p_count++;
        break;
      }
    }
  }
  fclose(f);
  printf("C Info: Loaded %d possible words mapped to allowed list\n", p_count);
  return N;
}

int filter_possible(int guess_idx, int target) {
  int act = 0;
  for (int i = 0; i < N; i++) {
    if (v[i]) {
      if (cmp(w[guess_idx], w[i]) != target)
        v[i] = 0;
      else
        act++;
    }
  }
  return act;
}

int calculate_best_move(int *out_score) {
  int b_score = 2e9, b_idx = 0;
  for (int i = 0; i < N; i++) {
    int c[243] = {0}, score = 0;
    for (int j = 0; j < N; j++) {
      if (v[j]) {
        c[cmp(w[i], w[j])]++;
      }
    }
    for (int k = 0; k < 243; k++) {
      if (c[k]) {
        score += L[c[k]];
      }
    }
    // Priority modifiers: heavily favor words that are potential answers
    // Entropy is ~3,000,000. We use a 200k/50k bonus to ensure it shifts the top tiers.
    score -= (v[i] ? 200000 : 0) + (q[i] ? 50000 : 0);
    
    if (score < b_score) {
      b_score = score;
      b_idx = i;
    }
  }
  *out_score = b_score;
  return b_idx;
}

int get_word_score(int idx) {
    int c[243] = {0}, score = 0;
    for (int j = 0; j < N; j++) {
      if (v[j]) {
        c[cmp(w[idx], w[j])]++;
      }
    }
    for (int k = 0; k < 243; k++) {
      if (c[k]) {
        score += L[c[k]];
      }
    }
    score -= (v[idx] ? 200000 : 0) + (q[idx] ? 50000 : 0);
    return score;
}

const char* get_word(int idx) {
  return w[idx];
}

int get_is_possible(int idx) {
  return v[idx];
}

int get_is_answer(int idx) {
    return q[idx];
}
