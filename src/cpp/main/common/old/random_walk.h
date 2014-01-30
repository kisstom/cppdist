/*
 * random_walk.h
 *
 *  Created on: 2013.07.29.
 *      Author: kisstom
 */

#ifndef RANDOM_WALK_H_
#define RANDOM_WALK_H_

struct RandomWalk {
	static const int RWMAX = 11;
	long buf[RWMAX];
	short edge_num;
	short fp_index;
};

#endif /* RANDOM_WALK_H_ */
