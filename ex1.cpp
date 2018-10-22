#include <iostream>
#include <string>

#include "rhbuilder.h"

using namespace TSPLINE;

#define WATCH(a) std::cout << #a << ": " << a << std::endl;

int main(int argc, char** argv)
{
    std::string filename = "/home/hbui/workspace2/T-SPLINE/rhino/simple.tsm";

    RhBuilderPtr reader = makePtr<RhBuilder>(filename);
	TSplinePtr spline = reader->findTSpline();

    std::cout << "T-Spline is found in " << filename << std::endl;
    WATCH(spline->getId())
    WATCH(spline->getName())
    WATCH(spline->isVirtual())
    WATCH(spline->getPhysicalId())
    WATCH(spline->getSDegree())
    WATCH(spline->getTDegree())
    return 0;
}

