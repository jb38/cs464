/*
 * Jim Blaney
 * CS464
 * Spring 2014
 * Homework 3
 * Approximation of Pi using the Monte Carlo method and pthreads
 */

#include <iostream>
#include <pthread.h>
#include <math.h>
#include <random>

using namespace std;

#define NUM_THREADS           4
#define NUM_POINTS_PER_THREAD 25000

random_device generator;
uniform_real_distribution<double> distribution(0.0, 1.0);

struct thread_arg {
  int result;
};

void *process_random_distances(void *arguments)
{   
    struct thread_arg *arg = (struct thread_arg *)arguments; 
    int i, total = 0;
    double x, y;
    
    for (i = 0; i < NUM_POINTS_PER_THREAD; i++) {
      // generate the random point
      x = distribution(generator);
      y = distribution(generator);
      
      // if the point falls within a distance of 1 from (0, 0), count it
      if (sqrt(pow(x, 2) + pow(y, 2)) <= 1) {
        total++;
      }
    }
    
    // set the result
    arg->result = total;
    
    pthread_exit(NULL);
}

int main() {
  pthread_t  threads[NUM_THREADS];
  thread_arg args[NUM_THREADS];
  int i, total = 0;
  double pi_approx = 0;
  
  // spawn threads and block on completion
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, &process_random_distances, &args[i]);
    pthread_join(threads[i], NULL);
  }
  
  // collect the results
  for (i = 0; i < NUM_THREADS; i++) {
    total += args[i].result;
  }
  
  // approximate pi
  pi_approx = 4 * total / (double)(NUM_THREADS * NUM_POINTS_PER_THREAD);
  
  // report findings
  cout << pi_approx << endl;
  
  return 0;
}
