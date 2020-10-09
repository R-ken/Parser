//Use only the following libraries:
#include "parserClasses.h"
#include <iostream>
#include <string>
#include <algorithm>


//****Token class function definitions******

//copy constructor
Token::Token(const Token &token)
{
    tokenDetails *temp = token.getTokenDetails();

    if(temp == nullptr)
    {
        details = nullptr;
    }
    else
    {
        details = new tokenDetails;
        details->type = temp->type;
        details->width = temp->width;
    }

    stringRep = token.getStringRep();
    _isKeyword = token.isKeyword();
    type = token.getTokenType();
}

//Destructor
Token::~Token()
{
    if(details != nullptr)
    {
        delete details;
    }
}

//overloaded assignment operator
void Token::operator =(const Token& token)
{
    if(&token != this)  //protection against self assignment
    {
        tokenDetails *temp = token.getTokenDetails();

        if(temp == nullptr && details != nullptr)
        {
            delete details;
            details = nullptr;
        }
        else if(details == nullptr && temp != nullptr)
        {
            details = new tokenDetails;
            details->type = temp->type;
            details->width = temp->width;
        }
        else
        {
            details->type = temp->type;
            details->width = temp->width;
        }
        stringRep = token.getStringRep();
        _isKeyword = token.isKeyword();
        type = token.getTokenType();
    }
}

void Token::setTokenDetails(const string &type, int width)
{
    if(details == nullptr)
    {
        details = new tokenDetails;
    }

    details->type = type;
    details->width = width;
}

//****TokenList class function definitions******
//           function implementations for append have been provided and do not need to be modified

//Creates a new token for the string input, str
//Appends this new token to the TokenList
//On return from the function, it will be the last token in the list
void TokenList::append(const string &str) {
	Token *token = new Token(str);
	append(token);
}

//Appends the token to the TokenList if not null
//On return from the function, it will be the last token in the list
void TokenList::append(Token *token) {
	if (!head) {
		head = token;
		tail = token;
	}
	else {
		tail->setNext(token);
		token->setPrev(tail);
		tail = token;
	}
}

//find token details and type and update token.  May require examining properties of neighbouring tokens
void TokenList::findAndSetTokenDetails(Token *token)
{
    string keyword[97] = {"abs","access","after","alias","all","and","architecture","array","assert",
    "attribute","begin","block","body","buffer","bus","case","component","configuration","constant","disconnect",
    "downto","else","elsif","end","entity","exit","file","for","function","generate","generic","group",
    "guarded","if","impure","in","inertial","inout","is","label","library","linkage","literal","loop",
    "map","mod","nand","new","next","nor","not","null","of","on","open","or","others","out","package","port","postponed",
    "procedure","process","pure","range","record","register","reject","rem","report","return","rol","ror","select",
    "severity","signal","shared","sla","sll","sra","srl","subtype","then","to","transport","type","unaffected",
    "units","until","use","variable","wait","when","while","with","xnor","xor"};

    string operators[30] = {"**","abs","not","*","/","mod","rem","+","-",
    "&","sll","srl","sla","sra","rol","ror","=","/=","<","<=",">",">=","and","or",
    "nand","nor","xor","xnor", ":=", "<>"};

    string identifiers[37] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","_","0","1",
    "2","3","4","5","6","7","8","9"};

    string temp_holder = token->getStringRep();
    string converted_to_lower ="";
    string convert_prev ="";
    string prev_token_value ="";
    string temp_char_holder_identifier ="";
    string *array_search_keyword = nullptr;
    string *array_search_operator = nullptr;
    string *array_search_identifier = nullptr;
    string first_value ="";
    Token *before_ptr = nullptr;
    tokenDetails *check_details_ptr = nullptr;
    int length = temp_holder.length();
    int lengthofPrev;
    bool numeric_token = false;
    bool identifier_token = false;
    char numeric;
    char tolowercase;
    char first_char;
    size_t found_quot;
    size_t found_apos;
    size_t found_apos2;

    check_details_ptr = token->getTokenDetails();

    //Takes in the previous token and converts it into a lower case string
    if(token->getPrev() != nullptr)
    {
        before_ptr = token->getPrev();
        prev_token_value = before_ptr->getStringRep();
        lengthofPrev = prev_token_value.length();

        for(int i=0; i<lengthofPrev; i++) //convert previous token all to lower case
        {
            tolowercase = prev_token_value.at(i);
            tolowercase = tolower(tolowercase);

            convert_prev.push_back(tolowercase);
        }
        prev_token_value = convert_prev;
    }

    //takes in the current token and converts it to lower case
    for(int i=0; i<length; i++)
    {
        tolowercase = temp_holder.at(i);
        tolowercase = std::tolower(tolowercase);

        converted_to_lower.push_back(tolowercase);
    }

    //search our current token if we find a quotation mark or apostrophe
    found_quot = converted_to_lower.find_first_of(quot);
    found_apos = converted_to_lower.find_first_of(apos);

    //special loop to check if our token contains only numbers
    for(int i=0; i<length; i++)
    {
        numeric = converted_to_lower.at(i);
        if(std::isdigit(numeric))
        {
            numeric_token = true;
        }
        else
        {
            numeric_token = false;
        }
    }

    //checks to see if our token satisfies the three conditions of an identifier
    //1.Does it start with a letter
    //2.Does it contain anything that cannot be part of an identifier
    //sets identifier_token to true if our current token is an identifier
    if(!converted_to_lower.empty())
    {
        first_char = converted_to_lower.at(0);
        if(std::isalpha(first_char))
        {
            for(int i=0; i<length-1; i++)
            {
                temp_char_holder_identifier = converted_to_lower.substr(i,1);
                array_search_identifier = std::find(identifiers, identifiers+37, temp_char_holder_identifier);

                if(array_search_identifier == identifiers+37)
                {
                    identifier_token = false;
                }
                else
                {
                    identifier_token = true;
                }
            }
        }
        else
        {
            identifier_token = false;
        }
    }

    //search and find if our current token is a keyword
    array_search_keyword = std::find(keyword, keyword+97, converted_to_lower);
    if(array_search_keyword != keyword+97)
    {
        token->setKeyword();
    }

    //search if our token is an operator
    array_search_operator = std::find(operators, operators+30, converted_to_lower);

    //List of ordered conditional statements for setting the token type
    //calls setSignalandVariableDetails if our token contains any specific tokenDetails
    if(array_search_operator != operators+30)
    {
        token->setTokenType(T_Operator);
    }
    else if(prev_token_value == comment2)
    {
        token->setTokenType(T_CommentBody);
    }
    else if(prev_token_value.compare(signal_1) == 0 && converted_to_lower.compare(signal_1) != 0)
    {
        token->setTokenType(T_Identifier);

        if(check_details_ptr == nullptr)
        {
            setSignalandVariableDetails(token);
        }
    }
    else if(prev_token_value.compare(variable_1) == 0 && converted_to_lower.compare(variable_1) != 0)
    {
        token->setTokenType(T_Identifier);

        if(check_details_ptr == nullptr)
        {
            setSignalandVariableDetails(token);
        }
    }
    else if(found_apos != string::npos)
    {
        found_apos2 = converted_to_lower.find_first_of(apos, found_apos);

        if(found_apos2 != string::npos)
        {
            token->setTokenType(T_Literal);
            token->setTokenDetails("std_logic");
        }
        else
        {
            token->setTokenType(T_Other);
        }
    }
    else if(numeric_token == true)
    {
        token->setTokenType(T_Literal);
        token->setTokenDetails("integer");
    }
    else if(found_quot != string::npos)
    {
        first_value = converted_to_lower.substr(0,1);
        token->setTokenType(T_Literal);

        if(first_value == binary)
        {
            token->setTokenDetails("std_logic_vector", (length-3));
        }
        else if(first_value == oct_1)
        {
            token->setTokenDetails("std_logic_vector", ((length-3)*3));
        }
        else if(first_value == hex_1)
        {
            token->setTokenDetails("std_logic_vector", ((length-3)*4));
        }
        else
        {
            token->setTokenDetails("std_logic_vector", (length-2));
        }
    }
    else if(converted_to_lower.compare(bool_1) == 0)
    {
        token->setTokenType(T_Literal);
        token->setTokenDetails("boolean");
    }
    else if(converted_to_lower.compare(bool_2) == 0)
    {
        token->setTokenType(T_Literal);
        token->setTokenDetails("boolean");
    }
    else if(identifier_token == true)
    {
        token->setTokenType(T_Identifier);
    }
    else //If it does not match any of the above conditions, it is of type other
    {
        token->setTokenType(T_Other);
    }
}

//gets called whenever we find a token that contains tokenDetails
//sets the type and width for the specific token
void TokenList::setSignalandVariableDetails(Token *token)
{
    Token *type_ptr;
    Token *iter_ptr;
    Token *prev_ptr;
    Token *after_ptr;
    tokenDetails *details_ptr;
    string temp_holder ="";
    string vector_check_value ="";  //checks if we have downto or to
    string first_vec_value ="";     //first number from vector initialization
    string second_vec_value ="";    //second number from vector initialization
    string type_value ="";
    string converted_to_lower ="";
    string type_holder ="";
    string after_holder ="";
    char tolowercase;
    int i = 0;
    int length = 0;
    int width_holder = 0;
    int first_int_value = 0;
    int second_int_value = 0;

    type_ptr = token;
    temp_holder = type_ptr->getStringRep();

    while(temp_holder.compare(semicolon) != 0)
    {
        i++;
        converted_to_lower = "";
        length = temp_holder.length();

        for(int j=0; j<length;j++)
        {
            tolowercase = temp_holder.at(j);
            tolowercase = std::tolower(tolowercase);

            converted_to_lower.push_back(tolowercase);
        }

        if(i == 3)
        {
            type_value = type_ptr->getStringRep();
        }

        if(converted_to_lower.compare(vec_1) == 0)
        {
            vector_check_value = temp_holder;
            prev_ptr = type_ptr->getPrev();
            after_ptr = type_ptr->getNext();

            first_vec_value = prev_ptr->getStringRep();
            second_vec_value = after_ptr->getStringRep();
        }
        else if(converted_to_lower.compare(vec_2) == 0)
        {
            vector_check_value = temp_holder;
            prev_ptr = type_ptr->getPrev();
            after_ptr = type_ptr->getNext();

            first_vec_value = prev_ptr->getStringRep();
            second_vec_value = after_ptr->getStringRep();
        }


        type_ptr = type_ptr->getNext();
        temp_holder = type_ptr->getStringRep();
    }

    if(!first_vec_value.empty() && !second_vec_value.empty())
    {
        first_int_value = std::stoi(first_vec_value,nullptr,0);
        second_int_value = std::stoi(second_vec_value,nullptr,0);
    }


    if(vector_check_value.compare(vec_1) == 0)
    {
        token->setTokenDetails("std_logic_vector", ((first_int_value - second_int_value)+1));
    }
    else if(vector_check_value.compare(vec_2) == 0)
    {
        token->setTokenDetails("std_logic_vector", ((second_int_value - first_int_value)+1));
    }
    else //not a vector type
    {
        token->setTokenDetails(type_value);
    }

    details_ptr = token->getTokenDetails();
    type_holder = details_ptr->type;
    width_holder = details_ptr->width;
    temp_holder = token->getStringRep();
    iter_ptr = token->getNext();

    while(iter_ptr != nullptr)
    {
        after_holder = iter_ptr->getStringRep();

        if(after_holder.compare(temp_holder) == 0)
        {
            iter_ptr->setTokenDetails(type_holder,width_holder);
        }

        iter_ptr = iter_ptr->getNext();
    }
}


//Complete the implementation of the following member functions:
//****Tokenizer class function definitions******

//Computes a new tokenLength for the next token
//Modifies: size_t tokenLength, and bool complete
//(Optionally): may modify offset
//Does NOT modify any other member variable of Tokenizer
void Tokenizer::prepareNextToken()
{
    string temp_holder = *str;
    string token_value;
    string offset_value;
    string offset_value2;
    string offset_value3;
    string check_delimiter_neighbour_right;
    string check_delimiter_neighbour_left;
    size_t offset2;
    size_t offset3;

    offset = temp_holder.find_first_of(delimiter1);
    offset2 = temp_holder.find_first_of(delimiter1, offset+1);
    offset3 = temp_holder.find_first_of(apos, offset+1);


    if(offset != string::npos)
    {
        offset_value = temp_holder.substr(offset,1);

        if(offset2 != string::npos)
        {
            offset_value2 = temp_holder.substr(offset2,1);
        }
        else
        {
            offset_value2 = "";
        }

        if(offset3 != string::npos)
        {
            offset_value3 = temp_holder.substr(offset3,1);
        }
        else
        {
            offset_value3 = "";
        }

        if(!(temp_holder.empty()))
        {
            complete = false;

            check_delimiter_neighbour_right = temp_holder.substr(offset+1, 1);

            if(offset != 0)
            {
                check_delimiter_neighbour_left = temp_holder.substr(offset-1,1);

                if(offset_value == apos)
                {
                    token_value = temp_holder.substr(0,offset);
                }
                else if((offset_value == quot) && (offset_value2 == quot))
                {
                    if(before_quot.find(check_delimiter_neighbour_left) != string::npos)
                    {
                        token_value = temp_holder.substr(offset-1,offset2+1);
                    }
                    else
                    {
                        token_value = temp_holder.substr(0,offset);
                    }
                }
                else if(offset_value == tab)
                {
                    token_value = temp_holder.substr(0,offset);
                }
                else if(offset_value == whitespace)
                {
                    token_value = temp_holder.substr(0,offset);
                }
                else
                {
                    token_value = temp_holder.substr(0,offset);
                }
            }
            else if(offset == 0)
            {
                if((delimiter2_head.find(offset_value) != string::npos) && (delimiter2_end.find(check_delimiter_neighbour_right) != string::npos))
                {
                    token_value = temp_holder.substr(0,2);
                }
                else if(offset_value == tab)
                {
                    temp_holder.erase(0,1);
                    token_value = "";
                }
                else if(offset_value == whitespace)
                {
                    temp_holder.erase(0,1);
                    token_value = "";
                }
                else if(offset_value == quot && offset_value2 == quot)
                {
                    token_value = temp_holder.substr(0,offset2+1);
                }
                else if(offset_value == apos)
                {
                    if((offset3-offset) == 2 && offset_value3 == apos)
                    {
                        token_value = temp_holder.substr(0,offset3+1);
                    }
                    else
                    {
                        token_value = temp_holder.substr(0,1);
                    }
                }
                else if((offset_value == comments) && (check_delimiter_neighbour_right == comments))
                {
                    token_value = temp_holder.substr(0,2);
                }
                else
                {
                    token_value = temp_holder.substr(0,1);
                }
            }
        }
        else
        {
            complete = true;
        }
    }
    else
    {
        token_value = temp_holder.substr(0);
    }

    *str = temp_holder;
    tokenLength = token_value.length();
}

void Tokenizer::processComments()
{
    string temp_holder = *str;

    tokenLength = temp_holder.length();
}

//Sets the current string to be tokenized
//Resets all Tokenizer state variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
void Tokenizer::setString(string *str)
{
    this->str = str;
    offset = 0;
    tokenLength = 0;

    prepareNextToken();
}

//Returns the next token. Hint: consider the substr function
//Updates the tokenizer state
//Updates offset, resets tokenLength, updates processingABC member variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
string Tokenizer::getNextToken()
{
    string token_value;
    string temp_holder;

    while(tokenLength == 0 && !isComplete())
    {
        prepareNextToken();
    }

    temp_holder = *str;

    token_value = temp_holder.substr(0,tokenLength);
    temp_holder.erase(0,tokenLength);
    *str = temp_holder;

    if(token_value == comment2)
    {
        processComments();
    }
    else
    {
        prepareNextToken();
    }

    return token_value;
}



//****Challenge Task Functions******

//Removes the token from the linked list if it is not null
//Deletes the token
//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
void TokenList::deleteToken(Token *token)
{
    if(head != NULL)
    {
        if(token == head)
        {
            head = token->getNext();

            head->prev = nullptr;
            token->next = nullptr;
        }
        else if(token == tail)
        {
            tail = token->getPrev();

            tail->next = nullptr;
            token->prev = nullptr;
        }
        else if(token == head && token == tail)
        {
            head = nullptr;
            tail = nullptr;
        }
        else
        {
            (token->prev)->next = token->next;
            (token->next)->prev = token->prev;
            token->next = nullptr;
            token->prev = nullptr;
        }
    }
    delete token;
}

//Removes all comments from the tokenList including the -- marker
//Returns the number of comments removed
int removeComments(TokenList &tokenList)
{
    Token *temp_ptr = tokenList.getFirst();
    Token *delete_ptr = nullptr;
    Token *delete_ptr2 = nullptr;
    string temp_holder;
    int count;

    while(temp_ptr != NULL)
    {
        temp_holder = temp_ptr->getStringRep();

        if(temp_holder == comment2)
        {
            count++;

            delete_ptr = temp_ptr;
            temp_ptr = temp_ptr->getNext();

            delete_ptr2 = temp_ptr;
            temp_ptr = temp_ptr->getNext();

            tokenList.deleteToken(delete_ptr);
            tokenList.deleteToken(delete_ptr2);
        }
        else
        {
            temp_ptr = temp_ptr->getNext();
        }
    }
    return count;
}

//Removes all tokens if it matches our type argument
//Returns the number of tokens removed that match the specific type
int removeTokensOfType(TokenList &tokenList, tokenType type)
{
    Token *temp_ptr = tokenList.getFirst();
    Token *delete_ptr = nullptr;
    tokenType temp_type;
    int count = 0;

    while(temp_ptr != nullptr)
    {
        temp_type = temp_ptr->getTokenType();

        if(temp_type == type)
        {
            count++;

            delete_ptr = temp_ptr;
            temp_ptr = temp_ptr->getNext();

            tokenList.deleteToken(delete_ptr);
        }
        else
        {
            temp_ptr = temp_ptr->getNext();
        }
    }

    return count;
}

//Creates a new TokenList, and returns a pointer to this list
//Searches for all conditional expressions in tokenList and appends them to the new list
//Format is as follows:
//Each token that is part of a condtional expression is appended sequentially
//At the end of a conditional expression a newline character is appened
   //Example: if (a = true) then
   //Your list should include "(", "a", "=", "true", ")" and "\n"
//tokenList is NOT modified
TokenList* findAllConditionalExpressions(const TokenList &tokenList)
{
    string temp_holder ="";
    string prev_token = "";
    string after_token = "";
    string convert_prev = "";
    string convert_after = "";
    string convert_current = "";
    Token *temp_ptr = nullptr;
    Token *prev_ptr = nullptr;
    Token *after_ptr = nullptr;
    Token *head = nullptr;
    TokenList *temp_list_ptr = new TokenList;
    char tolowercase;
    int length = 0;

    head = tokenList.getFirst();
    temp_ptr = head;

    //search through the entire token list
    while(temp_ptr != nullptr)
    {
        convert_prev = "";
        convert_current = "";

        if(temp_ptr != head)  //get our previous token
        {
            prev_ptr = temp_ptr->getPrev();
            prev_token = prev_ptr->getStringRep();
        }

        if(temp_ptr != tokenList.getLast())   //get the next token
        {
            after_ptr = temp_ptr->getNext();
            after_token = after_ptr->getStringRep();
        }

        temp_holder = temp_ptr->getStringRep();    //get our current token string

        if(!prev_token.empty())      //convert our previous token all to lower case
        {
            length = prev_token.length();

            for(int i=0; i<length; i++)
            {
                tolowercase = prev_token.at(i);
                tolowercase = tolower(tolowercase);

                convert_prev.push_back(tolowercase);
            }
        }

        if(!after_token.empty())     //convert our next token all to lowercase
        {
            length = after_token.length();

            for(int i=0; i<length; i++)
            {
                tolowercase = after_token.at(i);
                tolowercase = tolower(tolowercase);

                convert_after.push_back(tolowercase);
            }
        }

        if(!temp_holder.empty())   //converts our current token all to lowercase
        {
            length = temp_holder.length();


            for(int i=0; i<length; i++) //converts current token to lower case
            {
                tolowercase = temp_holder.at(i);
                tolowercase = tolower(tolowercase);

                convert_current.push_back(tolowercase);
            }
        }


        //find any token that contains "if" by comparing the current token and the previous token
        if(convert_current.compare(if_string) == 0 && convert_prev.compare(end_string) != 0)
        {
            while(convert_after.compare(then_string) != 0)    //starting at where the "if" is, append all tokens until we reach a "then" token
            {
                convert_after = "";
                temp_ptr = temp_ptr->getNext();
                temp_holder = temp_ptr->getStringRep();

                temp_list_ptr->append(temp_holder);

                if(temp_ptr->getNext() != nullptr)
                {
                    after_ptr = temp_ptr->getNext();
                    after_token = after_ptr->getStringRep();
                }

                if(!after_token.empty())
                {
                    length = after_token.length();
                    for(int i=0; i<length; i++) //converts current token to lower case
                    {
                        tolowercase = after_token.at(i);
                        tolowercase = tolower(tolowercase);

                        convert_after.push_back(tolowercase);
                    }
                }

            }

            temp_list_ptr->append("\n");
        }
        else if(convert_current.compare(elsif_string) == 0)      //find all elsif tokens
        {
            while(convert_after.compare(then_string) != 0)    //starting at where the elsif is, append all tokens until we reach a "then" token
            {
                convert_after = "";
                temp_ptr = temp_ptr->getNext();
                temp_holder = temp_ptr->getStringRep();

                temp_list_ptr->append(temp_holder);

                if(temp_ptr->getNext() != nullptr)
                {
                    after_ptr = temp_ptr->getNext();
                    after_token = after_ptr->getStringRep();
                }

                if(!after_token.empty())
                {
                    length = after_token.length();
                    for(int i=0; i<length; i++) //converts current token to lower case
                    {
                        tolowercase = after_token.at(i);
                        tolowercase = tolower(tolowercase);

                        convert_after.push_back(tolowercase);
                    }
                }

            }

            temp_list_ptr->append("\n");
        }
        else if(convert_prev.compare(when_string) == 0)         //find if our current token is equal to "when"
        {
            while(convert_after.compare(else_string) != 0)     //starting at where the "when" is, append all tokens until we reach a "then" token
            {
                convert_after = "";
                temp_list_ptr->append(temp_holder);
                temp_ptr = temp_ptr->getNext();

                temp_holder = temp_ptr->getStringRep();

                if(temp_ptr->getNext() != nullptr)
                {
                    after_ptr = temp_ptr->getNext();
                    after_token = after_ptr->getStringRep();
                }

                if(!after_token.empty())
                {
                    length = after_token.length();
                    for(int i=0; i<length; i++) //converts current token to lower case
                    {
                        tolowercase = after_token.at(i);
                        tolowercase = tolower(tolowercase);

                        convert_after.push_back(tolowercase);
                    }
                }
            }

            temp_list_ptr->append("\n");
        }
        else
        {
            temp_ptr = temp_ptr->getNext();
        }
    }

    return temp_list_ptr;
}

//Function used to check for any missing "then" and check for type/width mismatches
//Outputs the number of type and width mismatches as well as the number of missing then
//Outputs the entire line where the error occurs
void checkForMissingThenandMismatch(const TokenList &tokenList)
{
    string comparison[6] = {"=","<",">","/=","<=",">="};
    bool found_then = false;
    Token *temp_ptr = tokenList.getFirst();
    Token *prev_ptr = nullptr;
    Token *stationary_ptr = nullptr;
    Token *output_ptr = nullptr; ////////////////////////////
    Token *reset_ptr = nullptr;  /////////////////////////////
    Token *comparison_prev_ptr = nullptr; ///////////////////////////
    Token *comparison_after_ptr = nullptr;      ////////////////////////
    tokenDetails *check_details_ptr_prev = nullptr; ///////////////////////////
    tokenDetails *check_details_ptr_after = nullptr; /////////////////////////
    string stationary_holder = "";
    string convert_current ="";
    string convert_prev ="";
    string prev_token ="";
    string current_string_holder ="";
    string details_type_holder_prev =""; //////////////////////////////
    string details_type_holder_after =""; ////////////////////////////
    string *comparison_operator_identifier = nullptr; ////////////////////////
    char tolowercase;
    int details_width_holder_prev = 0; ////////////////////////////////////
    int details_width_holder_after = 0;///////////////////
    int missing_then_counter = 0;
    int number_of_conditional_express = 0;
    int number_of_mismatch_type = 0;    /////////////////////////////////
    int number_of_mismatch_width = 0;   ///////////////////////////////////
    int length = 0;


    while(temp_ptr != nullptr)
    {
        convert_prev = "";
        convert_current = "";
        found_then = false;
        current_string_holder = temp_ptr->getStringRep();

        if(temp_ptr != tokenList.getFirst())
        {
            prev_ptr = temp_ptr->getPrev();
            prev_token = prev_ptr->getStringRep();
        }

        if(!prev_token.empty())
        {
            length = prev_token.length();

            for(int i=0; i<length; i++) //converts previous token to lower case
            {
                tolowercase = prev_token.at(i);
                tolowercase = tolower(tolowercase);

                convert_prev.push_back(tolowercase);
            }
        }

        if(!current_string_holder.empty())            //converts first string to lower case
        {
            length = current_string_holder.length();


            for(int i=0; i<length; i++) //converts current token to lower case
            {
                tolowercase = current_string_holder.at(i);
                tolowercase = tolower(tolowercase);

                convert_current.push_back(tolowercase);
            }
        }

        if((convert_current.compare(if_string) == 0 || convert_current.compare(elsif_string) == 0) && convert_prev.compare(end_string) != 0)
        {
            stationary_ptr = temp_ptr;

            while(convert_current.compare(semicolon) != 0)     //iterate starting from the "if" to the closest semi-colon and search for a "then"
            {
                convert_current = "";
                temp_ptr = temp_ptr->getNext();
                current_string_holder = temp_ptr->getStringRep();

                length = current_string_holder.length();
                for(int i=0; i<length; i++) //converts current token to lower case
                {
                    tolowercase = current_string_holder.at(i);
                    tolowercase = tolower(tolowercase);

                    convert_current.push_back(tolowercase);
                }

                if(convert_current.compare(then_string) == 0)
                {
                    found_then = true;                //if we successfully find a "then" token
                    number_of_conditional_express++;
                    break;
                }
            }

            if(found_then == false)             //outputs the error line and increment our counter for missing "then"
            {
                missing_then_counter++;
                stationary_holder = stationary_ptr->getStringRep();

                while(stationary_holder.compare(newline) != 0)
                {
                    cout << stationary_holder << " ";
                    stationary_ptr = stationary_ptr->getNext();
                    stationary_holder = stationary_ptr->getStringRep();

                    //if(stationary_holder.compare(semicolon) == 0)
                    //{
                       // cout << stationary_holder << " ";
                    //}
                }
                cout << "-- missing \"then\" in conditional statement" << endl << endl;
            }
            else if(found_then == true)             //search our entire conditional statement for any type or width mismatches
            {
                output_ptr = stationary_ptr;       //pointer to where the "if" token is
                reset_ptr = stationary_ptr;        //pointer to reset our output pointer back to the "if" token

                while(stationary_ptr != temp_ptr)
                {
                    stationary_holder = stationary_ptr->getStringRep();

                    comparison_operator_identifier = std::find(comparison, comparison+6, stationary_holder);

                    if(comparison_operator_identifier != comparison+6)
                    {
                        comparison_prev_ptr = stationary_ptr->getPrev();
                        comparison_after_ptr = stationary_ptr->getNext();

                        if(comparison_prev_ptr != nullptr)
                        {
                            check_details_ptr_prev = comparison_prev_ptr->getTokenDetails();
                        }

                        if(comparison_after_ptr != nullptr)
                        {
                            check_details_ptr_after = comparison_after_ptr->getTokenDetails();
                        }

                        if(check_details_ptr_prev != nullptr && check_details_ptr_after != nullptr)
                        {
                            details_type_holder_prev = check_details_ptr_prev->type;
                            details_width_holder_prev = check_details_ptr_prev->width;

                            details_type_holder_after = check_details_ptr_after->type;
                            details_width_holder_after = check_details_ptr_after->width;

                            if(details_type_holder_after.compare(details_type_holder_prev) != 0)
                            {
                                number_of_mismatch_type++;
                                if(details_width_holder_after != 0 && details_width_holder_prev != 0)
                                {
                                    if(details_width_holder_after != details_width_holder_prev)
                                    {
                                        number_of_mismatch_width++;
                                    }
                                }

                                while(output_ptr != temp_ptr)
                                {
                                    current_string_holder = output_ptr->getStringRep();

                                    cout << current_string_holder << " ";
                                    output_ptr = output_ptr->getNext();
                                    current_string_holder = output_ptr->getStringRep();
                                }

                                output_ptr = reset_ptr;

                                cout << "-- type mismatch" << endl << endl;
                            }
                            else
                            {
                                if(details_width_holder_after != 0 && details_width_holder_prev != 0)
                                {
                                    if(details_width_holder_after != details_width_holder_prev)
                                    {
                                        number_of_mismatch_width++;

                                        while(output_ptr != temp_ptr)
                                        {
                                            current_string_holder = output_ptr->getStringRep();

                                            cout << current_string_holder << " ";
                                            output_ptr = output_ptr->getNext();
                                            current_string_holder = output_ptr->getStringRep();
                                        }
                                        output_ptr = reset_ptr;

                                        cout << "-- width mismatch" << endl << endl;
                                    }
                                }
                            }

                        }
                        else if(check_details_ptr_prev != nullptr && check_details_ptr_after == nullptr)
                        {
                            number_of_mismatch_type++;
                            number_of_mismatch_width++;

                            while(output_ptr != temp_ptr)
                            {
                                current_string_holder = output_ptr->getStringRep();

                                cout << current_string_holder << " ";
                                output_ptr = output_ptr->getNext();
                                current_string_holder = output_ptr->getStringRep();
                            }
                            output_ptr = reset_ptr;

                            cout << "-- type mismatch" << endl << endl;
                        }
                        else if(check_details_ptr_prev == nullptr && check_details_ptr_after != nullptr)
                        {
                            number_of_mismatch_type++;
                            number_of_mismatch_width++;

                            while(output_ptr != temp_ptr)
                            {
                                current_string_holder = output_ptr->getStringRep();

                                cout << current_string_holder << " ";
                                output_ptr = output_ptr->getNext();
                                current_string_holder = output_ptr->getStringRep();
                            }
                            output_ptr = reset_ptr;

                            cout << "-- type mismatch" << endl << endl;
                        }
                    }

                    stationary_ptr = stationary_ptr->getNext();
                }
            }
        }
        else if(convert_current.compare(when_string) == 0)
        {
            number_of_conditional_express++;
        }
        else
        {
            temp_ptr = temp_ptr->getNext();
        }
    }

    cout << endl;
    cout << "The number of missing \"then\" : " << missing_then_counter << endl;
    cout << "The number of conditional expressions : " << number_of_conditional_express << endl;
    cout << "The number of mismatching types : " << number_of_mismatch_type << endl;
    cout << "The number of mismatching width : " << number_of_mismatch_width << endl;
}

//Function used only to count the number of tokens in our list
//Does not count any newline tokens used for part 3
void numberOftokens(const TokenList &tokenList)
{
    Token *temp_ptr = tokenList.getFirst();
    string temp_holder = "";
    int token_count = 0;

    while(temp_ptr != nullptr)
    {
        temp_holder = temp_ptr->getStringRep();
        if(temp_holder.compare(newline) != 0)
        {
            token_count++;
        }

        temp_ptr = temp_ptr->getNext();
    }

    cout << endl;
    cout << "The total number of tokens in the list is: " << token_count << endl;
}


void checkForMissingendif(const TokenList &tokenList)
{
    int number_of_if = 0;
    int number_of_end_if = 0;
    int missing_num_of_endif = 0;
    int length = 0;
    char tolowercase;
    string prev_token = "";
    string current_token = "";
    string convert_current = "";
    string convert_prev ="";
    string stationary_holder ="";
    Token *temp_ptr = nullptr;
    Token *prev_ptr = nullptr;
    Token *stationary_ptr = nullptr;

    temp_ptr = tokenList.getFirst();
    while(temp_ptr != nullptr)
    {
        convert_current ="";
        convert_prev ="";
        stationary_ptr = temp_ptr;
        current_token = temp_ptr->getStringRep();
        if(temp_ptr != tokenList.getFirst())
        {
            prev_ptr = temp_ptr->getPrev();
            prev_token = prev_ptr->getStringRep();
        }

        if(!prev_token.empty())
        {
            length = prev_token.length();
            for(int i=0; i<length; i++) //converts current token to lower case
            {
                tolowercase = prev_token.at(i);
                tolowercase = tolower(tolowercase);

                convert_prev.push_back(tolowercase);
            }
        }

        if(!current_token.empty())
        {
            length = current_token.length();
            for(int i=0; i<length; i++) //converts current token to lower case
            {
                tolowercase = current_token.at(i);
                tolowercase = tolower(tolowercase);

                convert_current.push_back(tolowercase);
            }
        }


        if(convert_current.compare(if_string) == 0)
        {
            if(convert_prev.compare(end_string) == 0)
            {
                number_of_end_if++;
            }
            else
            {
                number_of_if++;
            }
        }

        temp_ptr = temp_ptr->getNext();
    }

    missing_num_of_endif = number_of_if - number_of_end_if;

    cout << endl;
    cout << "The number of missing end if : " << missing_num_of_endif << endl << endl;

    if(missing_num_of_endif != 0)
    {
        temp_ptr = tokenList.getFirst();

        while(temp_ptr != nullptr)
        {
            convert_current ="";
            convert_prev ="";
            stationary_ptr = temp_ptr;
            stationary_holder = stationary_ptr->getStringRep();
            current_token = temp_ptr->getStringRep();

            if(temp_ptr != tokenList.getFirst())
            {
            prev_ptr = temp_ptr->getPrev();
            prev_token = prev_ptr->getStringRep();
            }

            if(!prev_token.empty())
            {
                length = prev_token.length();
                for(int i=0; i<length; i++) //converts current token to lower case
                {
                    tolowercase = prev_token.at(i);
                    tolowercase = tolower(tolowercase);

                    convert_prev.push_back(tolowercase);
                }
            }

            if(!current_token.empty())
            {
                length = current_token.length();
                for(int i=0; i<length; i++) //converts current token to lower case
                {
                    tolowercase = current_token.at(i);
                    tolowercase = tolower(tolowercase);

                    convert_current.push_back(tolowercase);
                }
            }


            if(convert_current.compare(if_string) == 0 && convert_prev.compare(end_string) != 0)
            {
                temp_ptr = temp_ptr->getNext();
                while(temp_ptr != nullptr)
                {
                    convert_current ="";
                    convert_prev ="";
                    current_token = temp_ptr->getStringRep();

                    if(temp_ptr != tokenList.getFirst())
                    {
                        prev_ptr = temp_ptr->getPrev();
                        prev_token = prev_ptr->getStringRep();
                    }

                    if(!prev_token.empty())
                    {
                        length = prev_token.length();
                        for(int i=0; i<length; i++) //converts current token to lower case
                        {
                            tolowercase = prev_token.at(i);
                            tolowercase = tolower(tolowercase);

                            convert_prev.push_back(tolowercase);
                        }
                    }

                    if(!current_token.empty())
                    {
                        length = current_token.length();
                        for(int i=0; i<length; i++) //converts current token to lower case
                        {
                            tolowercase = current_token.at(i);
                            tolowercase = tolower(tolowercase);

                            convert_current.push_back(tolowercase);
                        }
                    }

                    if(convert_current.compare(if_string) == 0 && convert_prev.compare(end_string) != 0)
                    {
                        nestedIf(stationary_ptr);
                        break;
                    }
                    else if(convert_current.compare(if_string) == 0 && convert_prev.compare(end_string) == 0)
                    {
                        break;
                    }
                    else if(temp_ptr == tokenList.getLast())
                    {
                        while(stationary_holder.compare(newline) != 0)
                        {
                            cout << stationary_holder << " ";
                            stationary_ptr = stationary_ptr->getNext();
                            stationary_holder = stationary_ptr->getStringRep();
                        }

                        cout << "-- missing end if" << endl;

                        break;
                    }

                    temp_ptr = temp_ptr->getNext();
                }
            }

            temp_ptr = temp_ptr->getNext();
        }

    }
}

void nestedIf(Token *token)
{
    Token *temp_ptr = nullptr;
    Token *prev_ptr = nullptr;
    Token *stationary_ptr = nullptr;
    string temp_holder ="";
    string prev_token ="";
    string current_token ="";
    string convert_current ="";
    string convert_prev ="";
    string stationary_holder ="";
    int length = 0;
    int number_of_end_if = 0;
    int number_of_if = 0;
    int missing_num_of_endif = 0;
    int i = 0;
    char tolowercase;

    temp_ptr = token;
    while(temp_ptr != nullptr)
    {
        convert_current ="";
        convert_prev ="";
        current_token = temp_ptr->getStringRep();
        if(temp_ptr->getPrev() != nullptr)
        {
            prev_ptr = temp_ptr->getPrev();
            prev_token = prev_ptr->getStringRep();
        }

        if(!prev_token.empty())
        {
            length = prev_token.length();
            for(int i=0; i<length; i++) //converts current token to lower case
            {
                tolowercase = prev_token.at(i);
                tolowercase = tolower(tolowercase);

                convert_prev.push_back(tolowercase);
            }
        }

        if(!current_token.empty())
        {
            length = current_token.length();
            for(int i=0; i<length; i++) //converts current token to lower case
            {
                tolowercase = current_token.at(i);
                tolowercase = tolower(tolowercase);

                convert_current.push_back(tolowercase);
            }
        }


        if(convert_current.compare(if_string) == 0)
        {
            if(convert_prev.compare(end_string) == 0)
            {
                number_of_end_if++;
            }
            else
            {
                number_of_if++;
            }
        }

        temp_ptr = temp_ptr->getNext();
    }

    missing_num_of_endif = number_of_if - number_of_end_if;
    stationary_ptr = token;

    while(i != missing_num_of_endif)
    {
        stationary_holder = stationary_ptr->getStringRep();
        if(stationary_holder.compare(if_string) == 0)
        {
            i++;

            while(stationary_holder.compare(newline) != 0)
            {
                cout << stationary_holder << " ";
                stationary_ptr = stationary_ptr->getNext();
                stationary_holder = stationary_ptr->getStringRep();
            }
            cout << "-- missing end if" << endl;
        }
        stationary_ptr = stationary_ptr->getNext();
    }
}



