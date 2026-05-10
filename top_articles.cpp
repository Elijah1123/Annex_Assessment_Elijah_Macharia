#include <bits/stdc++.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* s) {
    s->append((char*)contents, size * nmemb);
    return size * nmemb;
}

json fetchPage(int page) {
    CURL* curl = curl_easy_init();
    string response;
    // FIX 1: was "https:jsonmock..." missing "//"
    string url = "https://jsonmock.hackerrank.com/api/articles?page=" + to_string(page);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // FIX 2: CURLPOT_WRITEFUNCTION -> CURLOPT_WRITEFUNCTION
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    // FIX 3: "culr" -> curl, "CURLPT" -> CURLOPT_WRITEDATA
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return json::parse(response);
}

vector<string> topArticles(int limit) {
    json firstPage = fetchPage(1);
    int totalPages = firstPage["total_pages"].get<int>();

    struct Article {
        string name;
        int numComments;
    }; // FIX 4: was "}}" with extra brace and misplaced

    vector<Article> articles; // FIX 5: moved inside function

    auto processPage = [&](const json& pageData) {
        for (const auto& item : pageData["data"]) {
            string name;
            if (!item["title"].is_null()) {
                name = item["title"].get<string>();
            // FIX 6: added missing story_title fallback
            } else if (!item["story_title"].is_null()) {
                name = item["story_title"].get<string>();
            } else {
                continue;
            }

            // FIX 7: "commets" -> comments, fixed broken null-check logic
            int comments = 0;
            if (!item["num_comments"].is_null()) {
                comments = item["num_comments"].get<int>();
            }

            articles.push_back({name, comments}); // FIX 8: was outside loop scope
        }
    }; // FIX 9: lambda was not closed properly

    processPage(firstPage);

    for (int p = 2; p <= totalPages; ++p) {
        processPage(fetchPage(p));
    }

    sort(articles.begin(), articles.end(), [](const Article& a, const Article& b) {
        if (a.numComments != b.numComments)
            return a.numComments > b.numComments;
        return a.name > b.name;
    });

    vector<string> result;
    for (int i = 0; i < min(limit, (int)articles.size()); ++i) {
        result.push_back(articles[i].name);
    }
    return result;
} // FIX 10: closing brace for topArticles was missing

int main() {
    int limit;
    cin >> limit;

    vector<string> top = topArticles(limit);
    for (const string& name : top) {
        cout << name << "\n";
    }
    return 0;
}