//Use only the following three libraries:
#include "parserClasses.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

//Example Test code for interacting with your Token, TokenList, and Tokenizer classes
//Add your own code to further test the operation of your Token, TokenList, and Tokenizer classes
int main() {
	ifstream sourceFile;
	TokenList tokens;
	TokenList *conditional_ptr;
	Token *new_list_ptr;

  //Lists for types of tokens
  TokenList operatorTokens;
  TokenList identifierTokens;
  TokenList literalTokens;
  TokenList commentBodyTokens;
  TokenList otherTokens;

	Tokenizer tokenizer;

	//Read in a file line-by-line and tokenize each line
	sourceFile.open("trickyCombinations.vhd");
	if (!sourceFile.is_open()) {
		cout << "Failed to open file" << endl;
		return 1;
	}

	while(!sourceFile.eof()) {
		string line;
		getline(sourceFile, line);

		tokenizer.setString(&line);
		while(!tokenizer.isComplete()) {
			tokens.append(tokenizer.getNextToken());
		}
		tokens.append("\n");
	}


	/*Test your tokenization of the file by traversing the tokens list and printing out the tokens*/
	Token *t = tokens.getFirst();

	cout << "LIST OF ALL OUR TOKENS" << endl;
	while(t) {
		cout << t->getStringRep() << " ";
		t = t->getNext();
	}

  /* For your testing purposes only */
    t = tokens.getFirst();
    while(t)
    {
        tokens.findAndSetTokenDetails(t);
        t = t->getNext();
    }

    //cout << endl << endl << endl;
    //cout << "LIST OF ALL THE TOKEN TYPES" << endl;
    t = tokens.getFirst();
    while(t)
    {
        //cout << t->getTokenType() << " ";
        t = t->getNext();
    }

    //cout << endl << endl << endl;
    //cout << "LIST OF ALL OUR CONDITIONAL TOKENS" << endl;
    conditional_ptr = findAllConditionalExpressions(tokens);
    new_list_ptr = conditional_ptr->getFirst();

    while(new_list_ptr)
    {
        //cout << new_list_ptr->getStringRep() << " ";
        new_list_ptr = new_list_ptr->getNext();
    }

  /* Ensure that tokens have all type information set*/

  /* Create operator,identifier,literal, etc. tokenLists from the master list of tokens */
    cout << endl << endl << endl;
    //cout << "CHECKING FOR MISSING THEN AND TYPE/WIDTH MISMATCHING" << endl;
    checkForMissingThenandMismatch(tokens);


    cout << endl << endl << endl;
    cout << "TOTAL NUMBER OF TOKENS" << endl;
    numberOftokens(tokens);

    cout << endl << endl << endl;
    cout << "TOTAL NUMBER OF MISSING END IF" << endl;
    checkForMissingendif(tokens);


	return 0;
}
