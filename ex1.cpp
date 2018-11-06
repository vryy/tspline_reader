#include <iostream>
#include <string>

#include "rhbuilder.h"
#include "extractor.h"

using namespace TSPLINE;

#define WATCH(a) std::cout << #a << ": " << a << std::endl;
#define WATCH2(n, a) for(int iiii=0;iiii<n;++iiii) std::cout << " "; std::cout << #a << ": " << a << std::endl;

/*
QUESTION:
1. What are differences between TNode and TVertex? Why TVertex has more than one TNode?
2. What is TLink?

*/

int main(int argc, char** argv)
{
//    std::string filename = "/home/hbui/workspace2/T-SPLINE/rhino/simple.tsm";
    std::string filename = std::string(argv[1]);

    RhBuilderPtr reader = makePtr<RhBuilder>(filename);
	TSplinePtr tspline = reader->findTSpline();

    std::cout << "T-Spline is found in " << filename << std::endl;
    WATCH(tspline->getId())
    WATCH(tspline->getName())
    WATCH(tspline->isVirtual())
    WATCH(tspline->getPhysicalId())
    WATCH(tspline->getSDegree())
    WATCH(tspline->getTDegree())

    TImagePtr timage = tspline->getTImage();
    WATCH(timage->sizeFaces())
    std::size_t i, j;

    std::cout << "******Dumping Tpoint******" << std::endl;
    TPointsetPtr tpoints = tspline->getTPointset();
    WATCH(tpoints->size())
    i = 0;
    for (TObjVIterator pit = tpoints->iteratorBegin(); pit != tpoints->iteratorEnd(); ++pit)
    {
        ++i;
        std::cout << "T-point " << i << ":" << std::endl;
        TPointPtr paux = std::dynamic_pointer_cast<TPoint>(*pit);
        paux->setId(i);
        WATCH2(1, paux->getId())
        WATCH2(1, paux->getX())
        WATCH2(1, paux->getY())
        WATCH2(1, paux->getZ())
        WATCH2(1, paux->getW())
    }

    std::cout << "******Dumping Tvertex******" << std::endl;
    i = 0;
    for (TVtxVIterator vit = timage->vertexIteratorBegin(); vit != timage->vertexIteratorEnd(); ++vit)
    {
        ++i;
        std::cout << "T-vertex " << i << ":" << std::endl;
        (*vit)->setId(i);
        WATCH2(1, (*vit)->getId())
        WATCH2(1, (*vit)->getS())
        WATCH2(1, (*vit)->getT())
    }
    std::cout << "***************************************" << std::endl;

    std::cout << "******Dumping Tface******" << std::endl;
    i = 0;
    for (TFacVIterator fit = timage->faceIteratorBegin(); fit != timage->faceIteratorEnd(); ++fit)
    {
        ++i;
        std::cout << "T-face " << i << ":" << std::endl;
        WATCH2(1, (*fit)->getId())
        WATCH2(1, (*fit)->width())
        WATCH2(1, (*fit)->height())
        WATCH2(1, (*fit)->area())
        WATCH2(1, (*fit)->northWest().s())
        WATCH2(1, (*fit)->northWest().t())
        WATCH2(1, (*fit)->southEast().s())
        WATCH2(1, (*fit)->southEast().t())
        WATCH2(1, (*fit)->sizeLinks())
        WATCH2(1, (*fit)->sizeBlendingNodes())
        j = 0;
        std::cout << " nodes:" << std::endl;
        for (TNodVIterator nit = (*fit)->blendingNodeIteratorBegin(); nit != (*fit)->blendingNodeIteratorEnd(); ++nit)
        {
            ++j;
            std::cout << "  node " << j << ":" << std::endl;
            WATCH2(3, typeid(*(*nit)).name())
            WATCH2(3, (*nit)->getId())
            WATCH2(3, (*nit)->getTVertex())
            WATCH2(3, (*nit)->getTVertex()->getId())
            WATCH2(3, (*nit)->getTVertex()->getS())
            WATCH2(3, (*nit)->getTVertex()->getT())
            WATCH2(3, (*nit)->getTPoint())
            WATCH2(3, (*nit)->getTPoint()->getId())
            WATCH2(3, (*nit)->getTPoint()->getX())
            WATCH2(3, (*nit)->getTPoint()->getY())
            WATCH2(3, (*nit)->getTPoint()->getZ())
            WATCH2(3, (*nit)->getTPoint()->getW())
            WATCH2(3, (*nit)->getTPoint()->getTNode())
            WATCH2(3, (*nit)->getTPoint()->getTNode()->getId())
            WATCH2(3, (*nit)->getTPoint()->getTNode()->getTPoint())
            WATCH2(3, (*nit)->getTPoint()->getTNode()->getTVertex())
            WATCH2(3, (*nit)->getTPoint()->getTNode()->getTVertex()->getId())
            WATCH2(3, (*nit)->getTMapper())
            WATCH2(3, typeid(*(*nit)->getTMapper()).name())

            TNodeV4Ptr pnodev4 = std::dynamic_pointer_cast<TNodeV4>(*nit);
            std::vector<Real> uknots, vknots;
            TExtractor::extractUVKnotsFromTNodeV4(pnodev4, uknots, vknots);
            std::cout << "   uknots:";
            for (int ii = 0; ii < uknots.size(); ++ii)
                std::cout << " " << uknots[ii];
            std::cout << std::endl;
            std::cout << "   vknots:";
            for (int ii = 0; ii < vknots.size(); ++ii)
                std::cout << " " << vknots[ii];
            std::cout << std::endl;
        }
    }
    std::cout << "***************************************" << std::endl;

    return 0;
}

