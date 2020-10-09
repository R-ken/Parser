#ifndef PARSERCLASSES_H_
#define PARSERCLASSES_H_

//Use only the string library DO NOT add any other libraries
#include <string>
#include <algorithm>

using namespace std;

enum tokenType {T_Operator, T_Identifier, T_Literal, T_CommentBody, T_Other};

struct tokenDetails {
  string type; //boolean, std_logic, std_logic_vector, integer etc.
  int width; //bit width for vector types
};

//Declare your variables for storing delimiters here:
const string delimiter1 = " .=<>\"':+-/*&|;)#(,\t";
const string delimiter2_head = "</>:=*";
const string delimiter2_end = ">=*";
const string whitespace = " ";
const string apos = "'";
const string quot = "\"";
const string comments = "-";
const string comment2 = "--";
const string tab = "\t";
const string newline = "\n";
const string before_quot = "xobXOB";
const string alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string signal_1 = "signal";
const string variable_1 = "variable";
const string vec_1 = "downto";
const string vec_2 = "to";
const string binary = "b";
const string oct_1 = "o";
const string hex_1 = "x";
const string bool_1 = "true";
const string bool_2 = "false";
const string if_string = "if";
const string then_string = "then";
const string when_string = "when";
const string elsif_string = "elsif";
const string else_string = "else";
const string endif_string = "end if";
const string end_string = "end";
const string semicolon = ";";



//Token class for a doubly-linked list of string tokens
class Token {
private:
	Token *next; //Next pointer for doubly linked list
	Token *prev; //Previous pointer for doubly linked list
	string stringRep; //Token value

  bool _isKeyword; //true if token is a reserved keyword
  tokenType type; //enum that holds the type of the token
  tokenDetails *details; //pointer to tokenDetails struct, owned by this token, only valid if type is T_Literal or  is a T_Identifier and is a variable/signal.  Lazy allocation, only allocated when needed (see setTokenDetails function declaration).

	//Allow TokenList class to access Token member variables marked private
  //https://en.wikipedia.org/wiki/Friend_class
	friend class TokenList;

public:
	//Default Constructor, pointers initialized to NULL, and other variable initialization
  //tokenDetails should NOT be allocated here
	Token() : next(nullptr), prev(nullptr), stringRep(""), _isKeyword(false), details(nullptr) {}

    //Constructor with string initialization
    Token(const string &stringRep) : next(nullptr), prev(nullptr), stringRep(stringRep), _isKeyword(false), details(nullptr){}

  //Copy constructor
  Token(const Token &token);

  //Destructor, free any memory owned by this object
  ~Token();

  //Assignment operator
  void operator =(const Token& token);

	//Returns the Token's *next member
	Token* getNext ( ) const {  return next; }

	//Sets the Token's *next member
	void setNext (Token* next ) { this->next = next; }

	//Returns the Token's *prev member
	Token* getPrev ( ) const { return prev; }

	//Sets the Token's *prev member
	void setPrev (Token* prev ){ this->prev = prev; }

	//Returns a reference to the Token's stringRep member variable
	const string& getStringRep ( ) const { return stringRep; }

	//Sets the token's stringRep variable
	void setStringRep (const string& stringRep ) { this->stringRep = stringRep; }

  //Returns true if token is a keyword
  bool isKeyword () const { return _isKeyword; }

  //Sets isKeyword to true
  void setKeyword() { _isKeyword = true; }

  //Returns the token type
  tokenType getTokenType() const { return type; }

  //Set's the token type
  void setTokenType(tokenType type) { this->type = type; }

  //Returns true if token matches this type
  bool isOperator() const { return (type == T_Operator); }
  //Returns true if token matches this type
  bool isIdentifier() const { return (type == T_Identifier); }
  //Returns true if token matches this type
  bool isLiteral() const { return (type == T_Literal); }
  //Returns true if token matches this type
  bool isComment() const { return (type == T_CommentBody); }
  //Returns true if token matches this type
  bool isOther() const { return (type == T_Other); }

  //Returns a pointer to tokenDetails
  tokenDetails* getTokenDetails() const { return details; }

  //Set's the tokenDetails given a string type and optional vector width
  //Allocates tokenDetails if it doesn't already exist
  void setTokenDetails(const string &type, int width = 0);

};

//A doubly-linked list class consisting of Token elements
class TokenList {
private:
	Token *head; //Points to the head of the token list (doubly linked)
	Token *tail; //Points to the tail of the function list (doubly linked)

	//gets called whenever we find a token that contains tokenDetails
    //sets the type and width for the specific token
	void setSignalandVariableDetails(Token *token);

public:
	//Default Constructor, Empty list with pointers initialized to NULL
	TokenList() : head(nullptr), tail(nullptr) { }

	//Returns a pointer to the head of the list
	Token* getFirst() const { return head; }

	//Returns a pointer to the tail of the list
	Token* getLast() const { return tail; }

	//Creates a new token for the string input, str
	//Appends this new token to the TokenList
	//On return from the function, it will be the last token in the list
	void append(const string &str); //example comment

	//Appends the token to the TokenList if not null
	//On return from the function, it will be the last token in the list
	void append(Token *token);

  //Removes the token from the linked list if it is not null
	//Deletes the token
	//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
	void deleteToken(Token *token);

  //find token details and type and update token.  May require examining properties of neighbouring tokens
  void findAndSetTokenDetails(Token *token);

};

//A class for tokenizing a string of VHDL  code into tokens
class Tokenizer {
private:
	/*State tracking variables for processing a single string*/
	bool complete; //True if finished processing the current string

	size_t offset; //Current position in string
	size_t tokenLength; //Current token length
	string *str; //A pointer to the current string being processed

	//Include any helper functions here
	//e.g. trimming whitespace, comment processing
	void processComments();

	//Computes a new tokenLength for the next token
	//Modifies: size_t tokenLength, and bool complete
	//(Optionally): may modify offset
	//Does NOT modify any other member variable of Tokenizer
	void prepareNextToken();

public:
	//Default Constructor- YOU need to add the member variable initializers.
	Tokenizer() : complete(false), offset(0), tokenLength(0), str(NULL) {}

	//Sets the current string to be tokenized
	//Resets all Tokenizer state variables
	//Calls Tokenizer::prepareNextToken() as the last statement before returning.
	void setString(string *str);

	//Returns true if all possible tokens have been extracted from the current string (string *str)
	bool isComplete() const
	{
        string temp_string;

        temp_string = *str;

        return(temp_string.empty());
	}

	//Returns the next token. Hint: consider the substr function
	//Updates the tokenizer state
	//Updates offset, resets tokenLength, updates processingABC member variables
	//Calls Tokenizer::prepareNextToken() as the last statement before returning.
	string getNextToken();
};


//Removes all comments from the tokenList including the -- marker
//Returns the number of comments removed
int removeComments(TokenList &tokenList);

//Removes all tokens of the given tokenType
//Returns the number of tokens removed
int removeTokensOfType(TokenList &tokenList, tokenType type);


//Creates a new TokenList, and returns a pointer to this list
//Searches for all conditional expressions in tokenList and appends them to the new list
//Format is as follows:
//Each token that is part of a condtional expression is appended sequentially
//At the end of a conditional expression a newline character is appened
   //Example: if (a = true) then
   //Your list should include "(", "a", "=", "true", ")" and "\n"
//tokenList is NOT modified
TokenList* findAllConditionalExpressions(const TokenList &tokenList);

void checkForMissingThenandMismatch(const TokenList &tokenList);

void numberOftokens(const TokenList &tokenList);

void checkForMissingendif(const TokenList &tokenList);

void nestedIf(Token *token);


#endif /* PARSERCLASSES_H_ */
