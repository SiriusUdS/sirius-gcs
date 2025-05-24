#ifndef TILESOURCECONNTEST_H
#define TILESOURCECONNTEST_H

#include <future>

class TileSourceUrlConnTest {
public:
    bool successful();
    void startConnectivityTest(std::string url);

private:
    void performConnectivityTest(std::string url);

    std::mutex mtx;
    std::atomic_bool lastFetchSuccessful{false};
};

#endif // TILESOURCEURL_H
