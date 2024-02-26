#include <cpr/cpr.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <string>

int main() {
    std::string api_key = "adadadadadadada";
    std::string error_message = "@everyone";
    std::string webhook_url = "http://20.127.165.86/aimbotalert";

    while (true) {
        // Hata gömülü mesajını oluştur
        std::string json_payload = R"({
            "embeds": [{
                "color": 0,
                "title": "naber nasılsın",
                "description": "naber nasılsın",
                "fields": [
                    {"name": "naber nasılsın", "value": "```)" + error_message + R"(```", "inline": false}
                ],
                "footer": {
                    "text": "naber nasılsın"
                }
            }]
        })";

        // İstek gönder
        cpr::Response response = cpr::Post(cpr::Url{webhook_url},
                                           cpr::Header{{"X-API-KEY", api_key}, {"Content-Type", "application/json"}},
                                           cpr::Body{json_payload});

        // Yanıtı kontrol et
        if (response.status_code == 200 || response.status_code == 204) {
            std::cout << "Hata mesajı başarıyla gönderildi!" << std::endl;
        } else if (response.status_code == 401) {
            std::cout << "Yetkisiz erişim: API Anahtarı geçersiz veya eksik." << std::endl;
        } else {
            std::cout << "Hata mesajı gönderilirken bir sorun oluştu: " << response.text << std::endl;
        }

        // Belirli bir süre bekleyin (örneğin, 1 saniye)
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
