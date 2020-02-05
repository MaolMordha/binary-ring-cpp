#include "Core.hpp"

#include <cstdlib>
#include <tclap/CmdLine.h>

int main(int argc, char const * const * argv)
{
    unsigned int framerate;
    unsigned int particleNumber;
    unsigned int clusterNumber;
    unsigned int maxAge;

    try {
        TCLAP::CmdLine cmd("draw cool particles");

        TCLAP::ValueArg<unsigned int> particleNumberArg(
            "n",
            "particle-number",
            "The number of particle to draw",
            false,
            1000,
            "unsigned int"
        );
        TCLAP::ValueArg<unsigned int> framerateArg(
            "r",
            "framerate",
            "The framerate at which the program should operate",
            false,
            60,
            "unsigned int"
        );
        TCLAP::ValueArg<unsigned int> clusterNumberArg(
            "c",
            "cluster-number",
            "The number of particle cluster to use",
            false,
            1,
            "unsigned int"
        );
        TCLAP::ValueArg<unsigned int> maxAgeArg(
            "a",
            "max-age",
            "The max age of a particle",
            false,
            300,
            "unsigned int"
        );

        cmd.add(particleNumberArg);
        cmd.add(framerateArg);
        cmd.add(clusterNumberArg);
        cmd.add(maxAgeArg);
        cmd.parse(argc, argv);
        framerate = framerateArg.getValue();
        particleNumber = particleNumberArg.getValue();
        clusterNumber = clusterNumberArg.getValue();
        maxAge = maxAgeArg.getValue();
    } catch (TCLAP::ArgException & exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }

    return BinaryRing::Core({1920, 1080}, particleNumber, clusterNumber, framerate, maxAge).run();
}