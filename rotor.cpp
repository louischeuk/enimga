#include "rotor.h"

/* constructor */
Rotor::Rotor(const char* filename) {
	code = NO_ERROR;
	number_of_notch = 0;
	top_pos = 0;
	notch_pos = NULL;

	for (int i=0; i < 26; i++) {
		this->alphabet_map[i] = i;
	}
}

/* function that checks the rotor configuration file */
void Rotor::check_config(const char* filename) {

	ifstream in;
	in.open(filename);

	if (in.fail()) {
		cerr << "Error opening configuration file in rotor file: " << filename << endl;
		code = ERROR_OPENING_CONFIGURATION_FILE;
	} else {
		string input;
		map<string, int> rotor_map; // store the existed numbers

		for (int i=0 ; (i<26) && (code == NO_ERROR) ; i++) { // check the first 26 alphabets index
			if (!(in >> input)) {
				cerr << "Not all inputs mapped in rotor file: " << filename << endl;
				code = INVALID_ROTOR_MAPPING;
			} else {
				if (is_repeated_in_map(input, rotor_map)) {
					cerr << "Invalid mapping of input " << i << " to output " << input;
					cerr << " (output " << input <<  " is already mapped to from input ";
					cerr << rotor_map[input] << ") in rotor file: " << filename << endl;
					code = INVALID_ROTOR_MAPPING;
				} else {
					rotor_map.insert(pair<string,int>(input, i));

					if (!is_index_valid(input)) {
						cerr << "Invalid index in rotor file " << filename << endl;
						code = INVALID_INDEX;
					} else if (!is_numeric(input)) {
						cerr << "Non-numeric character for mapping in rotor file " << filename << endl;
						code = NON_NUMERIC_CHARACTER;
					}
				}
			}
		}

		if (code == NO_ERROR) {
			int *temp_notch_pos = new int[26]; // each rotor can have maximum of 26 notches

			for (int i=0 ; (in >> input) && (code == NO_ERROR) ; i++) { // check the turnover notches
				if (!is_index_valid(input)) {
					cerr << "Invalid index in rotor file " << filename << endl;
					code = INVALID_INDEX;
				} else if (!is_numeric(input)) {
					cerr << "Non-numeric character for mapping in rotor file " << filename << endl;
					code = NON_NUMERIC_CHARACTER;
				} else {
					int input_number = string_to_int(input);
					if ((i>0) && is_repeated(i, input_number, temp_notch_pos)) {
						cerr << "Invalid mapping of input " << i << " to output " << input;
						cerr << " (output " << input <<  " is already mapped to from input ";
						cerr << rotor_map[input] << ") in rotor file: " << filename << endl;
						code = INVALID_ROTOR_MAPPING;
					} else {
						temp_notch_pos[i] = input_number;
						number_of_notch++;
					}
				}
			}
			in.close();

			if (code == NO_ERROR) {
				if (number_of_notch == 0) { // rotor must have at least one notch
					cerr << "Invalid rotor mapping in rotor file " << filename << endl;
					code = INVALID_ROTOR_MAPPING;
				}
			}
			number_of_notch = 0; // reset to 0 here
			delete[] temp_notch_pos;
		}
	}
}

/* function that implements the rotor configuration file */
void Rotor::implement_config(const char* filename) {
	ifstream in;
	in.open(filename);

	string input;
	int input_number;
	for (int i=0; i < 26; i++) { 	// first 26 alphabet config
		in >> input;
		input_number = string_to_int(input);
		alphabet_map[i] = input_number; // configuration is stored in righ-hand contact
	}

	int *temp_notch_pos = new int[26]; 	// store notches in temp array
	for (int i=0 ; (in >> input) ; i++) { // input the turnover notches
		input_number = string_to_int(input);
		temp_notch_pos[i] = input_number;
		number_of_notch++;
	}
	in.close();

	notch_pos = new int[number_of_notch]; // store notches in data member
	for (int notch=0 ; notch < number_of_notch ; notch++ ) {
		notch_pos[notch] = temp_notch_pos[notch];
	}
	delete[] temp_notch_pos;
}

/* function that converts the letter from the right-hand contact config to the left-hand contact of rotor config*/
int Rotor::wiring(const int &letter) {
	return alphabet_map[letter];
}

/* function thats converts the letter from the left-hand contact config to the right-hand contact of rotor config */
int Rotor::wiring_inverse(const int &letter) {
	for (int i=0; i < 26; i++) {
		if (wiring(i) == letter)
			return i;
	}
	return letter;
}

/* function that converts the letter index from right-hand position to land-hand position of rotor */
int Rotor::map_r_to_l(const int &letter) {
	int r_contact = mod(letter + top_pos);
	int l_contact = wiring(r_contact);
	int l_pos = mod(l_contact - top_pos);

	return l_pos;
}

/* function that converts the letter index from land-hand position to right-hand position of rotor */
int Rotor::map_l_to_r(const int &letter) {
	int l_contact = mod(letter + top_pos);
	int r_contact = wiring_inverse(l_contact);
	int r_pos = mod(r_contact - top_pos);

	return r_pos;
}

/* function that rotates the rotor by one step */
void Rotor::rotate_one_step() {
	top_pos = mod(top_pos + 1);
}

/* getter function thats get the code */
int Rotor::get_code() {
	return code;
}

/* getter function that gets the top_pos */
int Rotor::get_top_pos() {
	return top_pos;
}

/* setter function that set the top_pos */
void Rotor::set_top_pos(int position) {
	top_pos = position;
}

/* getter function that gets the number_of_notch */
int Rotor::get_number_of_notch() {
	return number_of_notch;
}

/* getter function that gets the notch_pos */
int Rotor::get_notch_pos(const int &iterator) {
	return notch_pos[iterator];
}
