#include "reflector.h"

/* constructor */
Reflector::Reflector(const char* filename) {

	code = NO_ERROR;

	// initialise 0-25 which corrospends to A-Z to the mapping
	for (int i=0; i < 26; i++)
		this->alphabet_map[i] = i;

//	cout << "reflector initialised with config file: " << filename << endl;

}

void Reflector::check_config(const char *filename) {

	ifstream in;
	in.open(filename);

	// if fails to open filename
	if ( in.fail() )
		code = ERROR_OPENING_CONFIGURATION_FILE;
	else {
		string input;
		int count = 0;
		map<string, int> reflector_map;

		// for every string from the filename
		while ( (in >> input) && (code == NO_ERROR) ) {
			count++;

			if (is_repeated_in_map(input, reflector_map)) {
				code = IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
			} else {
				// put every string into the map
				reflector_map.insert(pair<string,int>(input, count));

				// check varies conditions that would cause faulty
				if (count > 26)
					code = INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
				else if (!is_index_valid(input))
					code = INVALID_INDEX;
				else if (!is_numeric(input))
					code = NON_NUMERIC_CHARACTER;
				// else {
				//
				// 	if (!(in >> input))
				// 		code = IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
				// 	else {
				// 		count++;
				// 		if (is_repeated_in_map(input, reflector_map)) {
				// 			code = IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
				// 		} else {
				// 			reflector_map.insert(pair<string,int>(input, count));
				//
				// 			if (!is_index_valid(input))
				// 				code = INVALID_INDEX;
				// 			else if (!is_numeric(input))
				// 				code = NON_NUMERIC_CHARACTER;
				// 		}
				// 	}
				// }
			}
		}
		if (count%2 !=0 )
			code = IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
//		cout << count << endl;
	}
	in.close();
}

void Reflector::implement_config(const char *filename) {

	if (code == NO_ERROR) {
		ifstream in_2;
		in_2.open(filename);

		string input;
		int input_number_1, input_number_2;
		while (in_2 >> input) {
			input_number_1 = string_to_int(input);
			in_2 >> input;
			input_number_2 = string_to_int(input);
			alphabet_map[input_number_1] = input_number_2; // swap the pair
			alphabet_map[input_number_2] = input_number_1;
		}
		in_2.close();
	}
	//////////////////////
//	cout << code << endl;
//	cout << "Reflector array: ";
//	for (int j=0; j < 26; j++) {
//		cout << alphabet_map[j] << " ";
//	}
//	cout << endl;
	///////////////
}

/* encrypt the letter(s) with the plugboard setting */
int Reflector::encrypt(const int &letter) {
	return alphabet_map[letter];
}