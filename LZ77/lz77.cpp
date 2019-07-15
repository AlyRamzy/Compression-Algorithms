// Your includes here
#include"lz77.h"
#include<algorithm>
//#include<bits/stdc++.h>

// Your implementations here
 // - the input message
  // - S the size of the match buffer i.e. the maximum distance to look
  //   backward for a match
  // - T the size of the lookahead buffer   
  // - A the size of the alphabet
int LZ77::encode(const vector<unsigned char> & msg,
	int S, int T, int A,
	vector<Triplet>* encoded_msg) {
	int i = 0, k = 0, j = 0, offset = -1, lenght = 0, prevj = 0, searchEnd = 0, jend = 0, LookEnd = 0;
	while (i < msg.size()) {
		vector<Triplet > sortTr;

		if (S > searchEnd) {
			j = 0;
			jend = searchEnd;
		}
		else {
			j = i - S;
			jend = i;
		}
		if (j < 0)
			j = 0;
		while (j < jend) 
		{
			if (msg[j] == msg[i])
			{
				prevj = j;
				k = i;
				lenght = 0;
				while (k < T + i && k < msg.size() && lenght < searchEnd&&lenght < S&&j < jend) {

					if (msg[j] == msg[k]) {
						if (offset == -1)
							offset = j;
						j++;
						k++;
						lenght++;
					}
					else {
						break;
					}

				}
				unsigned char c = ' ';
				if (k != msg.size())
					c = msg[k];


				sortTr.push_back( { unsigned int(jend - offset),unsigned int(lenght),c } );
				offset = -1;
				j = prevj;
				j++;
			}
			else
			{

				j++;
			}

		}
		if (sortTr.empty()) {
			sortTr.push_back( { unsigned int(0),unsigned int(0),msg[i] } );
		}
		sort(sortTr.begin(), sortTr.end());
		encoded_msg->push_back(sortTr.back());
		int check = sortTr.back().k;
		if (check == 0) {
			i++;
			searchEnd++;

		}

		else {
			i += sortTr.back().k + 1;
			searchEnd += sortTr.back().k + 1;
		}

	}
	return encoded_msg->size()*(ceil(log2(S)) + ceil(log2(S + T)) + ceil(log2(A)));
}
void LZ77::decode(const vector<Triplet>& encoded_msg,
	vector<unsigned char>* decoded_msg) {



	for (int i = 0; i < encoded_msg.size(); i++) {
		int lenght = 0;

		Triplet tr = encoded_msg[i];
		for (int j = decoded_msg->size() - tr.j; lenght < tr.k; j++)
		{
			lenght++;

			decoded_msg->push_back((*decoded_msg)[j]);
		}
		decoded_msg->push_back(tr.c);




	}



}