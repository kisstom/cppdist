/*
 * pseudo_random.h
 *
 *  Created on: 2014.01.20.
 *      Author: mrita
 */

#ifndef PSEUDO_RANDOM_H_
#define PSEUDO_RANDOM_H_

class PseudoRandom {
  protected:
    int seed;
  public:
    virtual int get(short fp, short step, long node) = 0;
};

#endif /* PSEUDO_RANDOM_H_ */
