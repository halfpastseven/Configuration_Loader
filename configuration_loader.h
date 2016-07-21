/* 
 * File:   Configuration_Loader.h
 * Author: AQN
 *
 * Created on June 15, 2014, 5:04 PM
 * License: CC-BY-SA
 */

#ifndef CONFIGURATION_LOADER_H
#define	CONFIGURATION_LOADER_H

#include <sstream>
#include <vector>
#include <string>

namespace aqn{
    
class configuration_loader{
public:
    configuration_loader();
    ~configuration_loader();
    
	int Open(const char* Filename);
    int Open(const std::string& Filename); //Overloaded function

    inline unsigned check () const {return status_;}
    inline void reset () {status_=0;} //Reset the flags
    inline const std::vector <std::string>& par_list () const {return parameters_;} //Tell the list of parameters;

    std::string operator[] (const std::string& SearchTerm) const;
    inline std::string operator[] (const char* SearchTerm) const{return (*this) [std::string(SearchTerm)];}
    
    //Parsing functions
    inline int i(const std::string& SearchTerm) const {
        std::istringstream value_string ( (*this) [std::string(SearchTerm)]);
        int value_int;
        value_string>>value_int;
        return value_string.fail() ? DEADBEE : value_int;
    }
    
    inline long l(const std::string& SearchTerm) const {
        std::istringstream value_string ( (*this) [std::string(SearchTerm)]);
        long value_long;
        value_string>>value_long;
        return value_string.fail() ? DEADBEE : value_long;
    }
    
    inline float f(const std::string& SearchTerm) const {
        std::istringstream value_string ((*this) [std::string(SearchTerm)]);
        float value_float;
        value_string>>value_float;
        return value_string.fail() ? DEADBEE : value_float;
    }
    
    inline double d(const std::string& SearchTerm) const {
        std::istringstream value_string ((*this) [std::string(SearchTerm)]);
        double value_double;
        value_string>>value_double;
        return value_string.fail() ? DEADBEE : value_double;
    }
    
    //Overloaded functions for C-style string inputs
    inline int    i (const char* SearchTerm) const {return i (std::string(SearchTerm));}
    inline long   l (const char* SearchTerm) const {return l (std::string(SearchTerm));}
    inline float  f (const char* SearchTerm) const {return f (std::string(SearchTerm));}
    inline double d (const char* SearchTerm) const {return d (std::string(SearchTerm));}
    
private:
	unsigned status_;
    static const char Search_Fail_[];// = "not_found";
    static const int DEADBEE = 0xDEADBEE; //This is a large int unlikely to occur, so it's used as a fail value

    enum error{
    //Error: A line cannot be processed because there is a failure.
    //Warning: A line can be processed but something odd was encountered.
		FileError=0x1,
		ParameterError =0x2,
		//ParseError = 0x4, //Deprecated
        ParameterWarning = 0x8,
		EscapeWarning = 0x10
	};
    std::vector<std::string> parameters_;
    std::vector<std::string> values_;
}; //closing class configuration_loader

} //closing namespace aqn

#endif	/* CONFIGURATION_LOADER_H */

