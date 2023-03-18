//Bunnies! (3-11-2023)
//Code by Arnav Gupta
/*
* Exercise from https://cplusplus.com/forum/articles/12974/
* Excercises:
* -Classes
* -Linked Lists
* -File io
* -Vectors
* 
* Simulates a population of bunnies.
* Each year there are newborns (1 per female) and bunnies older than 10 die.
* Bunnies can be born mutant, making them infect one other bunny every year.
* Mutant bunnies live to age 50 and can't breed.
*/

/*
* Headers
*/
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

//Random number generator
int rngInt(int min, int max) {
	return min + rand() % (max - min + 1);
}

//Name and colour libraries
string const bunnyNames[87] = { "Luke Skywalker","C-3PO","R2-D2","Darth Vader","Leia Organa","Owen Lars","Beru Whitesun lars","R5-D4","Biggs Darklighter","Obi-Wan Kenobi","Anakin Skywalker","Wilhuff Tarkin","Chewbacca","Han Solo","Greedo","Jabba Desilijic Tiure","Wedge Antilles","Jek Tono Porkins","Yoda","Palpatine","Boba Fett","IG-88","Bossk","Lando Calrissian","Lobot","Ackbar","Mon Mothma","Arvel Crynyd","Wicket Systri Warrick","Nien Nunb","Qui-Gon Jinn","Nute Gunray","Finis Valorum","Jar Jar Binks","Roos Tarpals","Rugor Nass","Ric Olié","Watto","Sebulba","Quarsh Panaka","Shmi Skywalker","Darth Maul","Bib Fortuna","Ayla Secura","Dud Bolt","Gasgano","Ben Quadinaros","Mace Windu","Ki-Adi-Mundi","Kit Fisto","Eeth Koth","Adi Gallia","Saesee Tiin","Yarael Poof","Plo Koon","Mas Amedda","Gregar Typho","Cordé","Cliegg Lars","Poggle the Lesser","Luminara Unduli","Barriss Offee","Dormé","Dooku","Bail Prestor Organa","Jango Fett","Zam Wesell","Dexter Jettster","Lama Su","Taun We","Jocasta Nu","Ratts Tyerell","R4-P17","Wat Tambor","San Hill","Shaak Ti","Grievous","Tarfful","Raymus Antilles","Sly Moore","Tion Medon","Finn","Rey","Poe Dameron","BB8","Captain Phasma","Padmé Amidala" };
string const bunnyColours[4] = { "White", "Brown", "Black", "Spotted" };

//Can be changed by user input
int mutationChance = 2;

class Bunny {
private:
	//Dependent variables
	string colour;
	char sex;
	//Randomized independent variables
	string name = bunnyNames[rngInt(0, 87 - 1)];
	int tempSex = rngInt(0, 1);
	bool mutant = rngInt(0, 100) < mutationChance;

public:
	//For starting bunnies
	Bunny() {
		colour = bunnyColours[rngInt(0, 4 - 1)];
		age = rngInt(0, 10);

		switch (tempSex) {
		case 0:
			sex = 'F';
			break;
		case 1:
			sex = 'M';
			break;
		default:
			break;
		}
	}
	//For new-born bunnies
	Bunny(string colour) {
		this->colour = colour;
		age = 0;

		switch (tempSex) {
		case 0:
			sex = 'F';
			break;
		case 1:
			sex = 'M';
			break;
		default:
			break;
		}
	}

	string getName() { return name; }
	string getColour() { return colour; }
	int age;
	int getAge() { return age; }
	char getSex() { return sex; }
	bool getMutant() { return mutant; }

	//Returns whether bunny is able to breed
	bool getFertile() {
		return !mutant && age >= 2;
	}

	//Makes the bunny mutant if not already, otherwise return false
	bool makeMutant() {
		if (!mutant) { mutant = true; return true; }
		return false;
	}

	bool age_one_year() {
		++age;
		//Bunny dies if older than 10 (mutants live to 50)
		if ((!mutant && age > 10) || (mutant && age > 50)) {
			return true;
		}
		return false;
	}
};

//Outputs all bunnies and their data to file
void printList(list<Bunny> l, ofstream* output) {
	string toOutput = "";
	for (Bunny i : l) {
		toOutput +=  i.getName() + " aged " + to_string(i.getAge()) + ", " + i.getSex() + ", " + i.getColour();
		if (i.getMutant()) { toOutput += ", mutant"; }
		toOutput += "\n";
	}
	*output << toOutput << endl;
}

//Compares ages of two Bunnies
bool compareAge(const Bunny& first, const Bunny& last) {
	return first.age < last.age;
}

/*
* main
*/
int main() {
	//Create input and output files for read and write
	ifstream input;
	ofstream output;
	input.open("input.txt");
	output.open("bunniesLog.txt");
	if (!(input.is_open() && output.is_open())) { return 1; }

	//Seed random number generation
	srand(time(nullptr));

	//Declare doubly-linked list of Bunny objects
	list<Bunny> bunnies;
	typedef list<Bunny>::iterator bunny_iterator;

	//Get user input (swap cin to input for file reading)
	short startingPopulation;

	cout << "Enter starting population: ";
	cin >> startingPopulation;
	
	cout << "Enter mutation chance: ";
	cin >> mutationChance;

	if (startingPopulation < 1) { startingPopulation = 1; }
	else if (startingPopulation > 500) { startingPopulation = 500; }
	if (mutationChance <= 0) { mutationChance = 1; }

	cout << endl;

	//Add beginning population of bunnies
	for (int i = 0; i < startingPopulation; i++) {
		bunnies.push_back(Bunny());
	}


	//Sort the bunnies by age	(Sorting only has to be done on starting group as
	//							 new-borns will be added to the front of the list)
	bunnies.sort(compareAge);

	printList(bunnies, &output);

	short turns = 0;
	//Stores turns on which a population of 1000+ was reached
	vector<short> maxPopulationTurns;
	//Act until all bunnies are dead
	while (!bunnies.empty()) {
		//Data for each turn
		turns++;
		string eventOutput = "";
		short currentPopulation = 0;
		bool malePresent = false;
		short femaleBunnies = 0;
		vector<string> motherColours;
		short toMutate = 0;

		//Traverses bunnies list where b points to the current bunny
		bunny_iterator b = bunnies.begin();
		while (b != bunnies.end()) {

			//Switches malePresent to true when/if first fertile male is found
			if (!malePresent && b->getSex() == 'M' && b->getFertile()) { malePresent = true; }
			//Tallies total fertile females and stores their colour to be used for new-borns
			else if (b->getSex() == 'F' && b->getFertile()) {
				femaleBunnies++;
				motherColours.push_back(b->getColour());
			}

			//Tallies amount of mutations to occur
			if (b->getMutant()) { toMutate++; }

			//Ages bunny and erases it if past age limit
			if (b->age_one_year()) {
				//Add death to output
				if (b->getMutant()) { eventOutput += "Mutant "; }
				eventOutput += b->getName() + " died\n";
				//erase() returns the next element, effectively b++
				b = bunnies.erase(b);
			}
			else { b++; }

			//If population exceeds 1000, half the bunnies die
			currentPopulation = bunnies.size();
			if (currentPopulation > 1000) {
				//Add event to start of event output
				eventOutput.insert(0, "Starvation event occured.\n");
				//Iterates through until half the bunnies remain
				bunny_iterator e = bunnies.begin();
				while (bunnies.size() > ceil(currentPopulation / 2)) {
					//Adds death to event output
					if (e->getMutant()) { eventOutput += "Mutant "; }
					eventOutput += e->getName() + " died\n";

					//Erases element and e becomes next element
					e = bunnies.erase(e);
					//e increases again to skip one element
					if (e != bunnies.end()) { e++; }
				}
			}

		}

		//If atleast one fertile male, for each fertile female add a bunny with colour of female i
		if (malePresent) {
			for (int i = 0; i < femaleBunnies; i++) {
				bunnies.push_front(Bunny(motherColours[i]));
				//Add birth to output
				if (bunnies.begin()->getMutant()) { eventOutput += "Mutant "; }
				eventOutput += bunnies.begin()->getName() + " was born\n";
			}
		}

		//For every mutant, turn a non-mutant bunny into one.
		for (bunny_iterator b = bunnies.begin(); b != bunnies.end(); b++) {
			//Only decreases toMutate if bunny wasn't already mutant
			if (toMutate > 0 && b->makeMutant()) { toMutate--; }
		}

		//Outputs turn's events and alive bunnies to file
		eventOutput.insert(0, "Turn " + to_string(turns) + "\n");
		eventOutput += "\n" + to_string(currentPopulation) + " Bunnies:\n";
		output << eventOutput;
		printList(bunnies, &output);
	}

	output << "All bunnies died in " << turns << " turns" << endl;
	cout << "Check output file for log." << endl;

	return 0;
}