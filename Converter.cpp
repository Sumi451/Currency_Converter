#include <iostream>
#include <string>
#include <curl/curl.h>
using namespace std;

size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* data)
{
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

int main()
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::string fromCurrency, toCurrency;
    double amount;
    std::cout << "Enter the amount to convert: ";
    std::cin >> amount;
    std::cout << "Enter the currency to convert from (e.g. USD): ";
    std::cin >> fromCurrency;
    std::cout << "Enter the currency to convert to (e.g. EUR): ";
    std::cin >> toCurrency;

    std::string url = "https://v6.exchangerate-api.com/v6/dc90b2fc13dfaa331cd0a3b4/latest/" + fromCurrency ;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
            
        }
        else {
            // Parse the received data to extract the exchange rate
            size_t start = readBuffer.find(toCurrency) + 5;
            size_t end = readBuffer.find(",", start);
            string rate = readBuffer.substr(start, end - start);
            double conversionRate = stod(rate);

            // Calculate the converted amount
            double convertedAmount = amount * conversionRate;
            cout << amount << " " << fromCurrency << " = " << convertedAmount << " " << toCurrency << endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}

       



