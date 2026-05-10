#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

struct Article {
    string name;
    int num_comments;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string*userp){
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;

}

json fetchPage(string url){
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();
    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return json::parse(readBuffer);


}

vector<string> topArtcles(int limit){
    vector<Article> articles;
    string baseUrl = "https://jsonmock.hackerrank.com/api/articles?page=";

    json firstPage = fetchPage(baseUrl + "1");
    int totalPages = firstPage["total_pages"];

    for (int i = 1; i <= totalPages; ++i){
        json currentPage = (i == 1) ? firstPage : fetchPage(baseUrl + to_string(i));

        for (auto& item : currentPage["data"]){
            string name = "";

            if (!item["title"].is_null()){
                name = item["title"];

            } else if (!item["story_title"].is_null()){
                name = item["story_title"];
            }

            if (!name.empty()){
                int comments = item["num_comments"].is_null() ? 0 : (int)item["num_comments"];
                articles.push_back({name, comments});

            }
        }
    }

    sort(articles.begin(), articles.end(), [] (const Article& a, const Article& b) {
        if (a.num_comments != b.num_comments){

            return a.num_comments > b.num_comments;
        }

        return a.name > b.name;

    }) ;

    vector<string> result;
    for (int i = 0; i < min((int)articles.size(), limit); ++i){
        result.push_back(articles[i].name);
    }

    return result;

}

int main() {
    int limit = 5;
    cout << "Fetching top " << limit << "articles..." << endl;

    vector<string> result = topArtcles(limit);

    cout << "Result:" << endl;
    for (const string& title : result){
        cout << "-" << title << endl;
    }

    return 0;

    
}





