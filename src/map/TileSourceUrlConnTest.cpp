#include "TileSourceUrlConnTest.h"

#include <curl/curl.h>

bool TileSourceUrlConnTest::successful() {
    return lastFetchSuccessful;
}

void TileSourceUrlConnTest::startConnectivityTest(std::string url) {
    std::thread(&TileSourceUrlConnTest::performConnectivityTest, this, url).detach();
}

void TileSourceUrlConnTest::performConnectivityTest(std::string url) {
    std::lock_guard<std::mutex> lock(mtx);
    CURL* curl = {curl_easy_init()};
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
    lastFetchSuccessful = (curl_easy_perform(curl) == CURLE_OK);
    curl_easy_cleanup(curl);
}
