/** Created by aqn
 ** License: CC-BY-SA
 **/

#include <cstdio>
#include "configuration_loader.h"

using namespace std;

inline std::string showAsBits(unsigned long num_as_int, size_t num_of_bits = sizeof (unsigned long),
        char zero_char = '.', char one_char = 'X'){
    std::string bits (num_of_bits, zero_char);
    for (int iBit = num_of_bits-1; iBit>=0 && num_as_int; iBit--){
        if (num_as_int & 1) bits[iBit] = one_char;
        num_as_int>>=2;
    }
    return bits;
}

int main(int argc, char** argv) {

    aqn::configuration_loader Set;
    //Setting.Open("Configuration_Non-exist.txt");
    Set.Open("Configuration.txt");
    printf ("List of parameters loaded:\n");
    for (int ipar = 0; ipar <Set.par_list().size(); ipar++){
        printf ("[%s], ", Set.par_list()[ipar].c_str());
    }
    printf ("\n\n");

    printf ("Flight Number = %s :  %s\n", Set ["FlightNo"].c_str(), showAsBits(Set.check()).c_str()); Set.reset();
    printf ("Captain\'s Name = %s :  %s\n", Set ["Captain\'s Name"].c_str(),showAsBits(Set.check()).c_str());Set.reset();
    printf ("First Officer\'s Name = %s :  %s\n", Set ["First Officer"].c_str(),showAsBits(Set.check()).c_str());Set.reset();
    printf ("Cabin Steward\'s Name = %s :  %s\n", Set ["Cabin Steward"].c_str(),showAsBits(Set.check()).c_str());Set.reset();
    printf ("Take-off Weight = %f : %s\n", Set.f("Take-off Weight"), showAsBits(Set.check()).c_str());Set.reset();
    printf ("Flaps Setting = %i : %s\n", Set.i("Flaps"), showAsBits(Set.check()).c_str());Set.reset();
    printf ("N1 = %.1f : %s\n", Set.f(string ("N1")),showAsBits(Set.check()).c_str());Set.reset();
    printf ("Passenger in 5A = %s : %s\n", Set[string ("Passenger in seat \"5A\"")].c_str(),showAsBits(Set.check()).c_str());Set.reset();
    printf ("Passenger in 5B = %s : %s\n", Set[string ("Passenger in seat 5B")].c_str(),showAsBits(Set.check()).c_str());Set.reset();
    
    return 0;
}

