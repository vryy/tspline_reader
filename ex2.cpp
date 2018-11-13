#include <iostream>
#include <fstream>
#include <string>

#include "rhbuilder.h"
#include "extractor.h"

using namespace TSPLINE;

#define WATCH(a) std::cout << #a << ": " << a << std::endl;
#define WATCH2(n, a) for(int iiii=0;iiii<n;++iiii) std::cout << " "; std::cout << #a << ": " << a << std::endl;

int main(int argc, char** argv)
{
    std::string filename = std::string(argv[1]);
    std::size_t lastindex = filename.find_last_of(".");
    std::string rawname = filename.substr(0, lastindex);
    std::size_t i, j;

    RhBuilderPtr reader = makePtr<RhBuilder>(filename);
	TSplinePtr tspline = reader->findTSpline();

    std::cout << "T-Spline is found in " << filename << std::endl;

    std::ofstream mfile;
    std::string outfilename = rawname + ".m";
    mfile.open(outfilename.c_str());

    // Write control points and weights
    TPointsetPtr tpoints = tspline->getTPointset();
    // WATCH(tpoints->size())
    mfile << "%% Control points" << std::endl;
    mfile << "P = zeros(" << tpoints->size() << ", 3);" << std::endl;
    mfile << "%% Weight at control points" << std::endl;
    mfile << "W = zeros(1, " << tpoints->size() << ");" << std::endl;
    mfile << "%% knot vector in u- and v-direction" << std::endl;
    mfile << "Xi = {};" << std::endl;
    mfile << "Eta = {};" << std::endl;
    i = 0;
    for (TObjVIterator pit = tpoints->iteratorBegin(); pit != tpoints->iteratorEnd(); ++pit)
    {
        ++i;
        TPointPtr ptpoint = std::dynamic_pointer_cast<TPoint>(*pit);
        ptpoint->setId(i); // set id one time

        mfile << "%" << std::endl;
        mfile << "P(" << i << ",:) = [";
        mfile << ptpoint->getX() << " " << ptpoint->getY() << " " << ptpoint->getZ() << "];" << std::endl;

        mfile << "W(" << i << ") = " << ptpoint->getW() << ";" << std::endl;

        TNodeV4Ptr pnodev4 = std::dynamic_pointer_cast<TNodeV4>(ptpoint->getTNode());
        std::vector<Real> uknots, vknots;
        TExtractor::extractUVKnotsFromTNodeV4(pnodev4, uknots, vknots);

        mfile << "Xi{" << ptpoint->getId() << "} =" ;
        mfile << " [" ;
        for (int ii = 0; ii < uknots.size(); ++ii)
            mfile << " " << uknots[ii];
        mfile << " ] ;" << std::endl;

        mfile << "Eta{" << ptpoint->getId() << "} =" ;
        mfile << " [" ;
        for (int ii = 0; ii < vknots.size(); ++ii)
            mfile << " " << vknots[ii];
        mfile << " ] ;" << std::endl;
    }
    std::cout << "Write control points, weights and knot vectors successfully." << std::endl;

    // write order in u- and v-direction
    int p1 = tspline->getSDegree();
    int p2 = tspline->getTDegree();
    mfile << "%% the order in u- and v-direction as params" << std::endl;    
    mfile << "params.p1 = " << tspline->getSDegree() << std::endl;
    mfile << "params.p2 = " << tspline->getTDegree() << std::endl;
    std::cout << "Write params successfully." << std::endl;

    mfile.close();
    std::cout << "T-Spline is exported to " << outfilename << " successfully" << std::endl;

    return 0;
}

