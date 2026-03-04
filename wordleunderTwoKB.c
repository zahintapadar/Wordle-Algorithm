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

int main() {
  for (int i = 1; i < 13000; i++) {
    int l = 31 - __builtin_clz(i);
    L[i] = i * (l * 256 + ((i - (1 << l)) * 256) / (1 << l));
  }
  FILE *f = fopen("allowed_words.txt", "r");
  while (fscanf(f, "%5s", w[N]) == 1)
    N++;
  fclose(f);
  f = fopen("possible_words.txt", "r");
  char t[6];
  while (fscanf(f, "%5s", t) == 1)
    for (int i = 0; i < N; i++)
      if (!strcmp(w[i], t)) {
        v[i] = q[i] = 1;
        break;
      }
  fclose(f);

  char guess[6] = "salet";
  while (1) {
    printf("\nGuess: %s\nFeedback (0=gray,1=yel,2=grn): ", guess);
    char feed[6];
    if (scanf("%5s", feed) != 1)
      break;
    int target = 0, m[5] = {1, 3, 9, 27, 81};
    for (int i = 0; i < 5; i++)
      target += (feed[i] - '0') * m[i];
    if (target == 242) {
      printf("Solved!\n");
      break;
    }

    int act = 0;
    for (int i = 0; i < N; i++)
      if (v[i]) {
        if (cmp(guess, w[i]) != target)
          v[i] = 0;
        else
          act++;
      }
    printf("Remaining: %d\n", act);
    if (act == 1) {
      for (int i = 0; i < N; i++)
        if (v[i])
          strcpy(guess, w[i]);
      continue;
    }

    int b_score = 1e9, b_idx = 0;
    for (int i = 0; i < N; i++) {
      int c[243] = {0}, score = 0;
      for (int j = 0; j < N; j++)
        if (v[j])
          c[cmp(w[i], w[j])]++;
      for (int k = 0; k < 243; k++)
        if (c[k])
          score += L[c[k]];
      score -= (v[i] ? 1024 : 0) + (q[i] ? 256 : 0); // Priority
      if (score < b_score) {
        b_score = score;
        b_idx = i;
      }
    }
    strcpy(guess, w[b_idx]);
  }
  return 0;
}
