#ifndef _STREAMX_SERVER_H_
#define _STREAMX_SERVER_H_

#include <string>

namespace streamx {

class Server {
private:
    std::string make_daytime_string();

public:
    void run();
};

}; //namespace streamx

#endif
