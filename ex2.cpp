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
    i = 0;
    for (TObjVIterator pit = tpoints->iteratorBegin(); pit != tpoints->iteratorEnd(); ++pit)
    {
        ++i;
        TPointPtr paux = std::dynamic_pointer_cast<TPoint>(*pit);
        paux->setId(i); // set id one time
        mfile << "P(" << i << ",:) = [";
        mfile << paux->getX() << " " << paux->getY() << " " << paux->getZ() << "];" << std::endl;
        mfile << "W(" << i << ") = " << paux->getW() << ";" << std::endl;
    }
    std::cout << "Write control points and weights successfully." << std::endl;

    TImagePtr timage = tspline->getTImage();
    // WATCH(timage->sizeFaces())
    // Write knot vector in u-direction
    mfile << "%% knot vector in u- and v-direction" << std::endl;
    mfile << "Xi = {};" << std::endl;
    mfile << "Eta = {};" << std::endl;
    i = 0;
    for (TFacVIterator fit = timage->faceIteratorBegin(); fit != timage->faceIteratorEnd(); ++fit)
    {
        ++i;
        j = 0;
        for (TNodVIterator nit = (*fit)->blendingNodeIteratorBegin(); nit != (*fit)->blendingNodeIteratorEnd(); ++nit)
        {
            ++j;
            //WATCH2(3, (*nit)->getTVertex()->getId())

            TNodeV4Ptr pnodev4 = std::dynamic_pointer_cast<TNodeV4>(*nit);
            std::vector<Real> uknots, vknots;
            TExtractor::extractUVKnotsFromTNodeV4(pnodev4, uknots, vknots);

            mfile << "Xi{" << (*nit)->getTPoint()->getId() << "} =" ;
            mfile << " [" ;
            for (int ii = 0; ii < uknots.size(); ++ii)
                mfile << " " << uknots[ii] ;                
            mfile << " ] ;" << std::endl;

            mfile << "Eta{" << (*nit)->getTPoint()->getId() << "} =" ;
            mfile << " [" ;
            for (int ii = 0; ii < vknots.size(); ++ii)
                mfile << " " << vknots[ii] ;                
            mfile << " ] ;" << std::endl;
        }
    }
    std::cout << "Write knot vectors successfully." << std::endl;

    // write order in u- and v-direction
    int p1 = tspline->getSDegree();
    int p2 = tspline->getTDegree();
    mfile << "%% the order in u- and v-direction as params" << std::endl;    
    mfile << "params.p1 = " << tspline->getSDegree() << std::endl;
    mfile << "params.p2 = " << tspline->getTDegree() << std::endl;
    std::cout << "Write params successfully." << std::endl;

//    // write sampling points in parametric coordinates
//    mfile << "%% sampling points in parametric coordinates" << std::endl;
//    mfile << "sampling = ;" << std::endl;
//    mfile << "Pts = zeros (sampling, 2);" << std::endl;
//    mfile << "n = 1;" << std::endl;
//    mfile << "for n = 1: sampling" << std::endl;
//    mfile << "    Pts(n,:)= [(n)/sampling (n)/sampling];" << std::endl;
//    mfile << "end" << std::endl;

    mfile.close();
    std::cout << "T-Spline is exported to " << outfilename << " successfully" << std::endl;

    return 0;
}

