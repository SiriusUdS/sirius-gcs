#include "ComDiscovery.h"
#include "ComPortSelector.h"

#include <doctest.h>

class MockComDiscovery : public ComDiscovery {
public:
    void getAvailableComPorts(std::vector<std::string>& comPortVec) override {
        comPortVec = comPorts;
    }

    void setComPorts(std::vector<std::string> comPortVec) {
        comPorts = comPortVec;
    }

private:
    std::vector<std::string> comPorts;
};

TEST_CASE("TestComPortSelector should cycle correctly through com ports") {
    MockComDiscovery comDiscovery;
    ComPortSelector cps(comDiscovery);

    CHECK(cps.current() == "");
    cps.next();
    CHECK(cps.current() == "");

    comDiscovery.setComPorts({"COM1", "COM3", "COM5"});
    cps.next();
    CHECK(cps.current() == "COM1");
    cps.next();
    CHECK(cps.current() == "COM3");
    cps.next();
    CHECK(cps.current() == "COM5");
    cps.next();
    CHECK(cps.current() == "COM1");

    comDiscovery.setComPorts({"COM4", "COM6"});
    cps.next();
    CHECK(cps.current() == "COM3");
    cps.next();
    CHECK(cps.current() == "COM5");
    cps.next();
    CHECK(cps.current() == "COM4");
    cps.next();
    CHECK(cps.current() == "COM6");
    cps.next();
    CHECK(cps.current() == "COM4");
    cps.next();
    CHECK(cps.current() == "COM6");
}
