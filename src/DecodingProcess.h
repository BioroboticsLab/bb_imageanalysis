/*
 * DecodingProcess.h
 *
 *  Created on: 21.08.2014
 *      Author: mareikeziese
 */

#ifndef DECODINGPROCESS_H_
#define DECODINGPROCESS_H_

#include <string>

class DecodingProcess {
public:
    DecodingProcess() {}
    virtual ~DecodingProcess() {}

    void process(std::string const& filename) const;
};

#endif /* DECODINGPROCESS_H_ */

