/** Created by aqn
 ** License: CC-BY-SA
 **/

#include "configuration_loader.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace aqn;

configuration_loader::configuration_loader():status_(0){}

configuration_loader::~configuration_loader(){
}

const char configuration_loader::Search_Fail_[] = "not_found" ;

inline bool isWhiteSpace (char c) {return c==' ' || c=='\t';}

int configuration_loader::Open(const char* Filename){
    status_ = 0; // Clear all the error flags
    parameters_.resize(0);
    values_.resize(0);
    std::ifstream Configure (Filename);
    
	if (Configure.fail()) {
        status_ |= FileError;
        return 1;
    }

    /** Start Reading the Configuration file **/
    
    const char EscapeSign = '\\';
    const char CommentSign = '#';
    const char Semicolon = ';';
    const char QuoteSign = '\"';
    const char AssignmentSign = '=';

    const int MaxLineSizeAllowed = 1024;

    /*** Read line-by-line ***/
    do{
        bool SkipLine = 0; //Signal to skip this line without processing
        std::string LineBuffer;
        getline(Configure, LineBuffer); //Extract a line

        if (LineBuffer.size()>MaxLineSizeAllowed) {
            status_ |= FileError;
            continue;
        }

        /**** Process the line ****/
        bool InQuote = 0; //Stores the status that the character is inside a pair of quotes
        bool Escaped = 0; //Stores the status that the character is immediately after the escape sign
        size_t  StartQuote = std::string::npos; //Stores the location of the beginning quotation sign
        size_t  EndQuote = std::string::npos;   //Stores the location of the ending quotation sign
        size_t  AssignmentFound = std::string::npos; //Stores the location of the assignment sign
        
        for (int iChar =0; iChar<LineBuffer.size(); iChar++){//Check character by character
            
            if (InQuote) { //Inside Quote
                if (Escaped) { //Pass the current character as-is
                        LineBuffer.erase (iChar-1,1);
                        Escaped = 0; //Deactivate the escape mode
                        if (LineBuffer[iChar]!= EscapeSign || LineBuffer[iChar]!=QuoteSign) status_ |= EscapeWarning;
                        //Warns that the escaped character is not a \ or " character.
                        iChar--;
                }
                else {   //Not under escape mode
                    if (LineBuffer[iChar] == EscapeSign){ //Meet an escape sign that is not escaped
                        Escaped = 1; // Activate the escape mode
                    }
                    else if (LineBuffer[iChar] == QuoteSign){ //Meet a quotation sign that is not escaped
                        InQuote = 0;
                        EndQuote = iChar;
                    }
                }               
            }

            else{ //Outside quote
                
                if (LineBuffer[iChar] == CommentSign || LineBuffer[iChar] == Semicolon) {
                    LineBuffer.erase(iChar);
                }
                else if (LineBuffer[iChar] == QuoteSign){
                    //If a quotation sign appears before the assignment sign, this line is skipped
                    if (AssignmentFound == std::string::npos) { 
                        SkipLine = 1;
                        break;
                    }
                    //If the quotation was closed before, then this is discarded together with the content after
                    if (EndQuote != std::string::npos) { 
                        status_ |=  ParameterError;
                        LineBuffer.erase(iChar); //Ignore any content from the second start of quote
                    }
                    else { // A quotation was not started before, and this one appears after the assignment sign
                        InQuote = 1;
                        StartQuote = iChar;
                    }
                }
                
                else if (LineBuffer[iChar] == AssignmentSign) {
                    if (AssignmentFound != std::string::npos) {
                        status_ |= ParameterError;
                        LineBuffer.erase(iChar);
                    }
                    else {
                        AssignmentFound = iChar;
                    }
                }
            }
        }
        
        if (SkipLine) continue;

        /**** Split the valid lines ****/
        if (AssignmentFound==0 || AssignmentFound == std::string::npos) {
            // The assignment sign was not found or located at the very front
            status_ |= ParameterError;
            continue;
        }

        if (InQuote){ //If a quote is not closed, a warning is given
            status_ |= ParameterWarning;
        }

        std::string par = LineBuffer.substr(0, AssignmentFound); //Cut out the parameter string
        while (par.size() && isWhiteSpace (par[0])) par.erase (0,1);
        while (par.size() && isWhiteSpace (par[par.size()-1])) par.erase (par.size()-1); //strip end space
        if (!par.size()){ //Nothing left of the parameter name after white space is stripped
            status_ |= ParameterError;
            continue;
        }
        
        //When inserting the parameter names, strip the white space first, if empty then skip the line;
        //When inserting the values, store in the vector first then strip the white space. Keep it even if empty.
        parameters_.push_back (par);

        //If the start quote is not found or located at the end, 
        //then the whole content after the assignment sign is taken as the value string
        int valBegin = (StartQuote==-1 || StartQuote==LineBuffer.size()-1) ? (AssignmentFound+1) : (StartQuote+1);
        
        values_.push_back(LineBuffer.substr(valBegin, EndQuote-valBegin));

        std::string& val = values_.back();
        
        while (val.size() && isWhiteSpace (val[0])) val.erase(val.begin());
        while (val.size() && isWhiteSpace (val[val.size()-1])) val.erase (val.size()-1); //strip end space
        if (!val.size()){ //Nothing left of the parameter name after white space is stripped
            status_ |= ParameterWarning;
            continue;
        }
    } while (!Configure.eof());
    
	return status_;
}

int configuration_loader::Open(const std::string& Filename){
    Open (Filename.c_str());
}

std::string configuration_loader::operator[] (const std::string& SearchTerm) const {
	size_t par_position;
	for (par_position=0; par_position<parameters_.size();par_position++){
		if( parameters_[par_position] == SearchTerm ) break;
	}
    return (par_position<parameters_.size()) ? values_[par_position] : std::string(Search_Fail_);  
}


