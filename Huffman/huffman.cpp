#include "huffman.h"

using namespace std;

// Your implementations here
float Huffman::compute_entropy(const vector<unsigned char>& msg,
	vector<Symbol>* prob) {
	vector<double> freqVector;
	vector <unsigned char> uniqElements = msg;
	//First Get The uniqElements Elements 
	vector<unsigned char>::iterator it;
	sort(uniqElements.begin(), uniqElements.end());
	it = std::unique(uniqElements.begin(), uniqElements.end());
	uniqElements.resize(std::distance(uniqElements.begin(), it));
	//Second Get The Frequency of Uniq Elements 
	for (int i = 0; i < uniqElements.size(); i++) {
		double freq = 1.0*std::count(msg.begin(), msg.end(), uniqElements[i]) / msg.size();
		freqVector.push_back(freq);
		Symbol s;
		s.prob = freq;
		s.val = uniqElements[i];
		prob->push_back(s);

	}
	float entropy = 0;
	for (int i = 0; i < uniqElements.size(); i++) {
		entropy += -1 * freqVector[i] * log2(freqVector[i]);
	}
	return entropy;
}
void Huffman::build_tree(const vector<Symbol>& prob) {
	vector<Symbol>  prob1 = prob;
	vector<Node> nodes;
	for (int i = 0; i < prob1.size(); i++) {
		Node* node = new Node();
		node->s = prob1[i];
		nodes.push_back(*node);
	}
	Node* node, *right, *left;
	for (int i = 0; i < nodes.size() - 1; i += 2) {
		sort(nodes.begin(), nodes.end());
		left = new Node();
		right = new Node();
		left->s = nodes[i].s;
		left->left = nodes[i].left;
		left->right = nodes[i].right;
		right->s = nodes[i + 1].s;
		right->left = nodes[i + 1].left;
		right->right = nodes[i + 1].right;
		node = new Node();
		node->left =  left;
		node->right = right;
		node->s.prob = nodes[i].s.prob + nodes[i + 1].s.prob;
		nodes.push_back(*node);
	}
	tree = &nodes[nodes.size() - 1];
	print_code_table();
}

void Huffman::print_code_table() {
	print_code_table_rec(tree, "");
	cout << "Print Code Table " << endl;
	for (int i = 0; i < codeword.size(); i++) {
		cout << int(codeword[i].first) << " ";
		cout << codeword[i].second << endl;
	}
}
void  Huffman::print_code_table_rec(Node*node, string codewordvalue) {
	if (node != NULL) {  // (Otherwise, there's nothing to print.)
		if (node->left == NULL && node->right == NULL) {
			pair<unsigned char, string> code;
			code.first = node->s.val;
			code.second = codewordvalue;
			codeword.push_back(code);
		}// Print the root item.
		print_code_table_rec(node->left, codewordvalue + "0");    // Print items in left subtree.
		print_code_table_rec(node->right, codewordvalue + "1");   // Print items in right subtree.
	}

}
int Huffman::encode(const vector<unsigned char>& msg,
	vector<unsigned int>* encoded_msg) {
	int lenght = 0;
	for (int i = 0; i < msg.size(); i++) {
		int k = 0;
		//find matching in the codeword table 
		for (k = 0; k < codeword.size(); k++) {
			if (msg[i] == codeword[k].first)
				break;
		}
		string code = codeword[k].second;
		lenght += code.size();
		bitset<32>c(code);
		//for(int j=0;j<code.length();j++)
		//encoded_msg->push_back(code[j]-48);
		encoded_msg->push_back((unsigned int)(c.to_ulong()));

	}
	return lenght;
}
