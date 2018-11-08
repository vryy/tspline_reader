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

    // Write control points
    TPointsetPtr tpoints = tspline->getTPointset();
//    WATCH(tpoints->size())
    mfile << "%% Control points" << std::endl;
    mfile << "P = zeros(" << tpoints->size() << ", 3);" << std::endl;
    i = 0;
    for (TObjVIterator pit = tpoints->iteratorBegin(); pit != tpoints->iteratorEnd(); ++pit)
    {
        ++i;
        TPointPtr paux = std::dynamic_pointer_cast<TPoint>(*pit);
        paux->setId(i);
        mfile << "P(" << i << ",:) = [";
        mfile << paux->getX() << " " << paux->getY() << " " << paux->getZ() << "];" << std::endl;
    }
    std::cout << "Write control points successfully." << std::endl;

    mfile.close();
    std::cout << "T-Spline is exported to " << outfilename << " successfully" << std::endl;

    return 0;
}

