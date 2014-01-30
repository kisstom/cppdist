/*
 * ndcg.h
 *
 *  Created on: 2013.07.22.
 *      Author: kisstom
 */

#ifndef NDCG_H_
#define NDCG_H_
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;

class NDCG {
public:
	NDCG(string, string, int);
	double dcg(vector<double>*);
	// TODO score2 miert nem lehet const
	double ndcgForOneUser(const map<long, double>& score1,
			 map<long, double>& score2);
	void GetRelevanceByFirst(const map<long, double>& score1,
			 map<long, double>& score2, vector<double>* relevance);
	void GetRelevanceBySecond(map<long, double>& score2, vector<double>* relevance);
	void readScores(map<long, map<long, double> >& scores, string file);
	void run();
private:
	map<long, map<long, double> > score1_;
	map<long, map<long, double> > score2_;
	string scoreFile1_;
	string scoreFile2_;
	double cumNdcg_;
	int topk_;
};
#endif /* NDCG_H_ */
