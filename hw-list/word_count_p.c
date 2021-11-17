/*
 * Implementation of the word_count interface using Pintos lists and pthreads.
 *
 * You may modify this file, and are expected to modify it.
 */

/*
 * Copyright © 2021 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PINTOS_LIST
#error "PINTOS_LIST must be #define'd when compiling word_count_lp.c"
#endif

#ifndef PTHREADS
#error "PTHREADS must be #define'd when compiling word_count_lp.c"
#endif

#include "word_count.h"
int plock;

void init_words(word_count_list_t* wclist) { /* TODO */
  list_init(&wclist->lst);
  plock = pthread_mutex_init(&wclist->lock, NULL);
  if (plock) {
    printf("ERROR; return code from pthread_mutex_init() is %d\n", plock);
    exit(-1);
  }  
}

size_t len_words(word_count_list_t* wclist) {
  /* TODO */
  size_t result = 0;
  plock = pthread_mutex_init(&wclist->lock, NULL);
   if (plock) {
    printf("ERROR; return code from pthread_mutex_init() is %d\n", plock);
    exit(-1);
  }   
  result = list_size(&wclist);
  pthread_mutex_unlock(&wclist->lock);
  return result;
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  struct list_elem* e;
  plock = pthread_mutex_init(&wclist->lock, NULL);
  if (plock)
  {
    printf("ERROR; return code from pthread_mutex_init() is %d\n", plock);
    exit(-1);
  }
  for (e = list_begin(&wclist->lst); e != list_end(&wclist->lst); e = list_next(e)) {
    struct word_count* current_word = list_entry(e, word_count_t, elem);
    if (!strcmp(current_word->word, word))
    {
      plock = pthread_mutex_unlock(&wclist->lock);
      return current_word;
    }   
  }
  plock = pthread_mutex_unlock(&wclist->lock);
  
  return NULL;
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  word_count_t* current_word;
  plock = pthread_mutex_init(&wclist->lock, NULL);
  if (plock)
  {
    printf("ERROR; return code from pthread_mutex_init() is %d\n", plock);
    exit(-1);
  } 
  current_word = find_word(wclist, word);
  if (current_word != NULL) {
    current_word->count++;
  } else {
    word_count_t* new_word = malloc(sizeof(word_count_t));
    new_word->word = word;
    new_word->count = 1;
    list_push_back(&wclist->lst, &new_word->elem);
    current_word = new_word;
  }

  pthread_mutex_unlock(&wclist->lock);
  return current_word;
}

void fprint_words(word_count_list_t* wclist, FILE* outfile) { /* TODO */
  plock = pthread_mutex_init(&wclist->lock, NULL);
  struct list_elem* e;
  if (plock)
  {
    printf("ERROR; return code from pthread_mutex_init() is %d\n", plock);
    exit(-1);
  } 
  for (e = list_begin(&wclist->lst); e != list_end(&wclist->lst); e = list_next(e)) {
    word_count_t* current_word = list_entry(e, word_count_t, elem);
    fprintf(outfile, "%i\t%s\n", current_word->count, current_word->word);
  }   
  pthread_mutex_unlock(&wclist->lock);

}

static bool less_list(const struct list_elem* ewc1, const struct list_elem* ewc2, void* aux) {
  /* TODO */
  bool (*func_ptr)(word_count_t *, word_count_t *) = aux;
  struct word_count* w1 = list_entry (ewc1, struct word_count, elem);
  struct word_count* w2 = list_entry (ewc2, struct word_count, elem);
  return (*func_ptr)(w1, w2);

}

void wordcount_sort(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*)) {
  /* TODO */
  plock = pthread_mutex_init(&wclist->lock, NULL);
  if (plock)
  {
    printf("ERROR; return code from pthread_mutex_init() is %d\n", plock);
    exit(-1);
  }  
  list_sort(wclist, less_list, less);
  pthread_mutex_unlock(&wclist->lock);
}

