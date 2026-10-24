/* As Quintuplas Essenciais - Nintendo DS (libnds + libfat)
   Texto sem acentos: a fonte padrao do DS nao tem caracteres acentuados. */
#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static PrintConsole top, bot;
static char pname[9] = "Aluno";
static u32 unlocked = 0;
static int pts[5];
static u32 tick = 0;
static char lst[64];

#define VB() do { tick++; swiWaitForVBlank(); } while (0)

static const char *SN[5] = {"Ichika", "Nino", "Miku", "Yotsuba", "Itsuki"};
static const int W[7] = {2, 3, 3, 3, 3, 4, 5};

typedef struct { const char *title, *text, *o[5][2]; } Scene;
static const Scene S[7] = {
 {"O primeiro dia",
  "Voce, {n}, acaba de ser contratado como tutor das cinco irmas. Elas estao todas na sala. O silencio e pesado.\n\nComo voce comeca?",
  {{"Sustento o olhar de quem parece estar me avaliando", "Retribuo o sorriso de quem parece saber mais do que diz"},
   {"Ignoro a cara fechada e entro direto no assunto", "Aceito o primeiro comentario atravessado sem recuar"},
   {"Deixo o silencio durar um pouco mais", "Pergunto o que cada uma costuma ler ou ouvir"},
   {"Ja peco ajuda para arrumar as carteiras", "Pergunto quem quer comecar pelo mais facil"},
   {"Mostro a pauta da aula, topico por topico", "Combino um horario fixo e falo que vou levar a serio"}}},
 {"Hora de estudar",
  "A materia esta dificil. Voce percebe que cada uma lida com a frustracao de um jeito diferente.",
  {{"Respondo a duvida com outra pergunta, so para ver o que ela pensa", "Finjo nao ver uma cola discreta para aliviar a tensao"},
   {"Corrijo o erro sem rodeios, mesmo que doa", "Digo que ficar no meio-termo nao ajuda ninguem"},
   {"Sento mais perto e abaixo o tom de voz", "Ligo o assunto a algo que ela gosta"},
   {"Transformo a pergunta numa competicao boba", "Fico treinando o exercicio ate sair certo"},
   {"Repito a explicacao ate ela dizer que entendeu", "Monto um cronograma para o resto da semana"}}},
 {"Intervalo",
  "Ha um prato sobre a mesa e ninguem assume de quem e. Alguem comenta, num tom meio rispido, que voce pode se servir.",
  {{"Elogio o tempero e finjo nao saber quem fez", "Deixo o ultimo pedaco para quem ainda nao comeu"},
   {"Digo que esta bom e peco a receita", "Sugiro um ajuste e aceito a bronca"},
   {"Provo em silencio e agradeco so no fim do dia", "Pergunto a historia por tras da receita"},
   {"Ofereco lavar a louca depois", "Divido tudo em porcoes iguais"},
   {"Aceito a primeira porcao sem cerimonia", "Peco para guardar um pouco para mais tarde"}}},
 {"Preparativos do festival",
  "O festival da escola esta proximo. Varias tarefas precisam ser feitas ao mesmo tempo.",
  {{"Vou ver o ensaio e dou uma opiniao sincera", "Assumo o papel que ninguem quis"},
   {"Cuido do cardapio do estande", "Escolho tecidos e cores junto com ela"},
   {"Ajudo a montar a exposicao de livros", "Fico na parte quieta, organizando papeis"},
   {"Pego tudo o que for pesado de carregar", "Corro de um lado para o outro resolvendo imprevistos"},
   {"Faco a lista de tarefas e distribuo", "Fico responsavel pelo orcamento e pelas filas"}}},
 {"Chuva",
  "Na volta para casa comeca a chover forte. So existe um guarda-chuva disponivel.",
  {{"Abro o guarda-chuva e finjo que era so para mim", "Seguro o guarda-chuva e deixo ela escolher o caminho"},
   {"Empurro o guarda-chuva para o lado dela sem dizer nada", "Digo que nao preciso e estendo mesmo assim"},
   {"Espero a chuva diminuir debaixo de um toldo", "Divido o guarda-chuva e caminho devagar"},
   {"Corro na frente e rio da chuva", "Troco de casaco com ela para que nao molhe"},
   {"Calculo o caminho mais curto ate a estacao", "Peco desculpa por nao ter olhado a previsao"}}},
 {"Noite de Natal",
  "Voces se reuniram para passar o Natal. Voce so tem um presente e muitas pessoas a quem poderia entrega-lo.",
  {{"Uma entrada para uma peca de teatro", "Um diario com cadeado"},
   {"Um avental bonito", "Um livro de receitas usado"},
   {"Um livro antigo de historia", "Um fone de ouvido novo"},
   {"Um amuleto da sorte", "Um kit de primeiros socorros"},
   {"Um vale-refeicao", "Um caderno de exercicios caprichado"}}},
 {"O momento",
  "O tempo passou. As notas subiram. Agora existe algo no ar que nao da mais para ignorar.\n\nVoce decide falar o que sente. Como?",
  {{"Digo com um sorriso que ela ja sabe a resposta", "Escrevo uma carta e deixo no camarim"},
   {"Aceito uma discussao antes, porque sei que ela precisa", "Espero ela me chamar de idiota para entao falar"},
   {"Falo baixinho, onde ninguem ouve", "Entrego um marcador de livro com um recado"},
   {"Falo correndo, antes de perder a coragem", "Chamo para um passeio e falo no caminho"},
   {"Digo com calma, olhando nos olhos", "Falo de futuro e de planos, sem enrolar"}}}
};

typedef struct { const char *title, *ach, *text; } Ending;
static const int PAIRS[10][2] = {{0,1},{0,2},{0,3},{0,4},{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}};
#define NEND 18
static const Ending E[NEND] = {
 {"Final com Ichika", "Atriz Principal", "{n}, voce escolheu Ichika.\n\nEla te puxa pela mao para longe das irmas. O sorriso dela, dessa vez, nao e o de atriz.\n\n\"Eu esperava voce perceber.\"\n\nVoces se casam. O papel favorito dela agora e o de sua esposa."},
 {"Final com Nino", "Tsundere Conquistada", "{n}, voce conquistou Nino.\n\nEla te empurra de leve e depois te abraca forte, escondendo o rosto.\n\n\"Idiota... eu te amo desde o comeco.\"\n\nVoces abrem uma confeitaria. Ela ainda xinga, mas o olhar nunca mais esconde nada."},
 {"Final com Miku", "A Quietude que Falou", "{n}, voce escolheu Miku.\n\nVermelha ate as orelhas, ela murmura:\n\n\"Eu... tambem gosto de voce.\"\n\nVoces se casam numa cerimonia simples. Toda noite ela conta historias de samurais. O silencio entre voces nunca pesa."},
 {"Final com Yotsuba", "Energia Infinita", "{n}, voce conquistou Yotsuba.\n\nEla pula em voce e grita:\n\n\"Eu tambem te amo!!!\"\n\nA vida com ela e feita de corridas e risadas. O casamento e a festa mais animada que a familia ja viu."},
 {"Final com Itsuki", "Determinacao Compartilhada", "{n}, voce escolheu Itsuki.\n\nEla te abraca firme e diz sem hesitar:\n\n\"Vou te amar para sempre. E uma promessa.\"\n\nItsuki vira professora. Voce continua sendo o unico tutor que ela escuta."},
 {"Ichika e Nino", "Rivalidade Doce", "Ichika e Nino chegaram ao mesmo ponto, {n}.\n\nNino cruza os bracos. Ichika sorri de lado:\n\n\"A gente divide.\"\n\nNino resmunga, mas nao discorda. Uma relacao intensa, cheia de provocacoes."},
 {"Ichika e Miku", "Luz e Sombra", "{n}, Ichika e Miku empataram.\n\nIchika puxa Miku pela mao:\n\n\"A gente compartilha.\"\n\nMiku fica vermelha, mas nao recua. Uma traz o brilho, a outra a profundidade."},
 {"Ichika e Yotsuba", "Brilho e Energia", "{n}, Ichika e Yotsuba chegaram juntas.\n\nYotsuba comemora alto enquanto Ichika apenas sorri.\n\nUm trio barulhento e impossivel de ignorar. A casa nunca fica em silencio."},
 {"Ichika e Itsuki", "Maturidade Compartilhada", "{n}, Ichika e Itsuki empataram.\n\nAs duas trocam um olhar de compreensao. So um acordo silencioso.\n\nUma relacao estavel, madura e cheia de respeito."},
 {"Nino e Miku", "Contrastes", "{n}, Nino e Miku empataram.\n\nNino resmunga. Miku so observa. Cada uma aceita do seu jeito, e os jeitos se completam.\n\nUma fala pelos dois, a outra sente pelos dois."},
 {"Nino e Yotsuba", "Tempestade e Sol", "{n}, Nino e Yotsuba chegaram juntas.\n\nYotsuba abraca as duas ao mesmo tempo enquanto Nino tenta se soltar, sem muito esforco.\n\nCaotico, barulhento e muito caloroso."},
 {"Nino e Itsuki", "Orgulho e Determinacao", "{n}, Nino e Itsuki empataram.\n\nTeimosas demais para ceder, decidem que nenhuma vai desistir... entao as duas ficam.\n\nEntre discussoes e sinceridade brutal."},
 {"Miku e Yotsuba", "Silencio e Risada", "{n}, Miku e Yotsuba chegaram juntas.\n\nYotsuba puxa Miku para um abraco coletivo. Miku aceita, ainda vermelha.\n\nUma traz a paz, a outra a alegria."},
 {"Miku e Itsuki", "Quietude e Foco", "{n}, Miku e Itsuki empataram.\n\nNao precisam de muitas palavras.\n\nUma vida calma, estudiosa e profundamente conectada."},
 {"Itsuki e Yotsuba", "Forca e Alegria", "{n}, Itsuki e Yotsuba chegaram juntas.\n\nYotsuba comemora. Itsuki balanca a cabeca, mas esta sorrindo.\n\nDeterminacao e leveza tornam todos os dias interessantes."},
 {"Empate Multiplo", "Tres Coracoes", "{n}, {l} chegaram juntas no topo.\n\nAs irmas se olham, surpresas, e riem. Nenhuma quer desistir... entao nenhuma desiste.\n\nUm arranjo peculiar, cheio de carinho. Nao e convencional. Mas funciona."},
 {"Empate Multiplo", "Quase Todas", "{n}, {l} chegaram juntas no topo.\n\nAs irmas se olham, surpresas, e riem. Nenhuma quer desistir... entao nenhuma desiste.\n\nUm arranjo peculiar, cheio de carinho. Nao e convencional. Mas funciona."},
 {"Final Secreto", "O Impossivel", "{n}... as cinco irmas te olham ao mesmo tempo.\n\n\"Voce realmente conseguiu fazer a gente empatar?\"\n\nPela primeira vez, chegam a um acordo sem discutir:\n\n\"Entao a gente divide.\"\n\nVoce pertence a todas."}
};

/* ---------- utilitarios ---------- */
static void expand(const char *s, char *d) {
  while (*s) {
    if (s[0] == '{' && s[1] == 'n' && s[2] == '}') { strcpy(d, pname); d += strlen(pname); s += 3; }
    else if (s[0] == '{' && s[1] == 'l' && s[2] == '}') { strcpy(d, lst); d += strlen(lst); s += 3; }
    else *d++ = *s++;
  }
  *d = 0;
}

/* quebra de linha por palavra; w = largura, ind = recuo das linhas seguintes */
static void wrap(const char *s, int w, int ind) {
  int col = ind, n;
  char word[40];
  while (*s) {
    if (*s == '\n') { iprintf("\n%*s", ind, ""); col = ind; s++; continue; }
    if (*s == ' ') { s++; continue; }
    n = 0;
    while (s[n] && s[n] != ' ' && s[n] != '\n' && n < 39) { word[n] = s[n]; n++; }
    word[n] = 0; s += n;
    if (col > ind && col + 1 + n > w) { iprintf("\n%*s", ind, ""); col = ind; }
    if (col > ind) { iprintf(" "); col++; }
    iprintf("%s", word); col += n;
  }
  iprintf("\n");
}

static void waitA(void) {
  for (;;) { VB(); scanKeys(); if (keysDown() & KEY_A) break; }
}

/* ---------- save (precisa de flash cart / SD com DLDI) ---------- */
#define MAGIC 0x51544D31
static void loadSave(void) {
  FILE *f = fopen("/quintuplas.sav", "rb");
  u32 m[2]; char nm[9];
  if (!f) return;
  if (fread(m, 4, 2, f) == 2 && m[0] == MAGIC) {
    unlocked = m[1];
    if (fread(nm, 1, 9, f) == 9) { nm[8] = 0; if (nm[0]) strcpy(pname, nm); }
  }
  fclose(f);
}
static void writeSave(void) {
  FILE *f = fopen("/quintuplas.sav", "wb");
  u32 m[2] = {MAGIC, unlocked};
  if (!f) return;
  fwrite(m, 4, 2, f); fwrite(pname, 1, 9, f);
  fclose(f);
}

/* ---------- entrada do nome ---------- */
static void nameEntry(void) {
  static const char CS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  int n = strlen(CS), len, cur, redraw = 1, i, idx;
  char nm[9]; const char *p; u32 k;
  strncpy(nm, pname, 8); nm[8] = 0; len = strlen(nm);
  if (len == 0) { strcpy(nm, "A"); len = 1; }
  cur = len - 1;
  for (;;) {
    if (redraw) {
      consoleSelect(&bot); consoleClear();
      iprintf("\n Digite seu nome:\n\n   %s\n   ", nm);
      for (i = 0; i < cur; i++) iprintf(" ");
      iprintf("^\n\n Cima/Baixo: trocar letra\n Direita/Esquerda: posicao\n B: apagar    A: confirmar\n");
      redraw = 0;
    }
    VB(); scanKeys(); k = keysDown();
    p = strchr(CS, nm[cur]); idx = p ? (int)(p - CS) : 0;
    if (k & KEY_UP)   { nm[cur] = CS[(idx + 1) % n]; redraw = 1; }
    if (k & KEY_DOWN) { nm[cur] = CS[(idx + n - 1) % n]; redraw = 1; }
    if (k & KEY_RIGHT) {
      if (cur < 7) { cur++; if (cur >= len) { nm[cur] = 'a'; nm[cur + 1] = 0; len = cur + 1; } }
      redraw = 1;
    }
    if (k & KEY_LEFT) { if (cur > 0) cur--; redraw = 1; }
    if (k & KEY_B) { if (len > 1) { nm[--len] = 0; if (cur >= len) cur = len - 1; } redraw = 1; }
    if (k & KEY_A) break;
  }
  strcpy(pname, nm);
  writeSave();
}

/* ---------- escolha (D-pad ou toque) ---------- */
static int pickChoice(const char *opts[4]) {
  int sel = 0, redraw = 1, i, row, t;
  u32 k; touchPosition tp;
  for (;;) {
    if (redraw) {
      consoleSelect(&bot); consoleClear();
      iprintf("\x1b[0;0H Cima/Baixo + A, ou toque:");
      for (i = 0; i < 4; i++) {
        iprintf("\x1b[%d;0H", 2 + i * 5);
        iprintf("%c  ", i == sel ? '>' : ' ');
        wrap(opts[i], 30, 3);
      }
      redraw = 0;
    }
    VB(); scanKeys(); k = keysDown();
    if (k & KEY_DOWN) { sel = (sel + 1) % 4; redraw = 1; }
    if (k & KEY_UP)   { sel = (sel + 3) % 4; redraw = 1; }
    if (k & KEY_A) return sel;
    if (k & KEY_TOUCH) {
      touchRead(&tp); row = tp.py / 8;
      if (row >= 2) { t = (row - 2) / 5; if (t >= 0 && t < 4) return t; }
    }
  }
}

/* ---------- finais e conquistas ---------- */
static void ending(void) {
  int best = 0, win[5], nw = 0, id = 0, i, p, isNew;
  char buf[600], tmp[16];
  for (i = 0; i < 5; i++) if (pts[i] > best) best = pts[i];
  for (i = 0; i < 5; i++) if (pts[i] == best) win[nw++] = i;
  if (nw == 1) id = win[0];
  else if (nw == 2) { for (p = 0; p < 10; p++) if (PAIRS[p][0] == win[0] && PAIRS[p][1] == win[1]) id = 5 + p; }
  else if (nw == 3) id = 15;
  else if (nw == 4) id = 16;
  else id = 17;
  lst[0] = 0;
  for (i = 0; i < nw; i++) { snprintf(tmp, sizeof tmp, i ? ", %s" : "%s", SN[win[i]]); strcat(lst, tmp); }
  isNew = !(unlocked & (1u << id));
  unlocked |= (1u << id);
  writeSave();

  consoleSelect(&top); consoleClear();
  iprintf("\n *** %s ***\n Conquista: %s\n\n ", E[id].title, E[id].ach);
  expand(E[id].text, buf);
  wrap(buf, 31, 1);
  consoleSelect(&bot); consoleClear();
  if (isNew) iprintf("\n\n   ** NOVA CONQUISTA! **\n\n   %s\n", E[id].ach);
  iprintf("\n\n   Pressione A para voltar");
  waitA();
}

static void trophies(void) {
  int i, c = 0;
  consoleSelect(&top); consoleClear();
  for (i = 0; i < NEND; i++) if (unlocked & (1u << i)) c++;
  iprintf(" CONQUISTAS  (%d/%d)\n\n", c, NEND);
  for (i = 0; i < NEND; i++) {
    if (unlocked & (1u << i)) iprintf(" [*] %s\n", E[i].ach);
    else iprintf(" [ ] ???\n");
  }
  consoleSelect(&bot); consoleClear();
  iprintf("\n\n   Pressione B para voltar");
  for (;;) { VB(); scanKeys(); if (keysDown() & (KEY_B | KEY_A)) break; }
}

/* ---------- partida ---------- */
static void play(void) {
  int s, i, j, c, ord[5], who[4], t;
  const char *o[4]; char buf[500];
  memset(pts, 0, sizeof pts);
  srand((unsigned)time(NULL) + tick);
  for (s = 0; s < 7; s++) {
    for (i = 0; i < 5; i++) ord[i] = i;
    for (i = 4; i > 0; i--) { j = rand() % (i + 1); t = ord[i]; ord[i] = ord[j]; ord[j] = t; }
    for (i = 0; i < 4; i++) { who[i] = ord[i]; o[i] = S[s].o[who[i]][(rand() >> 7) & 1]; }
    consoleSelect(&top); consoleClear();
    iprintf(" Cena %d/7\n\n %s\n\n ", s + 1, S[s].title);
    expand(S[s].text, buf);
    wrap(buf, 31, 1);
    iprintf("\x1b[22;0H Afin: I%d N%d M%d Y%d S%d", pts[0], pts[1], pts[2], pts[3], pts[4]);
    c = pickChoice(o);
    pts[who[c]] += W[s];
  }
  ending();
}

/* ---------- menu ---------- */
int main(void) {
  videoSetMode(MODE_0_2D); videoSetModeSub(MODE_0_2D);
  vramSetBankA(VRAM_A_MAIN_BG); vramSetBankC(VRAM_C_SUB_BG);
  consoleInit(&top, 0, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
  consoleInit(&bot, 0, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
  if (fatInitDefault()) loadSave();

  for (;;) {
    consoleSelect(&top); consoleClear();
    iprintf("\n\n\n   AS QUINTUPLAS\n   ESSENCIAIS\n\n   Cinco destinos.\n   Uma escolha.\n");
    consoleSelect(&bot); consoleClear();
    iprintf("\n\n\n   A: Jogar\n\n   SELECT: Conquistas\n");
    for (;;) {
      VB(); scanKeys();
      if (keysDown() & KEY_A) { nameEntry(); play(); break; }
      if (keysDown() & KEY_SELECT) { trophies(); break; }
    }
  }
  return 0;
}
