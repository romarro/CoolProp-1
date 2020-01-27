
#include <vector>
#include <string>
#include <chrono>

#include "CoolProp.h"

int main(int argc, const char* argv[]) {

    double T_ref = 298.15;
    double rho_ref = 1.8079703036340105;
    double m_mass = CoolProp::Props1SI("molar_mass", "HEOS::CarbonDioxide");

    CoolProp::set_reference_stateD("R744", T_ref, rho_ref / m_mass, 0.0, 0.0);

    while (true) {
        double s_ref = CoolProp::PropsSI("Smass", "Dmass", rho_ref, "T", T_ref, "HEOS::CarbonDioxide");
    }

    CoolProp::set_debug_level(1000);
    double p1 = CoolProp::Props1SI("pmin", "HEOS::R134a");
    double p2 = CoolProp::Props1SI("pmin", "REFPROP::R134a");
    double p3 = CoolProp::Props1SI("pmin", "REFPROP::R449A.MIX");
    return 0;
}
