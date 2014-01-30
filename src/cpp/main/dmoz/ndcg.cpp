/*
 * ndcg.cpp
 *
 *  Created on: 2013.07.22.
 *      Author: kisstom
 */

#include "ndcg.h"
#include <math.h>
#include <queue>
#include "../common/util/MyCompare.h"
#include <stdio.h>
#include <stdlib.h>

using std::pair;
using std::priority_queue;

NDCG::NDCG(string file1, string file2, int topk) {
  cumNdcg_ = 0.0;
  scoreFile1_ = file1;
  scoreFile2_ = file2;
  topk_ = topk;
}

double NDCG::dcg(vector<double>* relevants) {
	double dcg = 0.0;
	for (int i = 0; i < (int) relevants->size(); ++i) {
		dcg += (pow(2.0, (*relevants)[i]) - 1.0) / log2(i + 2);
	}
	return dcg;
}

double NDCG::ndcgForOneUser(const map<long, double>& score1,
			 map<long, double>& score2) {
  double dcgValue = 0.0;
  double idcgValue = 0.0;

  vector<double>* rel = new vector<double>;
  GetRelevanceByFirst(score1, score2, rel);
  dcgValue = dcg(rel);

  rel->clear();
  GetRelevanceBySecond(score2, rel);
  idcgValue = dcg(rel);

  delete rel;

  if (idcgValue != 0.0) {
  	printf("%lf %lf %lf\n", dcgValue, idcgValue, dcgValue / idcgValue);
  	return dcgValue / idcgValue;
  }
  return 0.0;
}



void NDCG::GetRelevanceByFirst(const map<long, double>& score1,
		map<long, double>& score2, vector<double>* relevance) {
  long nextPage;
	priority_queue<pair<long, double>, vector<pair<long, double> >, MyCompare> firstSortedScore;

	for (map<long, double>::const_iterator it = score1.begin(); it != score1.end(); ++it) {
		printf("First %ld %lf\n", it->first, it->second);
    firstSortedScore.push(std::make_pair<long, double>(it->first, it->second));
	}

	while (!firstSortedScore.empty()) {
		nextPage = firstSortedScore.top().first;
		if (score2.find(nextPage) != score2.end()) {
			relevance->push_back(score2[nextPage]);
		} else {
			relevance->push_back(0.0);
		}
		printf("Rel %ld %lf\n", nextPage, score2[nextPage]);
		firstSortedScore.pop();
	}
}

void NDCG::GetRelevanceBySecond(map<long, double>& score, vector<double>* relevance) {
	priority_queue<double, vector<double>, std::less<double> > firstSortedScore;
	for (map<long, double>::const_iterator it = score.begin(); it != score.end(); ++it) {
	  firstSortedScore.push(it->second);
	}

	while (!firstSortedScore.empty()) {
		relevance->push_back(firstSortedScore.top());
	  firstSortedScore.pop();
	}
}

void NDCG::readScores(map<long, map<long, double> >& scores, string file) {
	FILE* fileO = fopen(file.c_str(), "r");
	long page1, page2;
	double score;

	while (fscanf(fileO, "%ld %ld %lf\n", &page1, &page2, &score) == 3) {
		if ((int) scores[page1].size() < topk_) {
		  scores[page1][page2] = score;
		}
	}

	fclose(fileO);
}

void NDCG::run() {
	readScores(score1_, scoreFile1_);
	readScores(score2_, scoreFile2_);

	int norm = 0;
	for (map<long, map<long, double> >::iterator it = score1_.begin(); it != score1_.end(); ++it) {
		if (score1_.find(it->first) != score1_.end() && score2_.find(it->first) != score2_.end()) {
                  printf("%ld\n", it->first);
		  cumNdcg_ += ndcgForOneUser(score1_[it->first], score2_[it->first]);
		  ++norm;
		}
	}

	if (0 != norm) {
		cumNdcg_ /= norm;
	}
	printf("ndcg average %lf\n", cumNdcg_);
}
