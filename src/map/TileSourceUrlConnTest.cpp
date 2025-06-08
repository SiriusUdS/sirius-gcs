#include "TileSourceUrlConnTest.h"

#include <curl/curl.h>

/**
 * @brief Returns whether the connection test to the tile source was successful
 * @returns True if the test was successful, else false
 */
bool TileSourceUrlConnTest::successful() {
    return lastFetchSuccessful;
}

/**
 * @brief Start the asynchronous connectivity test on a separate detached thread
 */
void TileSourceUrlConnTest::startConnectivityTest(std::string url) {
    std::thread(&TileSourceUrlConnTest::performConnectivityTest, this, url).detach();
}

/**
 * @brief Perform the connectivity test on the url tile source
 */
void TileSourceUrlConnTest::performConnectivityTest(std::string url) {
    std::unique_lock<std::mutex> lock(mtx, std::try_to_lock);
    if (!lock.owns_lock()) {
        return;
    }

    CURL* curl = {curl_easy_init()};
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
    lastFetchSuccessful = (curl_easy_perform(curl) == CURLE_OK);
    curl_easy_cleanup(curl);
}
