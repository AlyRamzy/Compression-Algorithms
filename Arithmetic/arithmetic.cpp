// Your includes here
#include"arithmetic.h"

using namespace std;

// Your implementations here
double Arithmetic::binary_to_decimal(const std::bitset<32>& fraction) {
	double output = 0;
	for (int i = 0; i < fraction.size(); i++) {
		output += fraction[i] / (pow(2, i + 1));
	}
	return output;




}

std::bitset<32> Arithmetic::decimal_to_binary(double fraction) {

	std::bitset<32> output;
	output.reset();
	for (int i = 0; i < 32; i++) {
		fraction = fraction * 2;
		if (fraction >= 1) {
			output[i] = 1;
			fraction -= 1;
		}
		else {
			output[i] = 0;
		}
	}

	return output;
}

int Arithmetic::encode(const std::vector<unsigned char>& msg,
	int num_symbols_per_block,
	std::vector<std::bitset<32> >* encoded_msg) {
	int bits = 0;
	vector<double> freqVector;
	vector <unsigned char> uniqElements = msg;
	//First Get The uniqElements Elements 
	vector<unsigned char>::iterator it;
	sort(uniqElements.begin(), uniqElements.end());
	it = std::unique(uniqElements.begin(), uniqElements.end());
	uniqElements.resize(std::distance(uniqElements.begin(), it));
	//Second Get The Frequency of Uniq Elements 
	for (int i = 0; i < uniqElements.size(); i++) {
		int freq = std::count(msg.begin(), msg.end(), uniqElements[i]);
		freqVector.push_back(freq);

	}
	//Calculate Propability
	for (int i = 0; i < freqVector.size(); i++) {
		freqVector[i] = freqVector[i] / msg.size();

	}
	//Making Intervals 
	for (int i = 1; i < freqVector.size(); i++) {
		freqVector[i] += freqVector[i - 1];
	}
	freqvec = freqVector;
	uniqvec = uniqElements;

	/////////////////////////////////////////////START ENCODING///////////////////////////////////////////
	for (int i = 0; i < msg.size(); i += num_symbols_per_block) {
		double l = 0, u = 0, w = 0, tag = 0, Fn = 0, F0 = 0;
		int index;
		for (index = 0; index < uniqElements.size(); index++) {
			if (uniqElements[index] == msg[i])
				break;
		}
		u = freqVector[index];
		if (index != 0)
			l = freqVector[index - 1];



		for (int j = i + 1; j < i + num_symbols_per_block && j < msg.size(); j++) {
			w = u - l;
			for (index = 0; index < uniqElements.size(); index++) {
				if (uniqElements[index] == msg[j])
					break;
			}
			Fn = freqVector[index];
			if (index != 0)
				F0 = freqVector[index - 1];
			else
				F0 = 0;
			u = l + w * Fn;
			l = l + w * F0;

		}
		tag = (l + u) / 2;
		if (fabs(u - l) < 1e-18)
			bits += 32;
		else
			bits += (ceil(log2(fabs(1.0 / (u - l)))) + 1);
		encoded_msg->push_back(decimal_to_binary(tag));
	}
	return bits;
}

void Arithmetic::decode(const std::vector<std::bitset<32> >& encoded_msg,
	int num_symbols_per_block,
	std::vector<unsigned char>* decoded_msg) {


	double l = 0, u = 1, tag, w, Fn, F0, t;

	for (int i = 0; i < encoded_msg.size(); i++) {
		tag = binary_to_decimal(encoded_msg[i]);
		l = 0;
		u = 1;
		for (int j = 0; j < num_symbols_per_block; j++) {
			w = u - l;
			t = (tag - l) / (w);
			int k = 0;
			for (k = 0; k < freqvec.size(); k++) {
				if (t < freqvec[k])
					break;
			}
			Fn = freqvec[k];
			if (k != 0)
				F0 = freqvec[k - 1];
			else
				F0 = 0;
			decoded_msg->push_back(uniqvec[k]);
			u = l + w * Fn;
			l = l + w * F0;

		}
	}
}